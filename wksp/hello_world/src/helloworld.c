#include "master_include.h"




int main()
{
    init_platform();
    XIic_setup();
    
    xil_printf("Command Parsing Active\n\r");
    
    while(1){
        init_uart0();
        
        cli_parse_command();
    }
    return 0;
}



