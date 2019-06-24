#ifdef FATLOAD

#include "ff.h"
#include "xfsbl_debug.h"
#include "xilfpga_pcap.h"
#include <stdint.h>
#include "xparameters.h"

typedef int bool_t;

u8 g_bFPGAProgrammed = 0;

static int extractBinFileFromBit(
  uint8_t* pBitFile,
  uint32_t nBitSize,
  char* szIdentifier,
  uint32_t nIdentifierSize,
  uint32_t* pSize,
  uint32_t* pOffset)
{
	int32_t nRemaining;
	typedef enum
	{
		StatePreamble = 0,
		StateRecords  = 1,
		StateQuit     = 2
	} State;
	State state;
	int status = 0;
	uint32_t offset;
	bool_t bFound;
	uint8_t recType;
	uint16_t nRecLen;
	//int n;
	/* Find the beginning of the SelectMap data in the file */
	//remaining = fileSize;
	state = StatePreamble;
	bFound = FALSE;
	//pos = 0;
	offset = 0;
	nRemaining = nBitSize;
	//level = 0;
	//bMore = TRUE;
	//skip = 0;

	/*for( n = 0; n < 16; n++ )
	{
		xil_printf( "\r\n0x%x", (uint32_t)pBitFile[n] );
	}*/

	/*xil_printf("\r\n");
	xil_printf("\r\nuint32_t is %d bytes", sizeof(uint32_t));
	xil_printf("\r\nint32_t is %d bytes", sizeof(int32_t));
	xil_printf("\r\n");
	xil_printf("\r\n");*/
	while (!bFound)
	{
		nRemaining = (int32_t)nBitSize - (int32_t)offset;
		//xil_printf("\r\nSearching 0x%x",  (int32_t)offset );
		switch (state)
		{
		case StatePreamble:
			if (nRemaining < 2)
			{
				status = -2; // Not enough data!
				goto done;
			}
			else
			{
				/* Skip over the preamble */
				nRecLen = (uint16_t)(((uint16_t)pBitFile[offset] << 8) + pBitFile[offset + 1]);
				offset += 2;
				offset += nRecLen + 2;
				/*xil_printf("\r\nJumping 0x%x",  (int32_t)nRecLen );*/
				state = StateRecords;
			}
			break;
		case StateRecords:
			if( nRemaining < 1 )
			{
				status = -2; // Not enough data!
				goto done;
			}
			else
			{
				recType = pBitFile[offset];
				/*xil_printf("\r\nRecord Type 0x%x\r\n", (unsigned int)recType );*/
				switch (recType)
				{
				case 0x62:
					/* Part type record */
					if (nRemaining < 3)
					{
						status = -2; // Not enough data!
						goto done;
					}
					else
					{
						nRecLen = (uint16_t)(((uint16_t)pBitFile[offset + 1] << 8) + pBitFile[offset + 2]);
						if (nRecLen >= nIdentifierSize * sizeof(char) )
						{
							status = -3; // Device name too long.
							goto done;
						}
						if (nRemaining < (nRecLen + 3U))
						{
							status = -2; // Not enough data!
							goto done;
						}
						else
						{
							memcpy( szIdentifier, pBitFile + offset + 3, nRecLen );
							szIdentifier[nRecLen] = '\0';
							offset += nRecLen + 3U;
						}
					}
					break;
				case 0x65:
					/* End of header record */
					offset += 5U;
					status = 0;
					state = StateQuit;
					break;
				case 0x61:
				case 0x63:
				case 0x64:
					/* Unknown record types */
					if (nRemaining < 3)
					{
						status = -2; // Not enough data!
						goto done;
					}
					else
					{
						nRecLen = (uint16_t)(((uint16_t)pBitFile[offset + 1] << 8) + pBitFile[offset + 2]);
						if (nRemaining < (nRecLen + 3U))
						{
							status = -2; // Not enough data!
							goto done;
						}
						else
						{
							offset += nRecLen + 3U;
						}
					}
					break;
				default:
					/* Unexpected record type - ignore it */
					nRecLen = (uint16_t)(((uint16_t)pBitFile[offset + 1] << 8) + pBitFile[offset + 2]);
					offset += nRecLen + 3U;
					break;
				}
		  }
		  break;
		case StateQuit:
		  bFound = TRUE;
		  break;
		}
	}

  *pSize = nRemaining;
  *pOffset = offset;

done:
  return status;
}

//#include "xwdtps.h"

#ifdef XPAR_XWDTPS_0_BASEADDR
extern XWdtPs Watchdog;		/* Instance of WatchDog Timer	*/
#endif

extern u8 BitstreamFlag;

int FatBitLoadFromSD( const char* szFilename )
{
	static FIL fil;
	static FATFS fatfs;
	FRESULT rc;
	uint32_t nStatus;
	uint32_t* pSourceAddr = 0x0;
	//uint32_t* pLoadAddr = 0x0;
	uint32_t nSourceLength = 0;
	char szIdentifier[80];
	uint32_t nBitFileSize = 0;
	uint32_t nBitFileOffset = 0;
	uint32_t n;
	u32* pBitfileData;
	u32 nBitFileWords;
	u32 nWord;
	u32* pBitfileBuffer = 0;
	rc = f_mount(0, &fatfs);
	if (rc != FR_OK)
	{
		xil_printf("\r\nFailed to mount SD Card.", rc);
		return -1;
	}
	//xil_printf("\r\nMounted SD Card.", rc);
	rc = f_open(&fil, szFilename, FA_READ);
	if (rc != FR_OK)
	{
		xil_printf("\r\nUnable to open bitfile %s: %d", szFilename, rc);
		return -1;
	}
	xil_printf("\r\nReading %s (%d bytes)", szFilename, fil.fsize);
	pSourceAddr = (uint32_t*)XPAR_PS7_DDR_0_S_AXI_BASEADDR;
	rc = f_read( &fil, pSourceAddr, fil.fsize, (UINT*)&nSourceLength );
	if (rc != FR_OK)
	{
		xil_printf("\r\nUnable to read file %s: %d", szFilename, rc);
		return -1;
	}
	xil_printf("\r\nBitfile loaded.");

	szIdentifier[0] = '\0';

	nStatus = extractBinFileFromBit( (uint8_t*)pSourceAddr, fil.fsize,
			szIdentifier, 80, &nBitFileSize, &nBitFileOffset );

#ifdef _DEBUG
	xil_printf("\r\nBitfile device id: %s", szIdentifier );
	xil_printf("\r\nBitfile size: %d", nBitFileSize );
	xil_printf("\r\nBitfile offset: %d", nBitFileOffset );
#endif

#ifdef	XPAR_XWDTPS_0_BASEADDR
	/* Prevent WDT reset */
	XWdtPs_RestartWdt(&Watchdog);
#endif
	pBitfileData = (u32*)( (uintptr_t)pSourceAddr + (uintptr_t)nBitFileOffset);
	nBitFileWords = nBitFileSize >> 2;
	pBitfileBuffer = (u32*)(XPAR_PS7_DDR_0_S_AXI_BASEADDR + ( nBitFileSize << 2 ));
	for( n = 0; n < nBitFileWords; n++ )
	{
		nWord = pBitfileData[n];
		pBitfileBuffer[n] =  (( nWord & 0xFF ) << 24) | (( nWord & 0xFF00 ) << 8 ) | (( nWord & 0xFF0000 ) >> 8 ) | (( nWord & 0xFF000000 ) >> 24 );
	}

	//xil_printf("\r\nByte Swap Done." );

	nStatus = PcapLoadPartition( pBitfileBuffer,
		(u32*)XDCFG_DMA_INVALID_ADDRESS, nBitFileWords, nBitFileWords, 0 );
	if(nStatus != XST_SUCCESS)
	{
		xil_printf("\r\nPCAP Bitstream Download Failed.");
		return XST_FAILURE;
	}
	else
	{
		xil_printf("\r\nPCAP Bitstream Download Done OK.");
		g_bFPGAProgrammed = 1;
		BitstreamFlag = 1;
	}

	f_close(&fil);
	return nStatus;
}

#endif
