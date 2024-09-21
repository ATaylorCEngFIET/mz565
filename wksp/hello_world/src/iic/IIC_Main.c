#include "../master_include.h"

int IicLowLevelWrite(u8 address,u8 data)
{

  XIic_DynSend(IIC_BASE_ADDRESS, address,
		  data, sizeof(data), XIIC_STOP);

  return XST_SUCCESS;

}



int XIic_setup()
{
  int Status;
  u32 StatusReg;

  Status = XIic_DynInit(IIC_BASE_ADDRESS);
  if (Status != XST_SUCCESS) {
	return XST_FAILURE;
  }

  while (((StatusReg = XIic_ReadReg(IIC_BASE_ADDRESS,
				XIIC_SR_REG_OFFSET)) &
				(XIIC_SR_RX_FIFO_EMPTY_MASK |
				XIIC_SR_TX_FIFO_EMPTY_MASK |
				XIIC_SR_BUS_BUSY_MASK)) !=
				(XIIC_SR_RX_FIFO_EMPTY_MASK |
				XIIC_SR_TX_FIFO_EMPTY_MASK)) {

  }
  return XST_SUCCESS;
}
