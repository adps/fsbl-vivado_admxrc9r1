#include "resps.h"

#include "xil_printf.h"

#include "../avrsysmon2_configs.h"
#ifdef FULL_RESPS_AUTO_EXT

RESP_FUNC( Resp_AlarmTemperatureW )
{
	(nByte);
	if( (*nProgress == 0) && bEoT )
	{
		xil_printf("\r\nThermal State Warning Detect\r\n");
		*bFinished = true;
		return true;
	}
	else
	{
		xil_printf("\r\nUnexpected data in Resp_AlarmTemperatureW packet\r\n");
		*bFinished = true;
		return false;
	}
}

RESP_FUNC( Resp_AlarmTemperatureC )
{
	(nByte);
	if( (*nProgress == 0) && bEoT )
	{
		xil_printf("\r\nThermal State Critical Detect\r\n");
		*bFinished = true;
		return true;
	}
	else
	{
		xil_printf("\r\nUnexpected data in Resp_AlarmTemperatureC packet\r\n");
		*bFinished = true;
		return false;
	}
}

RESP_FUNC( Resp_AlarmVoltageW )
{
	(nByte);
	if( (*nProgress == 0) && bEoT )
	{
		xil_printf("\r\nPower State Warning Detect\r\n");
		*bFinished = true;
		return true;
	}
	else
	{
		xil_printf("\r\nUnexpected data in Resp_AlarmVoltageW packet\r\n");
		*bFinished = true;
		return false;
	}
}

RESP_FUNC( Resp_AlarmVoltageC )
{
	(nByte);
	if( (*nProgress == 0) && bEoT )
	{
		xil_printf("\r\nPower State Critical Detect\r\n");
		*bFinished = true;
		return true;
	}
	else
	{
		xil_printf("\r\nUnexpected data in Resp_AlarmVoltageC packet\r\n");
		*bFinished = true;
		return false;
	}
}
#endif

RESP_FUNC( Resp_SensorUpdate )
{
	static unsigned char pData[64];
	if( *nProgress < 63 )
	{
		pData[*nProgress] = nByte;
		(*nProgress)++;
		*bFinished = false;
		return !bEoT;
	}
	else
	{
		pData[*nProgress] = nByte;
		(*nProgress)++;
		*bFinished = true;
		//unsigned int nErrors = 0;
		// TODO do something with sensors
		//xil_printf("\r\nSensor Update Recevied\r\n");
		return bEoT;
	}
}

RESP_FUNC( Resp_VPDUpdate )
{
	static unsigned char pData[256];
	if( *nProgress < 255 )
	{
		pData[*nProgress] = nByte;
		(*nProgress)++;
		*bFinished = false;
		return !bEoT;
	}
	else
	{
		pData[*nProgress] = nByte;
		(*nProgress)++;
		*bFinished = true;
		//unsigned int nErrors = 0;
		// TODO do something with vpd
		//xil_printf("\r\nVPD Update Recevied\r\n");
		return bEoT;
	}
}

#ifdef FULL_RESPS_AUTO_EXT
RESP_FUNC( Resp_Mez0ModuleUpdate )
{
	static unsigned char pData[256];
	if( *nProgress < 255 )
	{
		pData[*nProgress] = nByte;
		(*nProgress)++;
		*bFinished = false;
		return !bEoT;
	}
	else
	{
		pData[*nProgress] = nByte;
		(*nProgress)++;
		*bFinished = true;
		//unsigned int nErrors = 0;
		// TODO do something with vpd
		xil_printf("\r\nMEZ0 Update Recevied\r\n");
		return bEoT;
	}
}

RESP_FUNC( Resp_Mez1ModuleUpdate )
{
	static unsigned char pData[256];
	if( *nProgress < 255 )
	{
		pData[*nProgress] = nByte;
		(*nProgress)++;
		*bFinished = false;
		return !bEoT;
	}
	else
	{
		pData[*nProgress] = nByte;
		(*nProgress)++;
		*bFinished = true;
		//unsigned int nErrors = 0;
		// TODO do something with vpd
		xil_printf("\r\nMEZ1 Update Recevied\r\n");
		return bEoT;
	}
}

RESP_FUNC( Resp_Mez2ModuleUpdate )
{
	static unsigned char pData[256];
	if( *nProgress < 255 )
	{
		pData[*nProgress] = nByte;
		(*nProgress)++;
		*bFinished = false;
		return !bEoT;
	}
	else
	{
		pData[*nProgress] = nByte;
		(*nProgress)++;
		*bFinished = true;
		//unsigned int nErrors = 0;
		// TODO do something with vpd
		xil_printf("\r\nMEZ2 Update Recevied\r\n");
		return bEoT;
	}
}

RESP_FUNC( Resp_Mez3ModuleUpdate )
{
	static unsigned char pData[256];
	if( *nProgress < 255 )
	{
		pData[*nProgress] = nByte;
		(*nProgress)++;
		*bFinished = false;
		return !bEoT;
	}
	else
	{
		pData[*nProgress] = nByte;
		(*nProgress)++;
		*bFinished = true;
		//unsigned int nErrors = 0;
		// TODO do something with vpd
		xil_printf("\r\nMEZ3 Update Recevied\r\n");
		return bEoT;
	}
}

#endif
