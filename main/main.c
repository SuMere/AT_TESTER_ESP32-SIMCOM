#include "sim7000g.h"

void app_main(void) {

    ESP_LOGI("", "**********************************");
    ESP_LOGI("", "*        SIMMCOM AT TESTER       *");
    ESP_LOGI("", "**********************************");

    modem_init();

    test_at("AT\r\n");
}