// Low Level AVR System Monitor Driver

#ifndef AVRSYSMON2_H_
#define AVRSYSMON2_H_

#include <stdint.h>
#include <string.h>

#include "stdbool.h"

struct _SAVRSysMon_Context;

typedef struct _SAVRSysMon_Context* AVRSysMon_Context;

//enum _AVRSysMon_EReqType
//{
//	// Common mode
//	AVRSysMon_ReqNone = 0,
//	AVRSysMon_ReqVersion = 1,
//	// Application mode
//	AVRSysMon_ReqSensors32 = 2,
//	AVRSysMon_ReqSensors64 = 3,
//	AVRSysMon_ReqVPD = 4,
//	AVRSysMon_ReqXRM = 5,
//	AVRSysMon_ReqI2CWrite = 6,
//	AVRSysMon_ReqI2CRead = 7,
//	AVRSysMon_ReqI2CBus = 8,
//	AVRSysMon_ReqClobber = 9,
//	AVRSysMon_ReqLog32 = 10,
//	AVRSysMon_ReqLog64 = 11,
//	AVRSysMon_ReqLoopback = 12,
//	AVRSysMon_ReqGrace = 13,
//	// Service mode
//	AVRSysMon_ReqFlashWrite256 = 14,
//	AVRSysMon_ReqFlashRead256 = 15,
//	AVRSysMon_ReqFlashWrite512 = 16,
//	AVRSysMon_ReqFlashRead512 = 17,
//	// Special commands
//	AVRSysMon_Flush = 18,
//};
//
//typedef enum _AVRSysMon_EReqType AVRSysMon_ReqType;

enum _AVRSysMon_EResult
{
	AVRSysMon_ResultOK = 0,		// Everything is "bon"
	AVRSysMon_ResultFailed = 1,		// Something failed
	AVRSysMon_ResultTXNoData = 2,   // There is no data to TX
	AVRSysMon_ResultQueued = 3,     // The current request has been queued
	AVRSysMon_ResultActive = 4,     // The current request is being executed
	AVRSysMon_ResultTimeout = 5,    // I2C communication with the uController timed out.

	AVRSysMon_ResultOutstanding = 6,
	AVRSysMon_ResultModeError  = 7,
	AVRSysMon_ResultInvalidHandle = 8,
	AVRSysMon_ResultBusy = 9,
	AVRSysMon_ResultTXBuff = 10,
};

typedef enum _AVRSysMon_EResult AVRSysMon_Result;

enum _AVRSysMon_EMode
{
	AVRSysMon_ModeService = 0,
	AVRSysMon_ModeApplication = 1,
};

typedef enum _AVRSysMon_EMode AVRSysMon_Mode;

enum _AVRSysMon_EAlarm
{
	AVRSysMon_AlarmNone = 0,
	AVRSysMon_AlarmActive = 1,
};

typedef enum _AVRSysMon_EAlarm AVRSysMon_Alarm;

struct _AVRSysMon_SCommandReq
{
	//AVRSysMon_ReqType 		m_eType;
	const void* 			m_pTXData;
	unsigned int			m_nTXSize;
	const void* 			m_pRXData;
	unsigned int			m_nRXBuffSize;
	unsigned int			m_nRXSize;
	struct
	{
		struct _AVRSysMon_SCommandReq* m_pNextReq;
	} _private;
};

typedef struct _AVRSysMon_SCommandReq AVRSysMon_CommandReq;

struct _AVRSysMon_SCallbacks
{
	void(*m_pTXIRQEnableCallback)( AVRSysMon_Context* );
	void(*m_pTXIRQDisableCallback)( AVRSysMon_Context* );
	AVRSysMon_Mode(*m_pModeCheckCallback)( AVRSysMon_Context* );
	AVRSysMon_Alarm(*m_pAlarmCheckCallback)( AVRSysMon_Context* );
};

typedef struct _AVRSysMon_SCallbacks AVRSysMon_Callbacks;

//#ifdef __cplusplus
//extern "C"
//{
//#endif

// General functions
AVRSysMon_Result AVRSysMon_Open( uint8_t nIndex, AVRSysMon_Context* pContextOut, const AVRSysMon_Callbacks* pCallbacksIn );
AVRSysMon_Result AVRSysMon_Status( AVRSysMon_Context* pContext );
AVRSysMon_Result AVRSysMon_Close( AVRSysMon_Context* pContext );
AVRSysMon_Result AVRSysMon_GetMode( AVRSysMon_Context* pContext, AVRSysMon_Mode* eMode );

// Pumps and hooks (hardware layer)
AVRSysMon_Result AVRSysMon_PumpRX( AVRSysMon_Context* pContext, uint8_t nData, bool bSoH, bool bEoT );	    // Should be called for each byte of RX data received
AVRSysMon_Result AVRSysMon_PumpTX( AVRSysMon_Context* pContext, uint8_t* pData, bool* pSoH, bool* pEoT );	// Should be called periodically when TX FIFO is ready, to get next word to TX

// Requests and completions (Application or Service mode command requests)
AVRSysMon_Result AVRSysMon_Request(AVRSysMon_Context* pContext, AVRSysMon_CommandReq* pReq );  				// Schedules a command to execute (only one command can be executed at once)
AVRSysMon_Result AVRSysMon_CompleteReq( AVRSysMon_Context* pContext, AVRSysMon_CommandReq* pReq ); 			// Check on if Request is completed

// Application mode functions (Application mode only)
AVRSysMon_Result AVRSysMon_GetVPDData( AVRSysMon_Context* pContext, void* pData, size_t nMaxSize );
AVRSysMon_Result AVRSysMon_GetXRMData( AVRSysMon_Context* pContext, void* pData, size_t nMaxSize );
AVRSysMon_Result AVRSysMon_GetSensorData( AVRSysMon_Context* pContext, void* pData, size_t nMaxSize );
AVRSysMon_Result AVRSysMon_GetAlarm( AVRSysMon_Context* pContext, AVRSysMon_Alarm* pAlarm );

//#ifdef __cplusplus
//}
//#endif

#endif
