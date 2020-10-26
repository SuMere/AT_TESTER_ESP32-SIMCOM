#include "sim7000g.h"

void app_main(void) {

    ESP_LOGI("", "*****************************************");
    ESP_LOGI("", "*        SIMMCOM AT TESTER => GPS       *");
    ESP_LOGI("", "*****************************************");

    modem_init();

    test_at("AT\r\n");

    test_at("AT+CGNSPWR=1\r\n");
    vTaskDelay(pdMS_TO_TICKS(60000));
    
    while (true)
    {
        test_at("AT+CGNSINF\r\n");
    }
    
}