idf_component_register(
    SRCS "envirotrack.c"
    INCLUDE_DIRS "."
    REQUIRES esp_wifi esp_event esp_netif nvs_flash
)