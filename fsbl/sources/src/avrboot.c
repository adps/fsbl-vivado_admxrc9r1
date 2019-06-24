


//#include <unistd.h>
#include "avrsysmon2/avrsysmon2.h"
#include "xfsbl_hw.h"
#include "xfsbl_debug.h"
//#include "qspi.h"
//#include "nand.h"
//#include "nor.h"
//#include "sd.h"
//#include "pcap.h"
//#include "image_mover.h"
#include "xparameters.h"
#include "xil_cache.h"
#include "xil_exception.h"
#include "xstatus.h"
//#include "fsbl_hooks.h"
#include "xtime_l.h"
//#include "xuartps_hw.h"
#include "xuartps.h"
#include "avrsysmon_admxrc9r1.h"
#include "avrboot.h"
#include "avrsysmon2/avrtalk2appmode.h"
#include "avrsysmon2/avrtalk2servicemode.h"
//#include "adps_ps7.h"
#include <math.h>
#ifndef _MSC_VER
#	include <stdbool.h>
#else
#	define bool		int
#	define true		(1)
#	define false	(0)
#endif
#ifdef _DEBUG
#include "mem_test.h"
#endif

#ifdef AVRBOOT

#include "avrsysmon2_configs.h"

#ifdef DISABLE_OPTIMIZATION
#pragma GCC optimize ("O0")
#endif

#define XFSBL_JTAG_BOOT_MODE		(0x0U)
#define XFSBL_QSPI24_BOOT_MODE		(0x1U)
#define XFSBL_QSPI32_BOOT_MODE		(0x2U)
#define XFSBL_SD0_BOOT_MODE	        (0x3U)
#define XFSBL_NAND_BOOT_MODE		(0x4U)
#define XFSBL_SD1_BOOT_MODE	        (0x5U)
#define XFSBL_EMMC_BOOT_MODE		(0x6U)
#define XFSBL_USB_BOOT_MODE			(0x7U)
#define XFSBL_SD1_LS_BOOT_MODE	    (0xEU)

volatile u32* DATA_0_RO = (u32*)0xE000A060;

struct SAVRStatus
{
	unsigned char nData : 8;
	bool          bSoH : 1; // 8
	bool          bEoT : 1; // 9
	bool          bTxW : 1; // 10
	bool          bRxR : 1; // 11
	bool          bRxE : 1; // 12
	bool          bRxF : 1; // 13
	bool          bTxF : 1; // 14
	bool          bTxE : 1; // 15
	bool          bTxErr : 1; // 16
	bool          bSrvc : 1; // 17
	bool          bTxRst : 1; // 18
	unsigned short nRxCnt : 10; // 19-28
	bool          bMezUpdated : 1; // 29
	bool          bVPDUpdated : 1; // 30
	bool          bSnsUpdated : 1; // 31

	bool          bRxRstDrv : 1; // 32
	bool          bRxRstSart : 1; // 33
	bool          bRxErr : 1; // 34
};

void PumpUARTInternal( AVRSysMon_Context* pContext, bool bFlush );

AVRSysMon_Mode GetSysmonModePin( AVRSysMon_Context* pContext )
{
	return AVRSysMon_ModeApplication;

	if( ( *DATA_0_RO & ( 1U << 14 ) ) == 0 )
	{
		return AVRSysMon_ModeService;
	}
	else
	{
		return AVRSysMon_ModeApplication;
	}
}

AVRSysMon_Alarm GetSysmonAlarmPin( AVRSysMon_Context* pContext )
{
	return AVRSysMon_AlarmNone;

	if( ( *DATA_0_RO & ( 1U << 15 ) ) == 0 )
	{
		return AVRSysMon_AlarmActive;
	}
	else
	{
		return AVRSysMon_AlarmNone;
	}
}

int CheckAVRSysmonError( AVRSysMon_Result eValue )
{
	switch( eValue )
	{
	case AVRSysMon_ResultOK:
	case AVRSysMon_ResultTXNoData:
	case AVRSysMon_ResultQueued:
	case AVRSysMon_ResultActive:
		return 1;
	default:
		return 0;
	}
}

void PumpUARTRxChar( AVRSysMon_Context* pContext, unsigned char nChar )
{
	static unsigned char chLast = 0;
	static bool bInFrame = false;
	static bool bInFrameSoH = false;
	static bool bRXEscape = false;
	static bool bCharBuffered = false;

	//std::cout << "\r\nData received from AVR 0x" << std::setw( 2 ) << std::right << std::hex << (unsigned int)nChar << ".";

	if( nChar == 0x10 )
	{
		bRXEscape = true;
#ifdef _UART_DEBUG
		XFsbl_Printf(DEBUG_INFO, "\r\nFound Rx Escape");
		#endif
	}
	else
	{
		bool bEscaped = false;
		if( bRXEscape )
		{
			if( ( nChar == 0x10 ) || (nChar == 0x01) || (nChar == 0x04) )
			{
#ifdef _UART_DEBUG
				xil_printf("\r\nIllegal character following escape from AVR 0x%02x.\r\n",(unsigned int)nChar);
#endif
			}
			nChar = ~nChar;
			bRXEscape = false;
			bEscaped = true;
#ifdef _UART_DEBUG
			XFsbl_Printf(DEBUG_INFO, "\r\nEscaped Rx Data is 0x%02x", nChar);
#endif
		}
		if( bInFrame )
		{
			bool bEnd = false;
			if( !bEscaped && ( nChar == 0x04 ) )
			{
				bEnd = true;
				bInFrame = false;
#ifdef _UART_DEBUG
				XFsbl_Printf(DEBUG_INFO, "\r\nFound Rx End");
#endif
				if( !bCharBuffered )
				{
#ifdef _UART_DEBUG
					xil_printf("\r\nEmpty frame detected!\r\n");
#endif
				}
			}
			if( bCharBuffered )
			{
				//std::cout << "\r\nPumping 0x" << std::setw( 2 ) << std::right << std::hex << (unsigned int)chLast;
				//std::cout << " SoH 0x" << std::setw( 2 ) << std::right << std::hex << (unsigned int)bInFrameSoH;
				//std::cout << " EoT 0x" << std::setw( 2 ) << std::right << std::hex << (unsigned int)bEnd << ".";
				AVRSysMon_PumpRX( pContext, chLast, bInFrameSoH, bEnd );
				bInFrameSoH = false;
			}
			bCharBuffered = !bEnd;
			chLast = nChar;
		}
		else
		{
			bCharBuffered = false;
			if( nChar == 0x01)
			{
				bInFrame = true;
				bInFrameSoH = true;
#ifdef _UART_DEBUG
				XFsbl_Printf(DEBUG_INFO, "\r\nFound RxSoH");
#endif
			}
			else
			{
#ifdef _DEBUG
				xil_printf("\r\nIllegal character following escape from AVR 0x%02x.\r\n",(unsigned int)nChar);
#endif
			}
		}
	}

}

void PumpUART( AVRSysMon_Context* pContext )
{
	PumpUARTInternal( pContext, false );
}

void PumpUARTInternal( AVRSysMon_Context* pContext, bool bFlush )
{
	AVRSysMon_Result eStatus;
	uint8_t nTxData = 0;
	bool bTxSoH = false;
	bool bTxEoT = false;
	XTime tStart;
	XTime tEnd;
	XTime tRes;
	XTime_GetTime(&tStart);

	eStatus = AVRSysMon_PumpTX( pContext, &nTxData, &bTxSoH, &bTxEoT );
	//eStatus = AVRSysMon_ResultOK;
	switch( eStatus )
	{
		case AVRSysMon_ResultOK:
		{
			bool bEscape = false;
			if( (nTxData == 0x01) || (nTxData == 0x04) || (nTxData == 0x10) )
			{
				bEscape = true;
				nTxData = ~nTxData;
			}
			if( bTxSoH )
			{
				while(XUartPs_IsTransmitFull(XPAR_XUARTPS_1_BASEADDR))
				{
					;
				}
				XUartPs_WriteReg( XPAR_XUARTPS_1_BASEADDR, XUARTPS_FIFO_OFFSET, 0x01 );
				usleep(200);
#ifdef _UART_DEBUG
				//XFsbl_Printf(DEBUG_INFO, "\r\nSending TxSoH");
#endif
			}
			if( bEscape )
			{
				while(XUartPs_IsTransmitFull(XPAR_XUARTPS_1_BASEADDR))
				{
					;
				}
				XUartPs_WriteReg( XPAR_XUARTPS_1_BASEADDR, XUARTPS_FIFO_OFFSET, 0x10 );
				usleep(200);
#ifdef _UART_DEBUG
				//XFsbl_Printf(DEBUG_INFO, "\r\nSending Tx Escape");
#endif
			}
			while(XUartPs_IsTransmitFull(XPAR_XUARTPS_1_BASEADDR))
			{
				;
			}
			XUartPs_WriteReg( XPAR_XUARTPS_1_BASEADDR, XUARTPS_FIFO_OFFSET, nTxData );
			usleep(200);
#ifdef _UART_DEBUG
			//XFsbl_Printf(DEBUG_INFO, "\r\nSending Tx Data 0x%02x", nTxData);
#endif
			if( bTxEoT )
			{
				while(XUartPs_IsTransmitFull(XPAR_XUARTPS_1_BASEADDR))
				{
					;
				}
				XUartPs_WriteReg( XPAR_XUARTPS_1_BASEADDR, XUARTPS_FIFO_OFFSET, 0x04 );
				usleep(200);
#ifdef _UART_DEBUG
				//XFsbl_Printf(DEBUG_INFO, "\r\nSending TxEoF");
#endif
			}
	#ifdef _DEBUG
			for( unsigned char n = 0; n < txCnt; n++ )
			{
				std::cout << "\r\nData sent to AVR 0x" << std::setw( 2 ) << std::right << std::hex << (unsigned int)pTxBuffer[n] << ".";
			}
			std::cout << "\r\n";
	#endif
		}
		break;
		case AVRSysMon_ResultTXNoData:
		{
		}
		break;
	}

	//If there is no TX data, just poll for RX bytes to process
	if( eStatus == AVRSysMon_ResultTXNoData )
	{
		while( !XUartPs_IsReceiveData(XPAR_XUARTPS_1_BASEADDR) )
		{
			XTime_GetTime(&tEnd);
			tRes = tEnd - tStart;
			if( tRes > 100000000U )
			{
#ifdef _UART_DEBUG
				xil_printf("\r\nNo data detected from AVR");
#endif
				//set_error(pContext, AVRSysMon_ResultTimeout);
				return AVRSysMon_ResultTimeout;
			}
		}
		uint8_t nByte = XUartPs_ReadReg(XPAR_XUARTPS_1_BASEADDR, XUARTPS_FIFO_OFFSET);
#ifdef _UART_DEBUG
		XFsbl_Printf(DEBUG_INFO, "\r\nRead RxData 0x%02x", nByte);
#endif
		PumpUARTRxChar( pContext, nByte );
	}
	else
	{
		//There are TX bytes to send, so don't waste time waiting for RX data.
		if( XUartPs_IsReceiveData(XPAR_XUARTPS_1_BASEADDR) )
		{
			uint8_t nByte = XUartPs_ReadReg(XPAR_XUARTPS_1_BASEADDR, XUARTPS_FIFO_OFFSET);
#ifdef _UART_DEBUG
		XFsbl_Printf(DEBUG_INFO, "\r\nRead RxData 0x%02x", nByte);
#endif
			PumpUARTRxChar( pContext, nByte );
		}
	}

	return;
}


AVRSysMon_Result AVRSysMon_PumpReq( AVRSysMon_Context* pContext, AVRSysMon_CommandReq* pReq )
{
	AVRSysMon_Result eReqResult;
	XTime tStart;
	XTime tEnd;
	XTime tRes;
	XTime_GetTime(&tStart);
	while( ( eReqResult = AVRSysMon_CompleteReq( pContext, pReq ) ) != AVRSysMon_ResultOK )
	{
		int nIdle = 1;
		if( !CheckAVRSysmonError( eReqResult ) ) return AVRSysMon_ResultFailed;
		// Pump RX side
		while( XUartPs_IsReceiveData(XPAR_XUARTPS_1_BASEADDR) )
		{
			nIdle = 0;
			uint8_t nByte = XUartPs_ReadReg(XPAR_XUARTPS_1_BASEADDR, XUARTPS_FIFO_OFFSET);
			PumpUARTRxChar( pContext, nByte );
		}
		// Pump TX side
		if( !XUartPs_IsTransmitFull(XPAR_XUARTPS_1_BASEADDR) )
		{
			uint8_t nTxData;
			bool bTxSoH = false;
			bool bTxEoT = false;
			eReqResult = AVRSysMon_PumpTX( pContext, &nTxData, &bTxSoH, &bTxEoT );
			switch( eReqResult )
			{
				case AVRSysMon_ResultOK:
				{
					bool bEscape = false;
					if( (nTxData == 0x01) || (nTxData == 0x04) || (nTxData == 0x10) )
					{
						bEscape = true;
						nTxData = ~nTxData;
					}

					if( bTxSoH )
					{
						XUartPs_WriteReg( XPAR_XUARTPS_1_BASEADDR, XUARTPS_FIFO_OFFSET, 0x01 );
						//xil_printf("\r\nSending SoH");
						usleep(200);
					}
					if( bEscape )
					{
						XUartPs_WriteReg( XPAR_XUARTPS_1_BASEADDR, XUARTPS_FIFO_OFFSET, 0x10 );
						//xil_printf("\r\nSending Escape");
						usleep(200);
					}
					XUartPs_WriteReg( XPAR_XUARTPS_1_BASEADDR, XUARTPS_FIFO_OFFSET, nTxData );
					if( bTxEoT )
					{
						XUartPs_WriteReg( XPAR_XUARTPS_1_BASEADDR, XUARTPS_FIFO_OFFSET, 0x04 );
						//xil_printf("\r\nSending EoT");
						usleep(200);
					}
		#ifdef _DEBUG
					for( unsigned char n = 0; n < txCnt; n++ )
					{
						xil_printf("\r\nData sent to AVR 0x%02x.",(unsigned int)pTxBuffer[n]);
					}
					xil_printf("\r\n");
		#endif
				}
				break;
				case AVRSysMon_ResultTXNoData:
				{
				}
				break;
				default:
#ifdef _UART_DEBUG
					xil_printf("\r\nUnexpected result.\r\n");
#endif
					break;
					//CloseHandle( g_hComm );
			}
		}
		if( nIdle == 1 )
		{
			XTime_GetTime(&tEnd);
			tRes = tEnd - tStart;
			if( tRes > 100000000U )
			{
				return AVRSysMon_ResultTimeout;
			}
		}
	}
	return AVRSysMon_ResultOK;
}
/*
const char* GetDeviceString( uint16_t nDevice )
{
	switch( nDevice )
	{
	case 292: return  "7Z045";
	case 293: return  "7Z100";
	default:
	return "Unknown";
	}
}

const char* GetTempGradeString( uint8_t nGrade )
{
	switch( nGrade )
	{
	case 0: return  "Commercial";
	case 1: return  "Industrial";
	case 2: return  "Extended ";
	default:
	return "Unknown   ";
	}
}

const char* GetSDRAMConfigSpeedPS( uint16_t nSpeed )
{
	switch( nSpeed )
	{
	case 5333: return "533.3MHz";
	case 6666: return "666.5MHz";
	case 8000: return "800.0MHz";
	case 9333: return "933.3MHz";
	default:
		return "???.?MHz";
	}
}

const char* GetSDRAMConfigSizePS( uint8_t nSize )
{
	switch( nSize )
	{
	case 0:  return "      ";
	case 26: return "256MiB"; // 2x1Gib
	case 27: return "512MiB"; // 2x2Gib
	case 28: return "1GiB  "; // 2x4Gib
	//case 29: return "2GiB  "; // 2x8Gib
	default:
		return "      ";
	}
}

u32 GetSDRAMConfigSizePSBytes( uint8_t nSize )
{
	switch( nSize )
	{
	case 0:  return 0;
	case 26: return 1024U * 1024U * 256U;  // 2x1Gib
	case 27: return 1024U * 1024U * 512U;  // 2x2Gib
	case 28: return 1024U * 1024U * 1024U; // 2x4Gib
	//case 29: return 1024U * 1024U * 2048U; // 2x8Gib // This is not possible why is it here....
	default:
		return 0;
	}
}


const char* GetSDRAMConfigSpeedPL( uint16_t nSpeed )
{
	switch( nSpeed )
	{
	case 4000: return "400.0MHz";
	case 8000: return "800.0MHz";
	default:
		return "???.?MHz";
	}
}

const char* GetSDRAMConfigSizePL( uint8_t nSize )
{
	switch( nSize )
	{
	case 0:  return "      ";
	case 26: return "128MiB";// 1x1Gib
	case 27: return "256MiB";// 1x2Gib
	case 28: return "512MiB";// 1x4Gib
	default:
		return "?x?Gib";
	}
}

const char* GetSDRAMConfigSizeQSPI( uint8_t nSize )
{
	switch( nSize )
	{
	case 0:  return "       ";
	case 28: return "32MiB  "; // 256Mib
	case 29: return "64Mib  "; // 512Mib
	default:
		return "       ";
	}
}
*/
const char* GetADVStatus( const uint8_t* pData, uintptr_t nOffSet, uint8_t nShift )
{
	uint8_t nData = 0xF & ( pData[nOffSet] >> nShift );
	switch( nData )
	{
	case 0x8: return "(OK)";
	case 0x5: return "(!L)";
	case 0x6: return "(!H)";
	case 0x1: return "(Lo)";
	case 0x2: return "(Hi)";
	default: return "(?) ";
	}
}

void AVRBootCheck( EBootCheckType eBootType )
{
	XUartPs pUartAvr;
	XUartPs_Config* pUartAvrConfig;
	uint32_t n;

	// Setup serial link
	pUartAvrConfig = XUartPs_LookupConfig(XPAR_PSU_UART_1_DEVICE_ID);
	XUartPs_CfgInitialize( &pUartAvr, pUartAvrConfig, XPAR_XUARTPS_1_BASEADDR );
	XUartPsFormat format;
	format.BaudRate = 170000;
	format.DataBits = XUARTPS_FORMAT_8_BITS;
	format.Parity = XUARTPS_FORMAT_NO_PARITY;
	format.StopBits = XUARTPS_FORMAT_1_STOP_BIT;
	if( XUartPs_SetDataFormat(&pUartAvr, &format) != XST_SUCCESS )
	{
		xil_printf( "\r\nError setting BAUD rate for AVR comms." );
		xil_printf( "\r\n" );
	}

	AVRSysMon_Context sSysMon;
	AVRSysMon_Callbacks sSysMonCallbacks;
	AVRSysMon_Result eResult;
	// Setup the call back that will be needed
	sSysMonCallbacks.m_pAlarmCheckCallback = &GetSysmonAlarmPin;
	sSysMonCallbacks.m_pModeCheckCallback = &GetSysmonModePin;
	sSysMonCallbacks.m_pTXIRQEnableCallback = 0;
	sSysMonCallbacks.m_pTXIRQDisableCallback = 0;

	if( eBootType == BootCheckNormal )
	{
#ifndef _DEBUG
		xil_printf( "\x1b""c" ); // Reset device
		xil_printf( "\x1b""[2J" ); // Erase the screen
#endif

#ifdef PRINT_ALPHADATA_BANNER
		xil_printf( "\r\n" );
		xil_printf( "\r\n                                                                        ");
		xil_printf( "\r\n            @@@@@@@                                                     ");
		xil_printf( "\r\n         @@@@@@@@@@ @                                                   ");
		xil_printf( "\r\n      @@@@@@@@@@@@@ @@                                                  ");
		xil_printf( "\r\n     @@@@@@@@@@@@@ @@@                                                  ");
		xil_printf( "\r\n   @@@@@@@@@@@@@@ @@@@                                                  ");
		xil_printf( "\r\n  @@@@@@@@@@@@@@ @@@@@                                                  ");
		xil_printf( "\r\n @@@      @@@@@ @@@@@                                                   ");
		xil_printf( "\r\n@@ @@@@@@@@ @@ @@@@@                                                    ");
		xil_printf( "\r\n@ @@@@@@@@@@  @@@@@   @   @@   @@@  @  @   @      @@@@    @   @@@@   @  ");
		xil_printf( "\r\n@@  @@@@@@@ @@ @@    @ @  @@   @@ @ @@@@  @ @     @@ @@  @ @   @@   @ @ ");
		xil_printf( "\r\n@@@@       @@@@     @@@@@ @@   @@@  @@@@ @@@@@    @@ @@ @@@@@  @@  @@@@@");
		xil_printf( "\r\n @@@@@@@@@@@@       @@ @@ @@@@ @@   @  @ @@ @@    @@@@  @  @@  @@  @@ @@");
		xil_printf( "\r\n    @@@@@@");
		xil_printf( "\r\n" );
		xil_printf( "\r\nAlpha Data Parallel Systems Ltd. ZynqMP FSBL Release 1.0.2.0" );
		xil_printf( "\r\n" );
		usleep(1000000);
#endif
	}

	// Try opening the sysmon
	eResult = AVRSysMon_Open( 0, &sSysMon, &sSysMonCallbacks );
	if( !CheckAVRSysmonError( eResult ) ) goto avr_error;

	while(0)
	{
		/*if(!TestLoopback( &sSysMon, PumpUART, false ))
			xil_printf("\r\nAVR loopback failed");
		else
			xil_printf("\r\nAVR loopback passed");*/
	}

	if( eBootType == BootCheckNormal )
	{
		// Get the firmware version numbers
		u16 pVersion[4];
		if(!CheckVersion( &sSysMon, PumpUART, false, pVersion ))
		{
			xil_printf("\r\nFailed to get Version Data");
			goto avr_error;
		}

		// Check mode
		/*AVRSysMon_Mode eMode;
		eResult = AVRSysMon_GetMode( &sSysMon, &eMode );
		if( !CheckAVRSysmonError( eResult ) ) goto avr_error;

		if( eMode == AVRSysMon_ModeService )
		{
			xil_printf( "\r\nBoard is in service mode.");
			xil_printf( "\r\n");
			xil_printf("\r\nBootman Ver : %d.%d.%d.%d", pVersion[0], pVersion[1], pVersion[2], pVersion[3] );
			eResult = AVRSysMon_Close( &sSysMon );
			if( !CheckAVRSysmonError( eResult ) ) goto avr_error;
			goto avr_done;
		}
		else*/
		{
			xil_printf( "\r\nBoard is in application mode. System monitor reporting:");
			xil_printf( "\r\n");
			xil_printf("\r\n Boardman Ver : %d.%d.%d.%d", pVersion[0], pVersion[1], pVersion[2], pVersion[3] );
		}
		xil_printf("\r\n");
	}

	// Get the VPD Data
	volatile uint32_t pDataVPD32[256/4];
	volatile uint8_t* pVPDData = (uint8_t*)(void*)pDataVPD32;
	int count=0;
	{
		if( !RequestVPD( &sSysMon, PumpUART, pVPDData ))
		{
			xil_printf("\r\nFailed to get VPD Data");
			usleep(1000000);
			// Flush any current RX data
			while( XUartPs_IsReceiveData(XPAR_XUARTPS_1_BASEADDR) )
			{
				uint8_t nByte = XUartPs_ReadReg(XPAR_XUARTPS_1_BASEADDR, XUARTPS_FIFO_OFFSET);
				(void)(nByte);
			}
			goto avr_error;
		}
		// Flush any current RX data
		while( XUartPs_IsReceiveData(XPAR_XUARTPS_1_BASEADDR) )
		{
			uint8_t nByte = XUartPs_ReadReg(XPAR_XUARTPS_1_BASEADDR, XUARTPS_FIFO_OFFSET);
			(void)(nByte);
		}
		//goto retryVPD;
	}

	EMemTypePart g_eMemPart = MT41K256M16;

	EBoardType g_eBoardType = BoardUnknown;
	EFPGAType g_eFPGAType = FPGAUnknown;

	//eResult = AVRSysMon_GetVPDData( &sSysMon, pVPDData, 256 );
	if( eBootType != BootCheckNormal )
	{
		SVpdAdmxrc9r1Rev1* pVPD = (SVpdAdmxrc9r1Rev1*)pVPDData;
		/*u32 nPSRamSize = GetSDRAMConfigSizePSBytes( pVPD->m_nRamPSSize );
		switch( nPSRamSize  )
		{
		case  1024U * 1024U * 512U:
			g_eMemPart = MT41K128M16;
			break;
		case  1024U * 1024U * 1024U:
			g_eMemPart = MT41K256M16;
			break;
		default:
			g_eMemPart = MT41K256M16;
			break;
		}
		switch( pVPD->m_nBoardType )
		{
		case 0x10F:
			g_eBoardType = ADMXRC7Z1;
			break;
		case 0x110:
			g_eBoardType = ADMXRC7Z2;
			break;
		default:
			g_eBoardType = BoardUnknown;
			break;
		}
		switch( pVPD->m_nFPGAType )
		{
		case 292:
			g_eFPGAType = ZC7045;
			break;
		case 293:
			g_eFPGAType = ZC7100;
			break;
		default:
			g_eFPGAType = FPGAUnknown;
			break;
		}*/
		eResult = AVRSysMon_Close( &sSysMon );
		if( !CheckAVRSysmonError( eResult ) ) goto avr_error;
			goto avr_done;
	}

	// Request the sensor page (64-bytes for admxrc7k1 and later)
	// Note this return requested data (not using automatically update page)
	uint8_t pSensorData[64];
	{
retrySensors:
		if( !ReportSensors( &sSysMon, PumpUART, false, pSensorData ) ) goto retrySensors;
		//AVRSysMon_GetSensorData(&sSysMon, pSensorData, 64  );
	}


	AVRSysMon_Alarm eAlarm;
	eResult = AVRSysMon_GetAlarm( &sSysMon, &eAlarm );
	if( !CheckAVRSysmonError( eResult ) ) goto avr_error;

	//ToDo: Implement some XRM function
	/*crReq.m_eType = AVRSysMon_ReqXRM;
	eResult = AVRSysMon_Request( &sSysMon, &crReq );
	if( !CheckAVRSysmonError( eResult ) ) goto avr_error;
	eResult = AVRSysMon_PumpReq( &sSysMon, &crReq );
	if( !CheckAVRSysmonError( eResult ) ) goto avr_error;
	uint8_t pXRMData[256];
	eResult = AVRSysMon_GetXRMData( &sSysMon, pXRMData, 256 );
	if( !CheckAVRSysmonError( eResult ) ) goto avr_error;*/

	SVpdAdmxrc9r1Rev1* pVPD = (SVpdAdmxrc9r1Rev1*)pVPDData;

	uint8_t nSum = 0;
	for( n = 0; n < 0x50 - 1; n++ )
	{
		nSum += ((uint8_t*)pVPD)[n];
	}
	uint8_t nCheckSum = 0 - nSum;
	if( pVPD->m_nCheckSum != nCheckSum )
	{
		//xil_printf("\r\nVPD Checksum error!");
	}
	else
	{
		bool bP6Fitted 			= 0;
		bool bP4Fitted 			= 0;
		bool bMicroUSBFitted	= 0;
		bool bMicroSDFitted 	= 0;

		// String safety
		pVPD->m_szFPGASpeed[3] = '\0';
		pVPD->m_szFPGAStepping[7] = '\0';



		/*switch( pVPD->m_nBoardType )
		{
		case 0x10F:
			xil_printf("\r\n Board Type : ADM-XRC-7Z1       ");
			//g_eBoardType = ADMXRC7Z1;
			break;
		case 0x110:
			xil_printf("\r\n Board Type : ADM-XRC-7Z2       ");
			//g_eBoardType = ADMXRC7Z2;
			break;
		default:
			xil_printf("\r\n Board Type : Unknown           ");
			//g_eBoardType = BoardUnknown;
			break;
		}*/
		/*switch( pVPD->m_nFPGAType )
		{
		case 292:
			g_eFPGAType = ZC7045;
			break;
		case 293:
			g_eFPGAType = ZC7100;
			break;
		default:
			g_eFPGAType = FPGAUnknown;
			break;
		}

		xil_printf("Serial Number : %d", pVPD->m_nSerialNumber );
		xil_printf("\r\n         P6 : %s", bP6Fitted ? "Fitted            " : "Not Fitted        " );
		xil_printf("           P4 : %s", bP4Fitted ? "Fitted            " : "Not Fitted        " );
		xil_printf("\r\n  Micro-USB : %s", bMicroUSBFitted ? "Fitted            " : "Not Fitted        " );
		xil_printf("     Micro-SD : %s", bMicroSDFitted ? "Fitted            " : "Not Fitted        " );

		xil_printf("\r\n   Prog Clk : %-9dHz       ", pVPD->m_nSi5338Clk1 );
		xil_printf(" Tgt Stepping : %s", pVPD->m_szFPGAStepping );

		xil_printf("\r\nSpeed Grade : %-4s              ", pVPD->m_szFPGASpeed );
		xil_printf("   Tgt Device : %s", GetDeviceString( pVPD->m_nFPGAType ) );

		xil_printf("\r\n Temp Grade : %s        ", GetTempGradeString( pVPD->m_nFPGATempGrade ) );
		xil_printf("          PCB : %d.%d", ( ( pVPD->m_nPCBRev >> 4 ) & 0xF ), ( pVPD->m_nPCBRev & 0xF ) );

		xil_printf("\r\n       CPLD : %d.%d               ", ( ( pVPD->m_nCPLDRev >> 4 ) & 0xF ), ( pVPD->m_nCPLDRev & 0xF ) );
		xil_printf("     PS SDRAM : %s@%s", GetSDRAMConfigSizePS( pVPD->m_nRamPSSize), GetSDRAMConfigSpeedPS( pVPD->m_nRamPSMaxFreq ) );

		xil_printf("\r\n      QSPI0 : %s              ", GetSDRAMConfigSizeQSPI( pVPD->m_nQSPI0Size ) );
		xil_printf("     QSPI1 : %s", GetSDRAMConfigSizeQSPI( pVPD->m_nQSPI1Size ) );

		xil_printf("\r\n  PL SDRAM0 : %s@%s   ", GetSDRAMConfigSizePL( pVPD->m_nRamPL0Size), GetSDRAMConfigSpeedPL( pVPD->m_nRamPL0MaxFreq ) );
		xil_printf("    PL SDRAM1 : %s@%s", GetSDRAMConfigSizePL( pVPD->m_nRamPL1Size), GetSDRAMConfigSpeedPL( pVPD->m_nRamPL1MaxFreq ) );

		xil_printf("\r\n");
		*/
	}

	// Report analogue sensors
	for( n = 0; g_p9r1Analog[n*2].m_szName != NULL; n++ )
	{
		void* pData = (void*)(&pSensorData[g_p9r1Analog[n*2].m_nOffset]);
		float fValue = g_p9r1Analog[n*2].m_fOffset + g_p9r1Analog[n*2].m_fScaleFactor * (float)*(uint16_t*)pData;
		float fInt;
		float fFrac = modff( fValue, &fInt );
		int nInt = (int)fInt;
		int nFrac = (int)(fabs(fFrac) * 1000.0f);
		xil_printf("\r\n%11s : %3d.%03d%s %s", g_p9r1Analog[n*2].m_szName, nInt, nFrac, g_p9r1Analog[n*2].m_szUnit, GetADVStatus( pSensorData, g_p9r1Analog[n*2].m_nStatusOffset, g_p9r1Analog[n*2].m_nStatusShift ) );
		if( g_p9r1Analog[n*2+1].m_szName == 0 )
		{
			break;
		}
		pData = (void*)(&pSensorData[g_p9r1Analog[n*2+1].m_nOffset]);
		fValue = g_p9r1Analog[n*2+1].m_fOffset + g_p9r1Analog[n*2+1].m_fScaleFactor * (float)*(uint16_t*)pData;
		fFrac = modff( fValue, &fInt );
		nInt = (int)fInt;
		nFrac = (int)(fabs(fFrac) * 1000.0f);
		xil_printf("       %11s : %3d.%03d%s %s", g_p9r1Analog[n*2+1].m_szName, nInt, nFrac, g_p9r1Analog[n*2+1].m_szUnit, GetADVStatus( pSensorData, g_p9r1Analog[n*2+1].m_nStatusOffset, g_p9r1Analog[n*2+1].m_nStatusShift ) );
	}

	// Report alarm interrupt status
	if( eAlarm == AVRSysMon_AlarmNone )
	{
		xil_printf("       Alarm State : OK" );
	}
	else
	{
		xil_printf("       Alarm State : ALARM!" );
	}
	xil_printf("\r\n");


	// Report binary sensors
	for( n = 0; g_p9r1Binary[n*2].m_szName != NULL; n++ )
	{
		uint8_t nData = pSensorData[g_p9r1Binary[n*2].m_nOffset];
		nData = 0x1 & ( nData >> g_p9r1Binary[n*2].m_nBit );
		xil_printf("\r\n%11s : %s", g_p9r1Binary[n*2].m_szName, ( nData == 0x1 ) ? "TRUE " : "FALSE" );
		if( g_p9r1Binary[n*2+1].m_szName == NULL )
		{
			break;
		}
		nData = pSensorData[g_p9r1Binary[n*2+1].m_nOffset];
		nData = 0x1 & ( nData >> g_p9r1Binary[n*2+1].m_nBit );
		xil_printf("               %11s : %s", g_p9r1Binary[n*2+1].m_szName, ( nData == 0x1 ) ? "TRUE " : "FALSE" );

	}

	// Report ETC, and Event counter
	uint32_t nCount = (uint32_t)pSensorData[4];
	nCount += (uint32_t)pSensorData[5] << 8;
	nCount += (uint32_t)pSensorData[6] << 16;
	xil_printf( "\r\n         EC : %-8d    ", nCount );

	void* pData = (void*)(&pSensorData[0]);
	uint32_t nData = *((uint32_t*)pData);
	unsigned int nSeconds = nData / 4;
	unsigned int nMins = nSeconds / 60;
	nSeconds -= nMins * 60;
	unsigned int nHours = nMins / 60;
	nMins -= nHours * 60;
	unsigned int nDays = nHours / 24;
	nHours -= nDays * 24;
	xil_printf( "                ETC : %dd:%dh:%dm:%ds", nDays, nHours, nMins, nSeconds );


	// Try closing the sysmon
	eResult = AVRSysMon_Close( &sSysMon );
	if( !CheckAVRSysmonError( eResult ) ) goto avr_error;
	goto avr_done;

avr_error:
	xil_printf( "\r\n" );
	xil_printf( "\r\nError occurred communicating with AVR (%d)\r\n", eResult );
	// Try cleaning up (don't care about result)
	AVRSysMon_Close( &sSysMon );

avr_done:

//#define _PS512
	// In debug build do a full memory test of the PS7 DDR memory.
#ifdef _DEBUG
{
	if( eBootType == BootCheckNormal )
	{
		u32 nPSRamSize = GetSDRAMConfigSizePSBytes( pVPD->m_nRamPSSize );
		if( nPSRamSize == 0 )
		{
			xil_printf( "\r\n" );
			xil_printf( "\r\n" );
			nPSRamSize = 1024U * 1024U * 512U;
			xil_printf( "\r\nPS SDRAM reported size 0. (VPD error). Falling back to testing %dMiB.", nPSRamSize / ( 1024U * 1024U ) );
		}
		if( ( nPSRamSize + 0x00100000 ) > 0x40000000 )
		{
			nPSRamSize = 0x40000000 - 0x00100000;
		}
		u32 nErrors = MemTest( 0x00100000, nPSRamSize );
		if( nErrors != 0 )
		{
			xil_printf( "\r\nOne or more PS SDRAM errors detected.\r\nBooting further would results in non-deterministic behaviour.\r\n" );
			xil_printf( "Please contact support@alpha-data.com for help.\r\n" );
			do
			{
				xil_printf( "\r|PS SDRAM FAILED|" );
				xil_printf( "\r\\PS SDRAM FAILED\\" );
				xil_printf( "\r-PS SDRAM FAILED-" );
				xil_printf( "\r/PS SDRAM FAILED/" );
			} while( true );
		}
	}
}
#endif

	// Check boot mode and display info ...
	if( eBootType == BootCheckNormal )
	{
		xil_printf( "\r\n" );
#ifdef XFSBL_OVERRIDE_BOOT_MODE
		u32 nBootModeRegister = XFSBL_OVERRIDE_BOOT_MODE;
#else
		u32 nBootModeRegister = XFsbl_In32(CRL_APB_BOOT_MODE_USER) &
				CRL_APB_BOOT_MODE_USER_BOOT_MODE_MASK;
#endif
		switch( nBootModeRegister )
		{
		case XFSBL_JTAG_BOOT_MODE:
			xil_printf( "\r\nBooting from JTAG ..."); break;
		case XFSBL_QSPI24_BOOT_MODE:
		case XFSBL_QSPI32_BOOT_MODE:
		case XFSBL_NAND_BOOT_MODE:
			xil_printf( "\r\nBooting from QSPI ..."); break;
		case XFSBL_SD0_BOOT_MODE:
		case XFSBL_SD1_BOOT_MODE:
		case XFSBL_SD1_LS_BOOT_MODE:
			xil_printf( "\r\nBooting from EMMC/SD card ..."); break;
		case XFSBL_EMMC_BOOT_MODE:
			xil_printf( "\r\nBooting from MMC card ..."); break;
		case XFSBL_USB_BOOT_MODE:
			xil_printf( "\r\nBooting from USB ..."); break;
		default:
			xil_printf( "\r\nUnknown boot mode selected!"); break;
			break;
		}
		xil_printf( "\r\n" );
	}

	// Make sure sysmon is closed.
	AVRSysMon_Close( &sSysMon );

}

#endif
