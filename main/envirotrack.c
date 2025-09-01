#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define LED_GPIO GPIO_NUM_2
#define BLINK_DELAY_MS 500

static const char *TAG = "Blink";

void app_main(void)
{
    // Configure GPIO2 as output
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << LED_GPIO,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    ESP_LOGI(TAG, "Starting LED blink loop...");

    while (1)
    {
        gpio_set_level(LED_GPIO, 1); // LED ON
        vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY_MS));

        gpio_set_level(LED_GPIO, 0); // LED OFF
        vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY_MS));
    }
}