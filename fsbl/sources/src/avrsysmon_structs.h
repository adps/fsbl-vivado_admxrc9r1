#ifndef AVRSYSMON_STRUCTS_H_
#define AVRSYSMON_STRUCTS_H_

struct _SAnalogSensor
{
	const char* m_szName;
	const char* m_szUnit;
	uintptr_t	m_nOffset;
	float		m_fScaleFactor;
	float		m_fOffset;

	uintptr_t	m_nStatusOffset;
	uint8_t		m_nStatusShift;

	/*void println( UINT_PTR pData )
	{
		UINT16 nData = *(UINT16*)((UINT_PTR)pData + m_nOffset);
		float fData = (float)(UINT32)nData;
		sprintf( "\n% #25s: % #7.3f %s", m_szName, fData * m_fScaleFactor + m_fOffset, m_szUnit );
	};*/
};

typedef struct _SAnalogSensor SAnalogSensor;


struct _SBinarySensor
{
	const char* m_szName;
	uintptr_t	m_nOffset;
	uint8_t		m_nBit;

	/*void println( UINT_PTR pData )
	{
		UINT8 nData = *(UINT8*)((UINT_PTR)pData + m_nOffset);
		bool bData = ( nData & ( 0x1 << m_nBit ) ) != 0;
		printf( "\n% #25s: % #7s", m_szName, bData ? "true" : "false" );
	};*/
};

typedef struct _SBinarySensor SBinarySensor;

struct SI5538Error
{
	const char* m_szName;
	uintptr_t	m_nOffset;
	/*void println( UINT_PTR pData )
	{
		UINT8 nData = *(UINT8*)((UINT_PTR)pData + m_nOffset);
		if( (  ( nData >> 4 ) & 0xF ) != 0 )
		{
			printf( "\n% #25s: Error 0x%01x: Type 0x%01x", m_szName, ( nData >> 4 ) & 0xF, nData & 0xF );
		}
		else
		{
			printf( "\n% #25s: OK", m_szName );
		}
	}*/
};

typedef struct _SI5538Error SI5538Error;

struct _SETCSensor
{
	const char* m_szName;
	uintptr_t	m_nOffset;

	/*void println( UINT_PTR pData )
	{
		UINT32 nData = *(UINT32*)((UINT_PTR)pData + m_nOffset);
		unsigned int nSeconds = nData / 4;
		unsigned int nMins = nSeconds / 60;
		nSeconds -= nMins * 60;
		unsigned int nHours = nMins / 60;
		nMins -= nHours * 60;
		unsigned int nDays = nHours / 24;
		nHours -= nDays * 24;
		printf( "\n% #25s: %u days, %u hours, %u min.s, %u sec.s", m_szName, nDays, nHours, nMins, nSeconds );
	};*/
};

typedef struct _SETCSensor SETCSensor;


struct _SECSensor
{
	const char* m_szName;
	uintptr_t	m_nOffset0;
	uintptr_t	m_nOffset1;
	uintptr_t	m_nOffset2;
	uintptr_t	m_nOffset3;
	int			m_nByteWidth;

	/*void println( UINT_PTR pData )
	{
		UINT8 nData0 = *(UINT8*)((UINT_PTR)pData + m_nOffset0);
		UINT8 nData1 = *(UINT8*)((UINT_PTR)pData + m_nOffset1);
		UINT8 nData2 = *(UINT8*)((UINT_PTR)pData + m_nOffset2);
		UINT8 nData3 = *(UINT8*)((UINT_PTR)pData + m_nOffset3);
		UINT32 nCount = nData0;
		if( m_nByteWidth > 1 )
		{
			nCount += ((UINT32)nData1) << 8;
		}
		if( m_nByteWidth > 2 )
		{
			nCount += ((UINT32)nData2) << 16;
		}
		if( m_nByteWidth > 3 )
		{
			nCount += ((UINT32)nData3) << 24;
		}
		printf( "\n% #25s: %u", m_szName, nCount );
	};*/

};

typedef struct _SECSensor SECSensor;


#endif
