#include "../include/sim7000g.h"
#include <string.h>

#define APN ""

static const char *TAG = "SIM7000G";
static int rssi_conversion[] = {-113,-111,-109,-107,-105,-103,-101,-99,-97,-95,-93,-91,-89,-87,-85,-83,-81,-79,-77,-75,-73,-71,-69,-67,-65,-63,-61,-59,-57,-55,-53};

#define DCE_CHECK(a, str, goto_tag, ...)                                              \
    do                                                                                \
    {                                                                                 \
        if (!(a))                                                                     \
        {                                                                             \
            ESP_LOGE(TAG, "%s(%d): " str, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
            goto goto_tag;                                                            \
        }                                                                             \
    } while (0)

esp_err_t gpio_setup() {
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    return ESP_OK;
}

esp_err_t uart_setup() {

    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    uart_driver_install(UART_NUMBER, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart0_queue, 0);
    ESP_ERROR_CHECK(uart_param_config(UART_NUMBER, &uart_config));
    uart_set_pin(UART_NUMBER, MODEM_UART_TX, MODEM_UART_RX,
                UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    ESP_LOGI(TAG, "UART OK");

    return ESP_OK;
}

esp_err_t modem_on() {
    //MAKE SURE MODEM IS OFF IN D47
    gpio_set_level(GSM_VLTE_GPIO, 0);
    vTaskDelay(500/portTICK_PERIOD_MS);

    //THEN TURN IT ON
    gpio_set_level(GSM_VLTE_GPIO, 1);
    //WAIT
    vTaskDelay(1000/portTICK_PERIOD_MS);

    //SEND ENABLE SIG AND WAIT MODEM TO START
    gpio_set_level(GSM_ENABLE_GPIO, 1);
    vTaskDelay(1500/portTICK_PERIOD_MS);
    gpio_set_level(GSM_ENABLE_GPIO, 0);
    vTaskDelay(20000/portTICK_PERIOD_MS);
    return ESP_OK;
}

void modem_sync() {
    bool flag = true;

    const char* command = "AT\r\n\r\n";

    ESP_LOGI(TAG, "SENDING COMMAND =======> %s", command);
    
    ESP_LOGI(TAG, "BYTES WRITTEN, WAITING FOR RESPONCE");

    uint8_t* data = (uint8_t*) malloc(BUF_SIZE+1);

    while (flag) {
        const int rxBytes = uart_read_bytes(UART_NUMBER, data, BUF_SIZE, 1000 / portTICK_RATE_MS);
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            ESP_LOGI(TAG, "Read %d bytes: '%s'", rxBytes, data);
            ESP_LOG_BUFFER_HEXDUMP(TAG, data, rxBytes, ESP_LOG_INFO);

            flag = false;
        } else {
            uart_write_bytes(UART_NUMBER, command, strlen(command));
            uart_flush(UART_NUMBER);

            ESP_LOGI(TAG, "WAITIN'");
        }
    }

    free(data);

    ESP_LOGI(TAG, "SYNC OK");
}

void test_at(const char* command) {
    bool flag = true;
    int counter = 0;

    ESP_LOGI(TAG, "SENDING COMMAND =======> %s", command);
    ESP_LOGI(TAG, "BYTES WRITTEN, WAITING");
    uart_write_bytes(UART_NUMBER, command, strlen(command));

    uint8_t* data = (uint8_t*) malloc(BUF_SIZE+1);

    while (flag) {
        const int rxBytes = uart_read_bytes(UART_NUMBER, data, BUF_SIZE, 1000 / portTICK_RATE_MS);
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            ESP_LOGI(TAG, "Read %d bytes: '%s'", rxBytes, data);
            ESP_LOG_BUFFER_HEXDUMP(TAG, data, rxBytes, ESP_LOG_INFO);

            if(strstr((char *)data, "OK")){
                ESP_LOGI("INFO", "OK FOUND");
            }
            
            if(strstr((char *)data, "+CSQ") != NULL){
                int values[2];
                
                sscanf((char *) data, "%*s%d,%d", &values[0], &values[1]);
                
                ESP_LOGI("", "*************************************************");
                ESP_LOGI("", "*\tSIGNAL STRENGHT IS: %d", rssi_conversion[values[0]]);
                ESP_LOGI("", "*************************************************");
            }

            flag = false;
        }else if(counter > 30){
            ESP_LOGE(TAG, "COMMAND TIMEOUT");
            flag = false;
        }else {
            uart_flush(UART_NUMBER);

            ESP_LOGI(TAG, "WAITIN'");

            vTaskDelay(500/portTICK_PERIOD_MS);
        }
        counter++;
    }

    free(data);
}

esp_err_t modem_init() {
    int counter = 100;
    
    gpio_setup();
    modem_on();
    uart_setup();

    return ESP_OK;
}