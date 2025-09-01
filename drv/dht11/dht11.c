#include "dht11.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define TAG "DHT11"
#define DHT11_GPIO gpio_num

static gpio_num_t dht11_gpio = GPIO_NUM_NC;

esp_err_t dht11_init(gpio_num_t gpio_num) {
    if (!GPIO_IS_VALID_GPIO(gpio_num)) {
        ESP_LOGE(TAG, "Invalid GPIO: %d", gpio_num);
        return ESP_ERR_INVALID_ARG;
    }

    dht11_gpio = gpio_num;
    gpio_set_direction(dht11_gpio, GPIO_MODE_INPUT_OUTPUT_OD);
    gpio_set_pull_mode(dht11_gpio, GPIO_PULLUP_ONLY);
    return ESP_OK;
}

static int wait_for_level(int level, uint32_t timeout_us) {
    uint32_t start = esp_timer_get_time();
    while (gpio_get_level(dht11_gpio) != level) {
        if ((esp_timer_get_time() - start) > timeout_us) {
            return -1;
        }
    }
    return esp_timer_get_time() - start;
}

esp_err_t dht11_read(dht11_data_t *out_data) {
    if (dht11_gpio == GPIO_NUM_NC) return ESP_ERR_INVALID_STATE;

    uint8_t data[5] = {0};

    // Start signal
    gpio_set_direction(dht11_gpio, GPIO_MODE_OUTPUT);
    gpio_set_level(dht11_gpio, 0);
    vTaskDelay(pdMS_TO_TICKS(20));  // >18ms
    gpio_set_level(dht11_gpio, 1);
    ets_delay_us(30);
    gpio_set_direction(dht11_gpio, GPIO_MODE_INPUT);

    // Wait for response
    if (wait_for_level(0, 80) < 0 || wait_for_level(1, 80) < 0) {
        ESP_LOGE(TAG, "No response from DHT11");
        return ESP_FAIL;
    }

    // Read 40 bits
    for (int i = 0; i < 40; i++) {
        if (wait_for_level(0, 50) < 0) return ESP_FAIL;
        int pulse = wait_for_level(1, 70);
        if (pulse < 0) return ESP_FAIL;

        data[i / 8] <<= 1;
        if (pulse > 40) data[i / 8] |= 1;
    }

    // Verify checksum
    uint8_t checksum = data[0] + data[1] + data[2] + data[3];
    if (checksum != data[4]) {
        ESP_LOGE(TAG, "Checksum failed");
        return ESP_FAIL;
    }

    out_data->humidity = data[0];
    out_data->temperature = data[2];
    return ESP_OK;
}