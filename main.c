#include "Include/nextion.h"


int main(void) {
    nextion_init();

    while(1){
        printf("Sending data...\n");
        // send_read_test();
        send_test();
        // int res = send_req();
        // if(res != E_NO_ERROR) {
        //     printf("Error sending data\n");
        //     return res;
        // }
        // printf("Success");
    }
    MXC_UART_Shutdown(NEXTION_UART_REG);
    return 0;
    
}