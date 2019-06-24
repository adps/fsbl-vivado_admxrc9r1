#include "emmc_test.h"
#include "xsdps.h"


#ifdef EMMC_TEST

#pragma GCC optimize ("O0")

void emmcTest( void )
{

	XSdPs_Config* pConfig = XSdPs_LookupConfig( 0 );

	XSdPs SdPsInstance;
	s32 sResult =  XSdPs_CfgInitialize(&SdPsInstance, pConfig, 0xff160000);
	xil_printf("CfgInit: 0x%08x\r\n", sResult);

	sResult = XSdPs_CardInitialize(&SdPsInstance);
	xil_printf("XSdPs_CardInitialize: 0x%08x\r\n", sResult);

	//u8 nBusWidth = 8;
	//u8 nBusSpeed = 400000;
	//sResult = XSdPs_Get_BusWidth(&SdPsInstance, &nBusWidth);
	//xil_printf("BusWidthStatus: 0x%08x, BusWidth: %u\r\n", sResult, nBusWidth);
	//sResult = XSdPs_Get_BusSpeed(&SdPsInstance, &nBusSpeed);
	//xil_printf("BusSpeedStatus: 0x%08x, BusSpeed: %u\r\n", sResult, nBusSpeed);

	volatile u8 nTestData[XSDPS_BLK_SIZE_512_MASK];
	volatile u8 nReadData[XSDPS_BLK_SIZE_512_MASK];
	u32 i=0;

	while(1)
	{
		for(i=0; i<XSDPS_BLK_SIZE_512_MASK; i++)
		{
			nTestData[i] = rand()%0xff;
		}
		sResult = XSdPs_WritePolled(&SdPsInstance, 0, 1, nTestData);
		sResult = XSdPs_ReadPolled(&SdPsInstance, 0, 1, nReadData);
		for(u32 i=0; i<XSDPS_BLK_SIZE_512_MASK; i++ )
		{
			if( nReadData[i] != nTestData[i])
			{
				xil_printf("Error: Expected 0x%02x, found 0x%02x\r\n", nTestData[i], nReadData[i]);
			}
		}
		//xil_printf("WritePolled: 0x%08x\r\n", sResult);
	}

	for(i=0; i<XSDPS_BLK_SIZE_512_MASK; i++)
	{
		nTestData[i] = 0xdd;
	}
	for(i=0; i<XSDPS_BLK_SIZE_512_MASK; i++)
	{
		xil_printf("TestData[%d]: 0x%02x\r\n", i, nTestData[i]);
	}

	xil_printf("ReadPolled: 0x%08x\r\n", sResult);
	for(i=0; i<XSDPS_BLK_SIZE_512_MASK; i++)
	{
		xil_printf("TestData[%d]: 0x%02x\r\n", i, nTestData[i]);
	}


}

#endif
