#include "dht11.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_rom_sys.h"   // for esp_rom_delay_us()

#define DHT11_TAG "DHT11"

static gpio_num_t dht11_gpio = GPIO_NUM_NC;  // store selected GPIO

esp_err_t dht11_init(gpio_num_t gpio_num)
{
    if (gpio_num < 0 || gpio_num >= GPIO_NUM_MAX) {
        return ESP_ERR_INVALID_ARG;
    }

    dht11_gpio = gpio_num;

    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << dht11_gpio,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    return gpio_config(&io_conf);
}

/**
 * @brief Send start signal to DHT11
 */
static void dht11_send_start_signal(void)
{
    gpio_set_direction(dht11_gpio, GPIO_MODE_OUTPUT);
    gpio_set_level(dht11_gpio, 0);
    esp_rom_delay_us(20000); // 20 ms
    gpio_set_level(dht11_gpio, 1);
    esp_rom_delay_us(30);    // 20–40 µs
    gpio_set_direction(dht11_gpio, GPIO_MODE_INPUT);
    esp_rom_delay_us(40);    // recovery before response
}

/**
 * @brief Wait for pin to go to expected level within timeout (µs)
 */
static int dht11_wait_for_level(int level, int timeout_us)
{
    int t = 0;
    while (gpio_get_level(dht11_gpio) != level) {
        if (++t > timeout_us) {
            return -1; // timeout
        }
        esp_rom_delay_us(1);
    }
    return t;
}

esp_err_t dht11_read(dht11_data_t *out_data)
{
    if (!out_data || dht11_gpio == GPIO_NUM_NC) {
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t data[5] = {0};

    // Start signal
    dht11_send_start_signal();

    // Response sequence
    if (dht11_wait_for_level(0, 200) < 0) return ESP_FAIL; // 80 µs low
    if (dht11_wait_for_level(1, 200) < 0) return ESP_FAIL; // 80 µs high
    if (dht11_wait_for_level(0, 200) < 0) return ESP_FAIL; // ready to send data

    // Read 40 bits
    for (int i = 0; i < 40; i++) {
        if (dht11_wait_for_level(1, 100) < 0) return ESP_FAIL; // start of bit

        int high_time = dht11_wait_for_level(0, 150); // measure high
        if (high_time < 0) return ESP_FAIL;

        data[i / 8] <<= 1;
        if (high_time > 50) {  // threshold ~50 µs
            data[i / 8] |= 1;
        }
    }

    // Checksum
    uint8_t checksum = data[0] + data[1] + data[2] + data[3];
    if (checksum != data[4]) {
        ESP_LOGW(DHT11_TAG, "Checksum mismatch: expected %d, got %d", checksum, data[4]);
        return ESP_FAIL;
    }

    out_data->humidity = data[0];
    out_data->temperature = data[2];

    return ESP_OK;
}
