#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_log.h"

#define LED_GPIO GPIO_NUM_2
#define BLINK_DELAY_MS 500
#define POT_ADC_CHANNEL ADC1_CHANNEL_6  // GPIO34
#define TAG "EnviroTrack"

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

    // Configure ADC1 Channel 6 (GPIO34)
    adc1_config_width(ADC_WIDTH_BIT_12); // 0–4095 range
    adc1_config_channel_atten(POT_ADC_CHANNEL, ADC_ATTEN_DB_11); // Full range: 0–3.3V

    ESP_LOGI(TAG, "Starting LED blink + ADC read loop...");

    while (1)
    {
        // Blink LED
        gpio_set_level(LED_GPIO, 1);
        vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY_MS));
        gpio_set_level(LED_GPIO, 0);
        vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY_MS));

        // Read ADC value
        int adc_reading = adc1_get_raw(POT_ADC_CHANNEL);
        ESP_LOGI(TAG, "Potentiometer ADC value: %d", adc_reading);
    }
}