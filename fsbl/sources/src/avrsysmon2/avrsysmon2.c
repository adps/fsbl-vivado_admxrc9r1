#include "avrsysmon2.h"
#include "cmds.h"
#include "resps.h"
#include "xil_printf.h"

#ifndef __cplusplus
#ifndef _MSC_VER
#	include <stdbool.h>
#else
#	define bool		int
#	define true		(1)
#	define false	(0)
#endif
#endif

#include "cpp_types.h"

#include "../avrsysmon2_configs.h"

#ifdef DISABLE_OPTIMIZATION
#pragma GCC optimize ("O0")
#endif

//enum _AVRSysMon_Responce
//{
//	// Auto Commands
//	AVRSysMon_RespVPD,
//	AVRSysMon_RespXRM,
//	AVRSysMon_RespSensors32,
//	AVRSysMon_RespSensors64,
//	// User Commands
//	AVRSysMon_RespVN,
//	AVRSysMon_RespST,
//	AVRSysMon_RespSX,
//	AVRSysMon_RespLG,
//	AVRSysMon_RespLX,
//	AVRSysMon_RespIR,
//	AVRSysMon_RespIW,
//	// Special commands
//	AVRSysMon_RespX,
//	AVRSysMon_RespXX,
//	//
//	AVRSysMon_RespNone,
//};
//
//typedef enum _AVRSysMon_Responce AVRSysMon_Responce;

struct _SAVRSysMon_Context
{
    uint16_t                    m_nRespProgress;
	bool 						m_bInUse;
	uint8_t 					m_pXRMData[256];
	bool 						m_bXRMDataValid;
	uint8_t 					m_pVPDData[256];
	bool 						m_bVPDDataValid;
	uint8_t 					m_pSensorData[64];
	char	 					m_pRXCmd[4];
    unsigned char               m_pHeader[2];
    bool                        m_bLastSoH;
    bool                        m_bInFrame;
	bool 						m_bSensorDataValid;
	AVRSysMon_CommandReq*		m_pCurrentRequest;
	//AVRSysMon_Responce 			m_eCurrentResponce;
	//AVRSysMon_ReqType 			m_eCurrentResponceReq;
	unsigned int				m_nRXExpectedBytes;
	unsigned int				m_nRXTotalBytes;
	unsigned int				m_nTXSentBytes;
	unsigned int				m_nTXTotalBytes;
	//const unsigned char*		m_pTXHeader;
	//unsigned int				m_nTXHeaderSize;
	bool						m_bRequestFinished;
	void(*m_pTXIRQEnableCallback)( AVRSysMon_Context* );
	void(*m_pTXIRQDisableCallback)( AVRSysMon_Context* );
	AVRSysMon_Mode(*m_pModeCheckCallback)( AVRSysMon_Context* );
	AVRSysMon_Alarm(*m_pAlarmCheckCallback)( AVRSysMon_Context* );
};

// The one and only sysmon struct... do we want more....?
#ifndef _AVRSYSMON_INSTNACES
#define _AVRSYSMON_INSTNACES 1
#endif

//struct _SAVRSysMon_Context g_pAVRSysMonState[_AVRSYSMON_INSTNACES] = { { .m_bInUse = false } };
struct _SAVRSysMon_Context g_pAVRSysMonState[_AVRSYSMON_INSTNACES] = { 0 };

#define CHECK_HANDLE_VALID( x )

//\
//{ \
//	uintptr_t _n = &g_pAVRSysMonState[0] - (*x); \
//	if( _n % sizeof( struct _SAVRSysMon_Context ) != 0 ) \
//	{ \
//		return AVRSysMon_ResultInvalidHandle; \
//	} \
//	_n /= sizeof( struct _SAVRSysMon_Context ); \
//	if( _n >= _AVRSYSMON_INSTNACES ) \
//	{ \
//		return AVRSysMon_ResultInvalidHandle; \
//	} \
//	if( !(*x)->m_bInUse ) \
//	{ \
//		return AVRSysMon_ResultInvalidHandle; \
//	} \
//}

AVRSysMon_Result AVRSysMon_Open( uint8_t nIndex, AVRSysMon_Context* pContextOut, const AVRSysMon_Callbacks* pCallbacksIn )
{
	if( nIndex >= _AVRSYSMON_INSTNACES )
	{
		return AVRSysMon_ResultFailed;
	}
	if( g_pAVRSysMonState[nIndex].m_bInUse )
	{
		return AVRSysMon_ResultBusy;
	}
	else
	{
		g_pAVRSysMonState[nIndex].m_bInUse = true;
		*pContextOut = &g_pAVRSysMonState[nIndex];
		(*pContextOut)->m_pCurrentRequest = NULL;
		(*pContextOut)->m_bRequestFinished = true;
        (*pContextOut)->m_nRespProgress = 0;
        (*pContextOut)->m_bLastSoH = false;
        (*pContextOut)->m_bInFrame = false;
		//(*pContextOut)->m_eCurrentResponce = AVRSysMon_RespNone;
		(*pContextOut)->m_pTXIRQEnableCallback = pCallbacksIn->m_pTXIRQEnableCallback;
		(*pContextOut)->m_pTXIRQDisableCallback = pCallbacksIn->m_pTXIRQDisableCallback;
		(*pContextOut)->m_pModeCheckCallback = pCallbacksIn->m_pModeCheckCallback;
		(*pContextOut)->m_pAlarmCheckCallback = pCallbacksIn->m_pAlarmCheckCallback;
		(*pContextOut)->m_pRXCmd[3] = '\0';
		return AVRSysMon_ResultOK;
	}
}

AVRSysMon_Result AVRSysMon_Status( AVRSysMon_Context* pContext )
{
	CHECK_HANDLE_VALID( pContext );
	if( (*pContext)->m_pCurrentRequest != 0 )
	{
		return AVRSysMon_ResultOutstanding;
	}
	return AVRSysMon_ResultOK;
}


AVRSysMon_Result AVRSysMon_Close( AVRSysMon_Context* pContext )
{
	CHECK_HANDLE_VALID( pContext );
	if( (*pContext)->m_pCurrentRequest != 0 )
	{
		return AVRSysMon_ResultOutstanding;
	}
	(*pContext)->m_bInUse = false;
	*pContext = 0;
	return AVRSysMon_ResultOK;
}

AVRSysMon_Result AVRSysMon_GetMode( AVRSysMon_Context* pContext, AVRSysMon_Mode* pMode )
{
	CHECK_HANDLE_VALID( pContext );
	if( ( (*pContext)->m_pModeCheckCallback == 0 ) || ( pMode == NULL ) )
	{
		return AVRSysMon_ResultFailed;
	}
	*pMode = (*pContext)->m_pModeCheckCallback( pContext );
	return AVRSysMon_ResultOK;
}

//struct _AVRSysMon_Command
//{
//	const unsigned char m_szCmd[3];
//	const unsigned int m_nHeaderSize;
//	const unsigned int m_nSize;
////	const AVRSysMon_ReqType m_eReq;
//	//const AVRSysMon_Responce m_eResp;
//};

//static struct _AVRSysMon_Command g_cmdRXCmds[] =
//{
//		// Auto Commands
//        { { 0x03, 0x03 }, 2U, 8U, AVRSysMon_ReqVersion, AVRSysMon_RespVN },
//        //{ { 0x3, 0x3 }, 256U, AVRSysMon_ReqVPD },
//		//{ "XD=", 256U, AVRSysMon_ReqXRM },
//		//{ "AS=", 32U, AVRSysMon_ReqNone },
//		//{ "XS=", 64U, AVRSysMon_ReqNone },
//		//// User Commands
//		//{ "\x1\x3\x3", 8U, AVRSysMon_ReqVersion },
//		//{ "ST=", 32U, AVRSysMon_ReqSensors32 },
//		//{ "SX=", 64U, AVRSysMon_ReqSensors64 },
//		//{ "LG=", 1024U, AVRSysMon_ReqLog32 },
//		//{ "LX=", 2048U, AVRSysMon_ReqLog64 },
//		//{ "IR=", 11U, AVRSysMon_ReqI2CRead },
//		//{ "IW!", 11U, AVRSysMon_ReqI2CWrite },
//		//// Special commands
//		//{ "X", 1U, AVRSysMon_ReqNone },
//		//{ "!", 1U, AVRSysMon_ReqNone },
//		//// End of list
//		{ { 0x0, 0x0 }, 0U, 0, AVRSysMon_ReqNone, AVRSysMon_RespNone },
//};

//AVRSysMon_Responce CheckResponceType( const char* pCmd )
//{
//	unsigned int n = 0;
//	while( g_cmdRXCmds[n].m_nHeaderSize != 0 )
//	{
//        for( unsigned int k = 0; k < g_cmdRXCmds[n].m_nHeaderSize; k++ )
//        {
//            if( g_cmdRXCmds[n].m_szCmd[k] == pCmd[k] )
//            {
//                if( ( k + 1 ) == g_cmdRXCmds[n].m_nHeaderSize )
//                {
//                    return (AVRSysMon_Responce)g_cmdRXCmds[n].m_eResp;
//                }
//            }
//        }
//		n++;
//	}
//	return AVRSysMon_RespNone;
//}
//
//
//AVRSysMon_ReqType CheckResponceTypeReq( const char* pCmd )
//{
//	unsigned int n = 0;
//	while( g_cmdRXCmds[n].m_nHeaderSize != 0 )
//	{
//        for( unsigned int k = 0; k < g_cmdRXCmds[n].m_nHeaderSize; k++ )
//        {
//            if( g_cmdRXCmds[n].m_szCmd[k] == pCmd[k] )
//            {
//                if( ( k + 1 ) == g_cmdRXCmds[n].m_nHeaderSize )
//                {
//                    return g_cmdRXCmds[n].m_eReq;
//                }
//            }
//        }
//		n++;
//	}
//	return AVRSysMon_ReqNone;
//}
//
//unsigned int CheckResponceTypeSize( const char* pCmd )
//{
//	unsigned int n = 0;
//	while( g_cmdRXCmds[n].m_nHeaderSize != 0 )
//	{
//        for( unsigned int k = 0; k < g_cmdRXCmds[n].m_nHeaderSize; k++ )
//        {
//            if( g_cmdRXCmds[n].m_szCmd[k] == pCmd[k] )
//            {
//                if( ( k + 1 ) == g_cmdRXCmds[n].m_nHeaderSize )
//                {
//                    return g_cmdRXCmds[n].m_nSize;
//                }
//            }
//        }
//		n++;
//	}
//	return 0;
//}


static bool (*m_fptActive)( unsigned short*, unsigned char, bool, bool* );

// Pumps and hooks ( Can be called from a separate context from other functions)
AVRSysMon_Result AVRSysMon_PumpRX( AVRSysMon_Context* pContext, uint8_t nData, bool bSoH, bool bEoT )
{
	CHECK_HANDLE_VALID( pContext );

    if( (*pContext)->m_bInFrame && bSoH )
    {
        xil_printf("SoH Seen inside frame!\r\n");
        return AVRSysMon_ResultOK;
    }

    if( !((*pContext)->m_bInFrame || bSoH ) )
    {
        xil_printf("Discarding data outside frame.\r\n");
        return AVRSysMon_ResultOK;
    }

    (*pContext)->m_bInFrame = ( (*pContext)->m_bInFrame | bSoH ) & !bEoT;


    if( bSoH )
    {
        if( (*pContext)->m_nRespProgress != 0 )
        {
            xil_printf("Error progress was not zero when SoH detected.\r\n");
        }
        else
        {
        }
    }

    if( ( m_fptActive == nullptr ) && (*pContext)->m_bLastSoH )
    {
    	unsigned int n = 0;
        for( n = 0; g_pCommandResponce[n].m_fpt != nullptr; n++ )
        {
            if( ( g_pCommandResponce[n].m_pHeader[0] == (*pContext)->m_pHeader[0] )
                && ( nData == g_pCommandResponce[n].m_pHeader[1] ) )
            {
				//std:cout << "\r\nStarting to process packet " << std::hex << (unsigned int)(*pContext)->m_pHeader[0] << "_" << (unsigned int)nData;
                m_fptActive = g_pCommandResponce[n].m_fpt;
                (*pContext)->m_nRespProgress = 0;
				(*pContext)->m_pHeader[1] = nData;
				//(*pContext)->m_bLastSoH = false;// bSoH;
				if( ( (*pContext)->m_pCurrentRequest != nullptr )
					&& ( ( ( (unsigned char*)(*pContext)->m_pCurrentRequest->m_pTXData)[0] ) == (*pContext)->m_pHeader[0] )
					&& ( ( ( (unsigned char*)(*pContext)->m_pCurrentRequest->m_pTXData)[1] & 0x7FU )  == (*pContext)->m_pHeader[1] ) )
				{
					if( (*pContext)->m_pCurrentRequest->m_nRXSize < (*pContext)->m_pCurrentRequest->m_nRXBuffSize )
					{
						((uint8_t*)(*pContext)->m_pCurrentRequest->m_pRXData)[(*pContext)->m_pCurrentRequest->m_nRXSize++] = (*pContext)->m_pHeader[0];
					}
					if( (*pContext)->m_pCurrentRequest->m_nRXSize < (*pContext)->m_pCurrentRequest->m_nRXBuffSize )
					{
						((uint8_t*)(*pContext)->m_pCurrentRequest->m_pRXData)[(*pContext)->m_pCurrentRequest->m_nRXSize++] = nData;
					}
				}

				if( bEoT ) // Commands with no data!
				{
					bool bFinished = false;
					bool bCmdOK = m_fptActive( (*pContext)->m_nRespProgress, nData, bEoT, &bFinished );
					if( bCmdOK )
					{
						if( bFinished )
						{
							m_fptActive = nullptr;
							(*pContext)->m_nRespProgress = 0;
							if( ((*pContext)->m_pCurrentRequest != nullptr)
								&& ((((unsigned char*)(*pContext)->m_pCurrentRequest->m_pTXData)[0]) == (*pContext)->m_pHeader[0])
								&& ((((unsigned char*)(*pContext)->m_pCurrentRequest->m_pTXData)[1] & 0x7FU) == (*pContext)->m_pHeader[1]) )
							{
								(*pContext)->m_bRequestFinished = true;
							}
							// TODO anything with something waiting for the command to complete
						}
					}
					else
					{
						// Abort processing
						m_fptActive = nullptr;
						(*pContext)->m_nRespProgress = 0;
						if( ((*pContext)->m_pCurrentRequest != nullptr)
							&& ((((unsigned char*)(*pContext)->m_pCurrentRequest->m_pTXData)[0]) == (*pContext)->m_pHeader[0])
							&& ((((unsigned char*)(*pContext)->m_pCurrentRequest->m_pTXData)[1] & 0x7FU) == (*pContext)->m_pHeader[1]) )
						{
							(*pContext)->m_bRequestFinished = true;
						}
						//xil_printf("\r\nError Processing incomming packet\r\n)";
					}
				}
				(*pContext)->m_bLastSoH = bSoH;
				return AVRSysMon_ResultOK;
				//break;
            }
        }
    }

    if( m_fptActive != nullptr )
    {
        bool bFinished = false;
		if( ( (*pContext)->m_pCurrentRequest != nullptr )
			&& ((((unsigned char*)(*pContext)->m_pCurrentRequest->m_pTXData)[0]) == (*pContext)->m_pHeader[0])
			&& ((((unsigned char*)(*pContext)->m_pCurrentRequest->m_pTXData)[1] & 0x7FU) == (*pContext)->m_pHeader[1]) )
		{
			if( (*pContext)->m_pCurrentRequest->m_nRXSize < (*pContext)->m_pCurrentRequest->m_nRXBuffSize )
			{
				((uint8_t*)(*pContext)->m_pCurrentRequest->m_pRXData)[(*pContext)->m_pCurrentRequest->m_nRXSize++] = nData;
			}
		}
        bool bCmdOK = m_fptActive( &((*pContext)->m_nRespProgress), nData, bEoT, &bFinished );
        if( bCmdOK )
        {
            if( bFinished )
            {
                m_fptActive = nullptr;
                (*pContext)->m_nRespProgress = 0;
				if( ((*pContext)->m_pCurrentRequest != nullptr)
					&& ((((unsigned char*)(*pContext)->m_pCurrentRequest->m_pTXData)[0]) == (*pContext)->m_pHeader[0])
					&& ((((unsigned char*)(*pContext)->m_pCurrentRequest->m_pTXData)[1] & 0x7FU) == (*pContext)->m_pHeader[1]) )
				{
					(*pContext)->m_bRequestFinished = true;
				}
                // TODO anything with something waiting for the command to complete
            }
        }
        else
        {
            // Abort processing
            m_fptActive = nullptr;
            (*pContext)->m_nRespProgress = 0;
			if( ((*pContext)->m_pCurrentRequest != nullptr)
				&& ((((unsigned char*)(*pContext)->m_pCurrentRequest->m_pTXData)[0]) == (*pContext)->m_pHeader[0])
				&& ((((unsigned char*)(*pContext)->m_pCurrentRequest->m_pTXData)[1] & 0x7FU) == (*pContext)->m_pHeader[1]) )
			{
				(*pContext)->m_bRequestFinished = true;
			}
			//xil_printf("\r\nError Processing incomming packet\r\n");
        }
    }
	else
	{
		(*pContext)->m_pHeader[0] = nData;
		(*pContext)->m_bLastSoH = bSoH;
	}

	return AVRSysMon_ResultOK;


	//if( (*pContext)->m_eCurrentResponce != AVRSysMon_RespNone )
	//{
    //    //std::cout << "\r\nSwitch " << std::dec << (*pContext)->m_eCurrentResponce << "\r\n";
	//	switch( (*pContext)->m_eCurrentResponce )
	//	{
	//	case AVRSysMon_RespVPD: (*pContext)->m_pVPDData[256-(*pContext)->m_nRXExpectedBytes] = nData; break;
	//	case AVRSysMon_RespXRM: (*pContext)->m_pXRMData[256-(*pContext)->m_nRXExpectedBytes] = nData; break;
	//	case AVRSysMon_RespSensors32: (*pContext)->m_pSensorData[32-(*pContext)->m_nRXExpectedBytes] = nData; break;
	//	case AVRSysMon_RespSensors64: (*pContext)->m_pSensorData[64-(*pContext)->m_nRXExpectedBytes] = nData; break;
	//	default:
    //        //xil_printf("\r\nDefault\r\n)";
	//		if( !(*pContext)->m_bRequestFinished && (*pContext)->m_pCurrentRequest->m_eType == (*pContext)->m_eCurrentResponceReq )
	//		{
	//			unsigned int nOutIndex = (*pContext)->m_nRXTotalBytes - (*pContext)->m_nRXExpectedBytes;
	//			if( ( (*pContext)->m_pCurrentRequest->m_pOutput == NULL ) || ( (*pContext)->m_pCurrentRequest->m_nOutputSize <= nOutIndex ) )
	//			{
    //                //xil_printf("\r\nHmmmmm\r\n)";
	//				return AVRSysMon_ResultTXBuff;
	//			}
    //            //std::cout << "\r\nProcessing output: " << std::hex << "0x" << (unsigned int)nData << "\r\n";
	//			((uint8_t*)(*pContext)->m_pCurrentRequest->m_pOutput)[nOutIndex] = nData;
	//		}
	//	}
	//	(*pContext)->m_nRXExpectedBytes--;
    //    //std::cout << "\r\nExpected bytes remaining: " << std::dec << (unsigned int)(*pContext)->m_nRXExpectedBytes << "\r\n";
	//	if( (*pContext)->m_nRXExpectedBytes == 0 )
	//	{
	//		// This might be a command completion
	//		switch( (*pContext)->m_eCurrentResponce )
	//		{
	//		case AVRSysMon_RespVPD: (*pContext)->m_bVPDDataValid = true; break;
	//		case AVRSysMon_RespXRM: (*pContext)->m_bXRMDataValid = true; break;
	//		case AVRSysMon_RespSensors32: (*pContext)->m_bSensorDataValid = true; break;
	//		case AVRSysMon_RespSensors64: (*pContext)->m_bSensorDataValid = true; break;
	//		default:
	//			break;
	//		}
	//		if( (*pContext)->m_pCurrentRequest->m_eType == (*pContext)->m_eCurrentResponceReq )
	//		{
	//			// Finish the command ...
	//			(*pContext)->m_bRequestFinished = true;
	//		}
	//		(*pContext)->m_eCurrentResponce	= AVRSysMon_RespNone;
	//		(*pContext)->m_eCurrentResponceReq	= AVRSysMon_ReqNone;
	//		(*pContext)->m_pRXCmd[0] = '\0';
	//		(*pContext)->m_pRXCmd[1] = '\0';
	//		(*pContext)->m_pRXCmd[2] = '\0';
	//	}
	//}
	//else
	//{
	//	(*pContext)->m_pRXCmd[0] = (*pContext)->m_pRXCmd[1];
	//	(*pContext)->m_pRXCmd[1] = (*pContext)->m_pRXCmd[2];
	//	(*pContext)->m_pRXCmd[2] = nData;
	//	(*pContext)->m_eCurrentResponce = CheckResponceType( (*pContext)->m_pRXCmd );
	//	(*pContext)->m_eCurrentResponceReq = CheckResponceTypeReq( (*pContext)->m_pRXCmd );
	//	(*pContext)->m_nRXExpectedBytes = CheckResponceTypeSize( (*pContext)->m_pRXCmd );
	//	(*pContext)->m_nRXTotalBytes = (*pContext)->m_nRXExpectedBytes;
	//}
}



//struct _AVRSysMon_TXCommand
//{
//	const unsigned char m_szCmd[4];
//	const unsigned int m_nTXHeaderSize;
//	const unsigned int m_nSize;
//	//const AVRSysMon_ReqType m_eReq;
//};

//static struct _AVRSysMon_TXCommand g_cmdTXCmds[] =
//{
//		// Common Commands
//		// App mode Commands
//        { { 0x03, 0x83, 0x0, 0x0 }, 2U, 0U, AVRSysMon_ReqVersion },
//		//{ "ST?", 3U, 0U, AVRSysMon_ReqSensors32 },
//		//{ "SX?", 3U, 0U, AVRSysMon_ReqSensors64 },
//		//{ "LG?", 3U, 0U, AVRSysMon_ReqLog32 },
//		//{ "LX?", 3U, 0U, AVRSysMon_ReqLog64 },
//		//{ "IR?", 3U, 0U, AVRSysMon_ReqI2CRead },
//		//{ "IW?", 3U, 0U, AVRSysMon_ReqI2CWrite },
//		//{ "IS!", 3U, 0U, AVRSysMon_ReqI2CBus },
//		//{ "LB?", 3U, 0U, AVRSysMon_ReqLoopback },
//		//{ "KP!", 3U, 0U, AVRSysMon_ReqGrace },
//		//{ "SD!", 3U, 0U, AVRSysMon_ReqClobber },
//		//{ "VD!", 3U, 0U, AVRSysMon_ReqVPD },
//		//{ "MD!", 3U, 0U, AVRSysMon_ReqXRM },
//		//// Service mode commands
//		//{ "W", 1U, 256U + 2U, AVRSysMon_ReqFlashWrite256 }, // + 2
//		//{ "w", 1U, 512U + 4U, AVRSysMon_ReqFlashWrite512 }, // + 4
//		//{ "R", 1U, 256U + 2U, AVRSysMon_ReqFlashRead256 }, // + 2
//		//{ "r", 1U, 512U + 4U, AVRSysMon_ReqFlashRead512 }, // + 4
//		//// Flush command
//		//{ "", 0U, 512U, AVRSysMon_ReqXRM },
//		//// End of list
//		{ { 0x0, 0x0, 0x0, 0x0 }, 0, 0, AVRSysMon_ReqNone },
//};
//
//unsigned int GetReqPayloadSize( AVRSysMon_ReqType eReq )
//{
//	unsigned int n = 0;
//	while( g_cmdTXCmds[n].m_nTXHeaderSize != 0 )
//	{
//		if( eReq == g_cmdTXCmds[n].m_eReq )
//		{
//			return g_cmdTXCmds[n].m_nSize;
//		}
//		n++;
//	}
//	return 0U;
//}
//
//
//const unsigned char* GetReqHeader( AVRSysMon_ReqType eReq )
//{
//	unsigned int n = 0;
//	while( g_cmdTXCmds[n].m_nTXHeaderSize != 0 )
//	{
//		if( eReq == g_cmdTXCmds[n].m_eReq )
//		{
//			return g_cmdTXCmds[n].m_szCmd;
//		}
//		n++;
//	}
//	return 0U;
//}
//
//unsigned int GetReqHeaderSize( AVRSysMon_ReqType eReq )
//{
//	unsigned int n = 0;
//	while( g_cmdTXCmds[n].m_nTXHeaderSize != 0 )
//	{
//		if( eReq == g_cmdTXCmds[n].m_eReq )
//		{
//			return g_cmdTXCmds[n].m_nTXHeaderSize;
//		}
//		n++;
//	}
//	return 0U;
//}

AVRSysMon_Result AVRSysMon_PumpTX( AVRSysMon_Context* pContext, uint8_t* pData, bool* pSoH, bool* pEoT )
{
	CHECK_HANDLE_VALID( pContext );
	if( (*pContext)->m_pCurrentRequest == NULL )
	{
		return AVRSysMon_ResultTXNoData;
	}
	else
	{
        if( (*pContext)->m_nTXSentBytes < (*pContext)->m_nTXTotalBytes )
		{
			//if( (*pContext)->m_nTXSentBytes >= (*pContext)->m_nTXHeaderSize )
			{
				unsigned int nIndex = (*pContext)->m_nTXSentBytes;// - (*pContext)->m_nTXHeaderSize;
				if( nIndex >= (*pContext)->m_pCurrentRequest->m_nTXSize )
				{
					return AVRSysMon_ResultTXBuff;
				}
				else
				{
					*pData = ((uint8_t*)(*pContext)->m_pCurrentRequest->m_pTXData)[nIndex];
                    *pSoH = ( (*pContext)->m_nTXSentBytes == 0 );
                    *pEoT = ( ( (*pContext)->m_nTXSentBytes + 1 ) == (*pContext)->m_nTXTotalBytes );
				}
			}
			//else
			//{
			//	*pData = ((uint8_t*)(*pContext)->m_pTXHeader)[(*pContext)->m_nTXSentBytes];
            //    *pSoH = ( (*pContext)->m_nTXSentBytes == 0 );
            //    *pEoT = ( ( (*pContext)->m_nTXSentBytes + 1 ) == (*pContext)->m_nTXTotalBytes );
			//}
			(*pContext)->m_nTXSentBytes++;
			return AVRSysMon_ResultOK;
		}
		else
		{
            //(*pContext)->m_bRequestFinished = true; // Fire and forget for now....
			return AVRSysMon_ResultTXNoData;
		}
	}
}

// Requests and completions (Application mode only)
AVRSysMon_Result AVRSysMon_Request(AVRSysMon_Context* pContext, AVRSysMon_CommandReq* pReq )
{
	CHECK_HANDLE_VALID( pContext );
	pReq->_private.m_pNextReq = NULL;
    pReq->m_nRXSize = 0;
    (*pContext)->m_nRespProgress = 0;
	if( (*pContext)->m_pCurrentRequest != NULL )
	{
		(*pContext)->m_pCurrentRequest->_private.m_pNextReq = pReq;
		return AVRSysMon_ResultQueued;
	}
	else
	{
		(*pContext)->m_pCurrentRequest = pReq;
		//(*pContext)->m_nTXHeaderSize = GetReqHeaderSize( pReq->m_eType );
		//(*pContext)->m_pTXHeader = GetReqHeader( pReq->m_eType );
		(*pContext)->m_nTXSentBytes = 0;
		(*pContext)->m_nTXTotalBytes = pReq->m_nTXSize;//(*pContext)->m_nTXHeaderSize + GetReqPayloadSize( pReq->m_eType );
		(*pContext)->m_bRequestFinished = false;
		if( (*pContext)->m_pTXIRQEnableCallback != NULL )
		{
			(*pContext)->m_pTXIRQEnableCallback( pContext );
		}
		return AVRSysMon_ResultOK;
	}
}

AVRSysMon_Result AVRSysMon_CompleteReq( AVRSysMon_Context* pContext, AVRSysMon_CommandReq* pReq )
{
	AVRSysMon_CommandReq* pHead;
	CHECK_HANDLE_VALID( pContext );
	if( (*pContext)->m_pCurrentRequest == pReq )
	{
		if( (*pContext)->m_bRequestFinished )
		{
			(*pContext)->m_pCurrentRequest = (*pContext)->m_pCurrentRequest->_private.m_pNextReq;
			if( (*pContext)->m_pCurrentRequest != NULL )
			{
				//(*pContext)->m_nTXHeaderSize = GetReqHeaderSize( (*pContext)->m_pCurrentRequest->m_eType );
				//(*pContext)->m_pTXHeader = GetReqHeader( (*pContext)->m_pCurrentRequest->m_eType );
				(*pContext)->m_nTXSentBytes = 0;
				(*pContext)->m_nTXTotalBytes = pReq->m_nTXSize;// (*pContext)->m_nTXHeaderSize + GetReqPayloadSize( (*pContext)->m_pCurrentRequest->m_eType );
				(*pContext)->m_bRequestFinished = false;
			}
			else
			{
				if( (*pContext)->m_pTXIRQDisableCallback != NULL )
				{
					(*pContext)->m_pTXIRQDisableCallback( pContext );
				}
			}
			return AVRSysMon_ResultOK;
		}
		else
		{
			return AVRSysMon_ResultActive;
		}
	}
	pHead = (*pContext)->m_pCurrentRequest;
	while( pHead != NULL )
	{
		if( pHead->_private.m_pNextReq == pReq )
		{
			return AVRSysMon_ResultQueued;
		}
		else
		{
			pHead = pHead->_private.m_pNextReq;
		}
	}
	return AVRSysMon_ResultFailed;
}

// Application mode functions
AVRSysMon_Result AVRSysMon_GetVPDData( AVRSysMon_Context* pContext, void* pData, size_t nMaxSize )
{
	CHECK_HANDLE_VALID( pContext );
	//if( (*pContext)->m_bVPDDataValid == 1 )
	{
		size_t nSize = ( nMaxSize < 256U ) ? nMaxSize : 256U;
		int i=0;
		for(i=0; i<32; i++)
			xil_printf("[%d]: 0x%02x\n", i, (*pContext)->m_pVPDData[i]);
		memcpy( pData, (*pContext)->m_pVPDData, nSize );
		return AVRSysMon_ResultOK;
	}
	return AVRSysMon_ResultFailed;
}

AVRSysMon_Result AVRSysMon_GetXRMData( AVRSysMon_Context* pContext, void* pData, size_t nMaxSize )
{
	CHECK_HANDLE_VALID( pContext );
	if( (*pContext)->m_bXRMDataValid == 1 )
	{
		size_t nSize = ( nMaxSize < 256U ) ? nMaxSize : 256U;
		memcpy( pData, (*pContext)->m_pXRMData, nSize );
		return AVRSysMon_ResultOK;
	}
	return AVRSysMon_ResultFailed;
}

AVRSysMon_Result AVRSysMon_GetSensorData( AVRSysMon_Context* pContext, void* pData, size_t nMaxSize )
{
	CHECK_HANDLE_VALID( pContext );
	if( (*pContext)->m_bSensorDataValid == 1 )
	{
		size_t nSize = ( nMaxSize < 64U ) ? nMaxSize : 64U;
		memcpy( pData, (*pContext)->m_pSensorData, nSize );
		return AVRSysMon_ResultOK;
	}
	return AVRSysMon_ResultFailed;
}

AVRSysMon_Result AVRSysMon_GetAlarm( AVRSysMon_Context* pContext, AVRSysMon_Alarm* pAlarm )
{
	CHECK_HANDLE_VALID( pContext );
	if( ( (*pContext)->m_pAlarmCheckCallback == 0 ) || ( pAlarm == NULL ) )
	{
		return AVRSysMon_ResultFailed;
	}
	*pAlarm = (*pContext)->m_pAlarmCheckCallback( pContext );
	return AVRSysMon_ResultOK;
}
