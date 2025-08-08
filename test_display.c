#include "Include/nextion.h"


int main(void) {
    nextion_init();
    float data = 3405.11f;
    int i = 0;
    while(i < 100){
        printf("Sending data...\n");
        update_weight(data++, TXT_WEIGHT_0);
        MXC_Delay(MXC_DELAY_MSEC(500));
        i++;
    }
    MXC_UART_Shutdown(NEXTION_UART_REG);
    return 0;
    
}