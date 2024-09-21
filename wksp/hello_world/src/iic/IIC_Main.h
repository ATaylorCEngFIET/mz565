#ifndef _IIC_H_
#define _IIC_H_

#define IIC_BASE_ADDRESS	XPAR_XIIC_0_BASEADDR

int XIic_setup();
int IicLowLevelWrite(u8 address,u8 data);

#endif