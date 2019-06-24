#include "resps.h"

#include "xil_printf.h"

#include "../avrsysmon2_configs.h"

RESP_FUNC( Resp_ServiceModeDetect )
{
	(nByte);
	if( ( nProgress == 0 ) && bEoT )
    {
        //xil_printf("\r\nService Mode Detect\r\n");
        *bFinished = true;
        return true;
    }
    else
    {
        xil_printf("\r\nUnexpected data in Resp_ServiceModeDetect packet\r\n");
        *bFinished = true;
        return false;
    }
}

RESP_FUNC( Resp_ApplicationModeDetect )
{
	(nByte);
	if( ( nProgress == 0 ) && bEoT )
    {
        //xil_printf("\r\nApplication Mode Detect\r\n");
        *bFinished = true;
        return true;
    }
    else
    {
        xil_printf("\r\nUnexpected data in Resp_ApplicationModeDetect packet\r\n");
        *bFinished = true;
        return false;
    }
}

#ifdef FULL_RESPS_AUTO
RESP_FUNC( Resp_FirmwareErrorDetect )
{
	(nByte);
	if( (nProgress == 0) && bEoT )
	{
		xil_printf("\r\nFirmware Error Resp_FirmwareErrorDetect Detect\r\n");
		*bFinished = true;
		return true;
	}
	else
	{
		xil_printf("\r\nUnexpected data in packet\r\n");
		*bFinished = true;
		return false;
	}
}

#endif
