#include "../master_include.h"

XUartLite Uart;

char receive_buffer[CLI_BUFFER_SIZE];    /* Buffer for Receiving Data */
char receive_buffer_lst[CLI_BUFFER_SIZE];    /* Buffer for Receiving Data */
int mesg_ready;

int test_id = 0;
int test_stop = 0;
int test_oneshot = 0;
char command_delim[] = " ";



int init_uart0()
{
    int Status;

    Status = XUartLite_Initialize(&Uart, XPAR_XUARTLITE_0_BASEADDR);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    return XST_SUCCESS;

}

void xil_printf_float(float x){
    int integer, fraction, abs_frac;
    integer = x;
    fraction = (x - integer) * 100;
    abs_frac = abs(fraction);
    xil_printf("%d.%3d\n\r", integer, abs_frac);

}

int read_serial(char *buffer)
{
	static u8 read_val[3] = {0,0,0};
    unsigned int received_count = 0;
    static int fisrt = 1;
    static int index = 0;
    static int index_lst = 0;



    received_count =    XUartLite_Recv(&Uart, &read_val[received_count], 1);
    if (received_count > 0)
    {

        if (read_val[0] == '\r'){
            buffer[index] = '\0';

            // Save the entered command for when we use Arrow up
            index_lst = index;
            strcpy(receive_buffer_lst,receive_buffer);

            index = 0;
            fisrt = 0;
            //xil_printf("\n\r");

            return 1;
        }

        // Handles "arrow up" key press
        if (read_val[2] == 27 && read_val[1] == 91 && read_val[0] == 65 && fisrt == 0){
            // restore last entered command
        	strcpy(receive_buffer,receive_buffer_lst);
            index = index_lst;

            xil_printf("\b%s",receive_buffer);

            return 0;
        }

        // Handles backspace
        if (read_val[0] == 8)
        {
        	xil_printf("\b \b");
        	if (index > 0)
        	{
        		index--;
        	}

        }

        // Prevents buffer overflow
        if (index >= CLI_BUFFER_SIZE-1)
        {
            buffer[index] = '\0';
            index = 0;
            xil_printf("\n\r");
            return 1;
        }

        // Prevents not printable characters from being added to the buffer
        if (read_val[0] >= 32 && read_val[0] <= 126)
        {
            buffer[index++] = read_val[0];
            //xil_printf("%c", read_val[0]);
        }

        read_val[2] = read_val[1];
        read_val[1] = read_val[0];
    }
    return 0;
}

void cli_parse_command()
{
    // /static u32 block = 0;
    static u32 val = 0;

    mesg_ready = read_serial(receive_buffer);
    if (mesg_ready == 1)
    {
        //xil_printf("cmd> %s\n\r", receive_buffer);

        if (strcmp(receive_buffer, "") == 0)
		{
            val = 0; // NOP
		}
    
        else if (strcmp(receive_buffer, "read_accel_y") == 0)
        {
                //xil_printf("read_accel_y\n\r");
            //    NAV_GetData(&nav);
            //    xil_printf_float(nav.acclData.Y);

        }

        else if (strcmp(receive_buffer, "read_accel_z") == 0)
        {
            //xil_printf("read_accel_z\n\r");
          //  NAV_GetData(&nav);
          //  xil_printf_float(nav.acclData.Z);
        }
        else
        {
            char *ptr = strtok(receive_buffer, command_delim);

            if (strcmp(receive_buffer, "ltc_2974id") == 0)
            {
                
                u8 addr = 0x32;
                u8 data[1] = {0xe7};
                u8 rx[2];
                XIic_DynSend(IIC_BASE_ADDRESS, addr,
		        data, sizeof(data), XIIC_REPEATED_START);
                XIic_DynRecv(IIC_BASE_ADDRESS, addr, 
                rx, 2);
                xil_printf("%x %x \n\r",rx[1], rx[0] );

            }
           
            else if (strcmp(receive_buffer, "iic_temp") == 0)
            {
                
                u8 addr = 0x32;
                u8 data[4] = {0x00,0x00,0x00,0x04};
                u8 rx[2];
                u16 temp_reg;
                float temp;
                XIic_DynSend(IIC_BASE_ADDRESS, addr,
		        data, sizeof(data), XIIC_REPEATED_START);
                XIic_DynRecv(IIC_BASE_ADDRESS, addr, 
                rx, 2);
                temp_reg = rx[1]<<8 | rx[0];
                temp = (( (float) temp_reg * 509.3140064) / 65536 ) - 280.23087870;
                xil_printf_float(temp);

            }

            else if (strcmp(receive_buffer, "iic_vccint") == 0)
            {
                
                u8 addr = 0x32;
                u8 data[4] = {0x00,0x00,0x01,0x04};
                u8 rx[2];
                u16 vcc_reg;
                float vcc;
                XIic_DynSend(IIC_BASE_ADDRESS, addr,
		        data, sizeof(data), XIIC_REPEATED_START);
                XIic_DynRecv(IIC_BASE_ADDRESS, addr, 
                rx, 2);
                vcc_reg = rx[1]<<8 | rx[0];
                vcc = ( (float) vcc_reg  / 65536 ) * 3.0;
                xil_printf("VCCint Voltage = ");
                xil_printf_float(vcc);

            }

            else if (strcmp(receive_buffer, "iic_vccaux") == 0)
            {
                
                u8 addr = 0x32;
                u8 data[4] = {0x00,0x00,0x02,0x04};
                u8 rx[2];
                u16 vcc_reg;
                float vcc;
                XIic_DynSend(IIC_BASE_ADDRESS, addr,
		        data, sizeof(data), XIIC_REPEATED_START);
                XIic_DynRecv(IIC_BASE_ADDRESS, addr, 
                rx, 2);
                vcc_reg = rx[1]<<8 | rx[0];
                vcc = ( (float) vcc_reg  / 65536 ) * 3.0;
                xil_printf("AUX Voltage = ");
                xil_printf_float(vcc);

            }

            else if (strcmp(receive_buffer, "iic_vccbram") == 0)
            {
                
                u8 addr = 0x32;
                u8 data[4] = {0x00,0x00,0x06,0x04};
                u8 rx[2];
                u16 vcc_reg;
                float vcc;
                XIic_DynSend(IIC_BASE_ADDRESS, addr,
		        data, sizeof(data), XIIC_REPEATED_START);
                XIic_DynRecv(IIC_BASE_ADDRESS, addr, 
                rx, 2);
                vcc_reg = rx[1]<<8 | rx[0];
                vcc = ( (float) vcc_reg  / 65536 ) * 3.0;
                xil_printf("BRAM Voltage = ");
                xil_printf_float(vcc);

            }

            else if (strcmp(receive_buffer, "iic_vccuser0") == 0)
            {
                
                u8 addr = 0x32;
                u8 data[4] = {0x00,0x00,0x80,0x04};
                u8 rx[2];
                u16 vcc_reg;
                float vcc;
                XIic_DynSend(IIC_BASE_ADDRESS, addr,
		        data, sizeof(data), XIIC_REPEATED_START);
                XIic_DynRecv(IIC_BASE_ADDRESS, addr, 
                rx, 2);
                vcc_reg = rx[1]<<8 | rx[0];
                vcc = ( (float) vcc_reg  / 65536 ) * 3.0;
                xil_printf("VCC User 0 Voltage = ");
                xil_printf_float(vcc);

            }

            else if (strcmp(receive_buffer, "iic_vccuser1") == 0)
            {
                
                u8 addr = 0x32;
                u8 data[4] = {0x00,0x00,0x81,0x04};
                u8 rx[2];
                u16 vcc_reg;
                float vcc;
                XIic_DynSend(IIC_BASE_ADDRESS, addr,
		        data, sizeof(data), XIIC_REPEATED_START);
                XIic_DynRecv(IIC_BASE_ADDRESS, addr, 
                rx, 2);
                vcc_reg = rx[1]<<8 | rx[0];
                vcc = ( (float) vcc_reg  / 65536 ) * 3.0;
                xil_printf("VCC User 1 Voltage = ");
                xil_printf_float(vcc);

            }

            else if (strcmp(receive_buffer, "iic_vccuser2") == 0)
            {
                
                u8 addr = 0x32;
                u8 data[4] = {0x00,0x00,0x82,0x04};
                u8 rx[2];
                u16 vcc_reg;
                float vcc;
                XIic_DynSend(IIC_BASE_ADDRESS, addr,
		        data, sizeof(data), XIIC_REPEATED_START);
                XIic_DynRecv(IIC_BASE_ADDRESS, addr, 
                rx, 2);
                vcc_reg = rx[1]<<8 | rx[0];
                vcc = ( (float) vcc_reg  / 65536 ) * 3.0;
                xil_printf("VCC User 2 Voltage = ");
                xil_printf_float(vcc);

            }

            else if (strcmp(receive_buffer, "iic_vccuser3") == 0)
            {
                
                u8 addr = 0x32;
                u8 data[4] = {0x00,0x00,0x83,0x04};
                u8 rx[2];
                u16 vcc_reg;
                float vcc;
                XIic_DynSend(IIC_BASE_ADDRESS, addr,
		        data, sizeof(data), XIIC_REPEATED_START);
                XIic_DynRecv(IIC_BASE_ADDRESS, addr, 
                rx, 2);
                vcc_reg = rx[1]<<8 | rx[0];
                vcc = ( (float) vcc_reg  / 65536 ) * 3.0;
                xil_printf("VCC User 3 Voltage = ");
                xil_printf_float(vcc);

            }


            else if (strcmp(ptr, "r") == 0)
            {
                ptr = strtok(NULL, command_delim);
                u32 addr = char_to_int(strlen(ptr), ptr);
                val = Xil_In32( XPAR_XGPIO_0_BASEADDR + addr);
                xil_printf("Addr: 0x%x (%d)  Val: 0x%x (%d)\r\n",addr,addr, val, val);

            }
            else if (strcmp(ptr, "w") == 0)
            {
                ptr = strtok(NULL, command_delim);
                u32 addr = char_to_int(strlen(ptr), ptr);

                ptr = strtok(NULL, command_delim);
                val = char_to_int(strlen(ptr), ptr);

                Xil_Out32(XPAR_XGPIO_0_BASEADDR + addr,val);

                val = Xil_In32( XPAR_XGPIO_0_BASEADDR + addr);
                xil_printf("Addr: 0x%x (%d)  Val: 0x%x (%d)\r\n",addr,addr, val, val);

            }
            else
            {
                xil_printf("Command not found.\n\r");
            }

        }
    }

}

u8 string_to_u8(const char *str) {
    uint8_t value = 0;

    // Convert first char to upper 4 bits
    if (str[0] >= '0' && str[0] <= '9') {
        value |= (str[0] - '0') << 4;
    } else if (str[0] >= 'A' && str[0] <= 'F') {
        value |= (str[0] - 'A' + 10) << 4;
    } else if (str[0] >= 'a' && str[0] <= 'f') {
        value |= (str[0] - 'a' + 10) << 4;
    }

    // Convert second char to lower 4 bits
    if (str[1] >= '0' && str[1] <= '9') {
        value |= (str[1] - '0');
    } else if (str[1] >= 'A' && str[1] <= 'F') {
        value |= (str[1] - 'A' + 10);
    } else if (str[1] >= 'a' && str[1] <= 'f') {
        value |= (str[1] - 'a' + 10);
    }

    return value;
}


int char_to_int(int len, char* buffer)
{

    int value = 0, char_val, scalar = 1;

    for (int i = len - 1; i >= 0; i--) {
        char_val = (int)(buffer[i] - '0');
        value = value + (char_val * scalar);
        scalar = scalar * 10;
    }

    return value;
}

