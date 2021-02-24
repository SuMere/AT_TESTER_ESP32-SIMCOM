#include "sim7000g.h"

/**
 * USEFUL COMMANDS TO TEST
 * 
 * "AT+CGSN\r" To retrieve IMEI number
 * "AT+CIMI\r" To retrieve IMSI number
 * "AT+COPS?\r" To get operator name
 * "AT+CGREG?\r" To get registration status
 * "AT+CSQ\r" To get signal quality
 */

void set_apn(uint32_t cid, const char* type, const char* apn){
    char cmd[100];
    
    sprintf(cmd, "AT+CGDCONT=%d,\"%s\",\"%s\"\r\n", cid, type, apn);

    test_at(cmd);
}

void app_main(void) {

    ESP_LOGI("", "**********************************");
    ESP_LOGI("", "*        SIMMCOM AT TESTER       *");
    ESP_LOGI("", "**********************************");

    modem_init();

    test_at("AT\r\n");    
}