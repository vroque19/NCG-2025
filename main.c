#include "Include/nextion.h"


int main(void) {
    nextion_init();
    float data = 4343.69f;
    char output_string[32];
    sprintf(output_string, "\"%.2f\"", data);
    while(1){
        printf("Sending data...\n");
        nextion_send_command(output_string);
        // send_read_test();
        // send_test();
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