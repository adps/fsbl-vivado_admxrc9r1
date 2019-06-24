#include "resps.h"

#include "xil_printf.h"

#include "../avrsysmon2_configs.h"

//#define DEBUG_RESPS

#ifdef FULL_RESPS_USER_EXT

RESP_FUNC( Resp_I2CRead )
{
    static unsigned int nETC;
    static unsigned char chStatus;
    switch( *nProgress )
    {
    case 0:
        chStatus = nByte;
        (*nProgress)++;
        *bFinished = false;
        if( chStatus != 0 )
        {
            xil_printf("\r\nI2C Read Error\r\n");
        }
        return !bEoT;
    default:
        (*nProgress)++;
        *bFinished = bEoT;
        return true;
    //case 1:
    //    ((unsigned char*)&nETC)[0] = nByte;
    //    (*nProgress)++;
    //    *bFinished = false;
    //    return !bEoT;
    //case 2:
    //    ((unsigned char*)&nETC)[1] = nByte;
    //    (*nProgress)++;
    //    *bFinished = false;
    //    return !bEoT;
    //case 3:
    //    ((unsigned char*)&nETC)[2] = nByte;
    //    (*nProgress)++;
    //    *bFinished = false;
    //    return !bEoT;
    //case 4:
    //    ((unsigned char*)&nETC)[3] = nByte;
    //    (*nProgress)++;
    //    *bFinished = true;
    //    xil_printf("\r\nI2C Read Back ETC : " << std::dec << nETC << "\r\n";
    //    return bEoT;
    }
    //return false;
}

RESP_FUNC( Resp_I2CWrite )
{
    static unsigned char chStatus;
    switch( *nProgress )
    {
    case 0:
        chStatus = nByte;
        (*nProgress)++;
        *bFinished = true;
        if( chStatus != 0 )
        {
        	xil_printf("\r\nI2C Write Error\r\n");
        }
        else
        {
            //xil_printf("\r\nI2C Write OK" << "\r\n";
        }
        return bEoT;
    }
    return false;
}

RESP_FUNC( Resp_AbortProtection )
{
    if( *nProgress == 0 )
    {
    	xil_printf("\r\nAbort protection response: %u\r\n",(unsigned int)nByte);
        (*nProgress)++;
        *bFinished = true;
        return bEoT;
    }
    return false;
}

RESP_FUNC( Resp_InvokeProtection )
{
    if( *nProgress == 0 )
    {
    	xil_printf("\r\nInvoke protection response: %u\r\n",(unsigned int)nByte);
        (*nProgress)++;
        *bFinished = true;
        return bEoT;
    }
    return false;
}
#endif

RESP_FUNC( Resp_RequestVPDUpdate )
{
    if( *nProgress == 0 )
    {
    	xil_printf("\r\nRequest VPD Update response: %u\r\n",(unsigned int)nByte);
        (*nProgress)++;
        *bFinished = true;
        return bEoT;
    }
    return false;
}

#ifdef FULL_RESPS_USER_EXT
RESP_FUNC( Resp_RequestMezUpdate )
{
    if( *nProgress == 0 )
    {
    	xil_printf("\r\nRequest Mez Update response: %u\r\n",(unsigned int)nByte);
        (*nProgress)++;
        *bFinished = true;
        return bEoT;
    }
    return false;
}
#endif

//extern void PrintSensors( unsigned char* pPacket );

RESP_FUNC( Resp_RequestSensors )
{
    static unsigned char pData[64];
    static unsigned char chStatus;
    if( *nProgress == 0 )
    {
        chStatus = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    }
    else if( *nProgress < 64 )
    {
        pData[*nProgress-1] = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    }
    else
    {
        pData[*nProgress-1] = nByte;
        (*nProgress)++;
        *bFinished = true;
        //unsigned int nErrors = 0;
#ifdef DEBUG_RESPS
        xil_printf("Progress: %d\r\n", *nProgress);
        xil_printf("\r\nSensor Data RXed. Status: %u\r\n",(unsigned int)chStatus);
#endif

        //PrintSensors( pData );

        return bEoT;
    }
}

#ifdef FULL_RESPS_USER_EXT
RESP_FUNC( Resp_RequestBlockBox )
{
    static unsigned char pData[64];
    static unsigned char chStatus;
    if( *nProgress == 0 )
    {
        chStatus = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    }
    else if( *nProgress < 64 )
    {
        pData[*nProgress-1] = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    }
    else
    {
        pData[*nProgress-1] = nByte;
        (*nProgress)++;
        *bFinished = true;
		//unsigned int nErrors = 0;
#ifdef DEBUG_RESPS
        xil_printf("\r\nBlock Box Sensor Data RXed. Status: %u\r\n",(unsigned int)chStatus);
		if( chStatus == 0 )
		{
			//PrintSensors( pData );
		}
		xil_printf("\r\n");
#endif
		return bEoT;
    }
}

RESP_FUNC( Resp_ForceMezSupply )
{
    if( *nProgress == 0 )
    {
#ifdef DEBUG_RESPS
        xil_printf("\r\nRequest Force Mez Supply response: %u\r\n",(unsigned int)nByte);
#endif
        (*nProgress)++;
        *bFinished = true;
        return bEoT;
    }
    return false;
}
#endif

RESP_FUNC( Resp_RequestVPDData )
{
    static unsigned char pData[256];
    static unsigned char chStatus;
    if( *nProgress == 0 )
    {
        chStatus = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    }
    else if( *nProgress < 256 )
    {
        pData[*nProgress-1] = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    }
    else
    {
        pData[*nProgress-1] = nByte;
        (*nProgress)++;
        *bFinished = true;
        //unsigned int nErrors = 0;
#ifdef DEBUG_RESPS
        xil_printf("Progress: %d\r\n", *nProgress);
        xil_printf("\r\nVPD Data RXed. Status: %u\r\n",(unsigned int)chStatus);
#endif
        return bEoT;
    }
}

#ifdef FULL_RESPS_USER_EXT
RESP_FUNC( Resp_RequestMezData )
{
    static unsigned char pData[256];
    static unsigned char chStatus;
    if( *nProgress == 0 )
    {
        chStatus = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    }
    else if( *nProgress < 256 )
    {
        pData[*nProgress-1] = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    }
    else
    {
        pData[*nProgress-1] = nByte;
        (*nProgress)++;
        *bFinished = true;
        //unsigned int nErrors = 0;
#ifdef DEBUG_RESPS
        xil_printf("\r\nMez Data RXed. Status: %u\r\n",(unsigned int)chStatus);
#endif
        return bEoT;
    }
}

RESP_FUNC( Resp_SetFrequency )
{
    static unsigned char pData[4];
    static unsigned char chStatus;
    switch( *nProgress )
    {
    case 0:
        chStatus = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    case 1:
        pData[0] = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    case 2:
        pData[1] = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    case 3:
        pData[2] = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    case 4:
        pData[3] = nByte;
        (*nProgress)++;
#ifdef DEBUG_RESPS
        if( chStatus == 0 )
        {
            xil_printf("\r\nSet Frequency Resp: %u\r\n", *((unsigned int*)&pData[0]));
        }
        else
        {
            xil_printf("\r\nSet Frequency Resp Error: %u\r\n",(unsigned int)chStatus);
        }
#endif
        *bFinished = true;
        return bEoT;
    }
    return false;
}

RESP_FUNC( Resp_GetFrequency )
{
    static unsigned char pData[4];
    static unsigned char chStatus;
    switch( *nProgress )
    {
    case 0:
        chStatus = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    case 1:
        pData[0] = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    case 2:
        pData[1] = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    case 3:
        pData[2] = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    case 4:
        pData[3] = nByte;
        (*nProgress)++;
#ifdef DEBUG_RESPS
        if( chStatus == 0 )
        {
            xil_printf("\r\nGet Frequency Resp: %u\r\n", *((unsigned int*)&pData[0]));
        }
        else
        {
            xil_printf("\r\nGet Frequency Resp Error: %u\r\n",(unsigned int)chStatus);
        }
#endif
        *bFinished = true;
        return bEoT;
    }
    return false;
}

RESP_FUNC( Resp_OverrideSensor )
{
    if( *nProgress == 0 )
    {
#ifdef DEBUG_RESPS
        xil_printf("\r\nRequest Sensor Value response: %u\r\n",(unsigned int)nByte);
#endif
        (*nProgress)++;
        *bFinished = true;
        return bEoT;
    }
    return false;
}



RESP_FUNC( Resp_SetFrequencyEx )
{
	static unsigned char pData[4];
	static unsigned char chStatus;
	switch( *nProgress )
	{
	case 0:
		chStatus = nByte;
		(*nProgress)++;
		*bFinished = false;
		return !bEoT;
	case 1:
		pData[0] = nByte;
		(*nProgress)++;
		*bFinished = false;
		return !bEoT;
	case 2:
		pData[1] = nByte;
		(*nProgress)++;
		*bFinished = false;
		return !bEoT;
	case 3:
		pData[2] = nByte;
		(*nProgress)++;
		*bFinished = false;
		return !bEoT;
	case 4:
		pData[3] = nByte;
		(*nProgress)++;
#ifdef DEBUG_RESPS
		if( chStatus == 0 )
		{
			xil_printf("\r\nSet FrequencyEx Resp: %u\r\n", *((unsigned int*)&pData[0]));
		}
		else
		{
			xil_printf("\r\nSet FrequencyEx Resp Error: %u\r\n",(unsigned int)chStatus);
		}
#endif
		*bFinished = true;
		return bEoT;
	}
	return false;
}

RESP_FUNC( Resp_GetFrequencyEx )
{
	static unsigned char pData[4];
	static unsigned char chStatus;
	switch( *nProgress )
	{
	case 0:
		chStatus = nByte;
		(*nProgress)++;
		*bFinished = false;
		return !bEoT;
	case 1:
		pData[0] = nByte;
		(*nProgress)++;
		*bFinished = false;
		return !bEoT;
	case 2:
		pData[1] = nByte;
		(*nProgress)++;
		*bFinished = false;
		return !bEoT;
	case 3:
		pData[2] = nByte;
		(*nProgress)++;
		*bFinished = false;
		return !bEoT;
	case 4:
		pData[3] = nByte;
		(*nProgress)++;
#ifdef DEBUG_RESPS
		if( chStatus == 0 )
		{
			xil_printf("\r\nGet FrequencyEx Resp: %u\r\n", *((unsigned int*)&pData[0]));
		}
		else
		{
			xil_printf("\r\nGet FrequencyEx Resp Error: %u\r\n",(unsigned int)chStatus);
		}
#endif
		*bFinished = true;
		return bEoT;
	}
	return false;
}
#endif
