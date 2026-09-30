#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"

#define TAG "EnviroTrack"

void app_main(void)
{
    ESP_LOGI(TAG, "EnviroTrack starting...");

    esp_err_t ret = nvs_flash_init();

    if(ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize NVS flash: %s", esp_err_to_name(ret));
        return;
    }

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Wi-Fi initialized and started in station mode.");
    ESP_LOGI(TAG, "Starting Wi-Fi scan...");

    wifi_scan_config_t scan_config = {
        .ssid = NULL,
        .bssid = NULL,
        .channel = 0,
        .show_hidden = true,
    };

    ESP_ERROR_CHECK(
        esp_wifi_scan_start(&scan_config, true)
    );

    uint16_t ap_count = 0;

    ESP_ERROR_CHECK(
        esp_wifi_scan_get_ap_num(&ap_count)
    );

    ESP_LOGI(TAG, "Found %u access points.", ap_count);

    if (ap_count > 0) {

        wifi_ap_record_t *ap_records =
            malloc(sizeof(wifi_ap_record_t) * ap_count);

        if (ap_records == NULL) {
            ESP_LOGE(TAG, "Failed to allocate memory for AP records");
            return;
        }

        uint16_t record_count = ap_count;

        ESP_ERROR_CHECK(
            esp_wifi_scan_get_ap_records(
                &record_count,
                ap_records
            )
        );

        for (uint16_t i = 0; i < record_count; i++) {

            ESP_LOGI(
                TAG,
                "[%u] SSID: %s | RSSI: %d dBm | Channel: %d",
                i,
                (char *)ap_records[i].ssid,
                ap_records[i].rssi,
                ap_records[i].primary
            );
        }

        free(ap_records);
    }

    ESP_LOGI(TAG, "Wi-Fi scan completed.");

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}