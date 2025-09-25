#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "dht11.h"

#define TAG "EnviroTrack"
#define DHT11_GPIO GPIO_NUM_4   // Change this to the GPIO you wired the DHT11 to

void app_main(void)
{
    esp_err_t ret = dht11_init(DHT11_GPIO);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize DHT11 on GPIO %d", DHT11_GPIO);
        return;
    }

    ESP_LOGI(TAG, "DHT11 initialized on GPIO %d", DHT11_GPIO);

    while (1) {
        dht11_data_t data;
        ret = dht11_read(&data);

        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "Temperature: %d °C | Humidity: %d %%", 
                     data.temperature, data.humidity);
        } else {
            ESP_LOGW(TAG, "Failed to read from DHT11 sensor");
        }

        vTaskDelay(pdMS_TO_TICKS(2000)); // DHT11 min sampling period ~2s
    }
}
