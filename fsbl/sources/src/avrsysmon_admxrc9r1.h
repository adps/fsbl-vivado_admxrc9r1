#ifndef AVRSYSMON_ADMXRC9R1_H_
#define AVRSYSMON_ADMXRC9R1_H_

#include "avrsysmon_structs.h"

struct _VpdAdmxrc9r1Rev1
{
	uint16_t	m_nVPDVersion;
	uint16_t	m_nLength;
	uint16_t	m_nPartNumber;
	uint32_t	m_nSerialNumber;
	uint8_t	        m_nPCBRev;
	uint32_t	m_nSi5338RefClk;
	uint32_t	m_nSi5338Clk1;
	uint32_t	m_nReserved1;
	uint8_t		m_nQSPI0Size;
	uint8_t		m_nReserved2;
	uint8_t		m_nQSPI1Size;
	uint8_t		m_nReserved3;
	uint16_t	m_nRamPSMaxFreq;
	uint8_t		m_nRamPSSize;
	uint8_t		m_nReserved4;
	uint32_t	m_nReserved5;
	uint16_t	m_nRamPL0MaxFreq;
	uint8_t		m_nRamPL0Size;
	uint8_t		m_nReserved6;
	uint32_t	m_nReserved7;
	uint16_t	m_nRamPL1MaxFreq;
	uint8_t		m_nRamPL1Size;
	uint8_t		m_nReserved8;
	uint32_t	m_nReserved9;
	uint8_t  	m_szFPGAStepping[8];
	uint8_t  	m_szFPGASpeed[4];
	uint16_t	m_nFPGAType;
	uint8_t		m_nFPGATempGrade;
	uint8_t		m_nReserved10;
	uint8_t		m_nCPLDRev;
	uint8_t		m_nReserved11;
	uint8_t		m_nCheckSum;
};

typedef struct _VpdAdmxrc9r1Rev1 SVpdAdmxrc9r1Rev1;

//static SETCSensor g_s7z1ETCSensor = { "ETC", 0x0 };
//static SECSensor g_s7z1ECSensor = { "Events", 0x4, 0x5, 0x6, 0x7, 3 };
//static SI5538Error g_s7z1Si5338 = { "Si5338", 0x35 };

// Voltages in 4:12 format
// Temperature in 12:4 format
static SAnalogSensor g_p9r1Analog[] =
{
	{ "VPWR",				"V", 0x08, 1.0f / 4096.0f, 0.0f, 0x36, 0 },
	{ "12V0_DIG",			"V", 0x0A, 1.0f / 4096.0f, 0.0f, 0x36, 4 },
	{ "5V0_DIG",			"V", 0x0C, 1.0f / 4096.0f, 0.0f, 0x37, 0 },
	{ "3V3_DIG",			"V", 0x0E, 1.0f / 4096.0f, 0.0f, 0x37, 4 },
	{ "2V5_DIG",			"V", 0x10, 1.0f / 4096.0f, 0.0f, 0x38, 0 },
	{ "1V8_DIG",			"V", 0x12, 1.0f / 4096.0f, 0.0f, 0x38, 4 },
	{ "0V85_DIG",			"V", 0x14, 1.0f / 4096.0f, 0.0f, 0x39, 0 },
	{ "MGT_AUX_1V8",	    "V", 0x16, 1.0f / 4096.0f, 0.0f, 0x39, 4 },
	{ "VCC_DRAM",        	"V", 0x18, 1.0f / 4096.0f, 0.0f, 0x3a, 0 },
	{ "FMC_VADJ",			"V", 0x1A, 1.0f / 4096.0f, 0.0f, 0x3a, 4 },
	{ "0V85_BRAM",	 		"V", 0x1C, 1.0f / 4096.0f, 0.0f, 0x3b, 0 },
	{ "PL_MGTAVTT",			"V", 0x1E, 1.0f / 4096.0f, 0.0f, 0x3b, 4 },
	{ "PL_MGTAVCC",			"V", 0x20, 1.0f / 4096.0f, 0.0f, 0x3c, 0 },
	{ "VCCPSINTFP",			"V", 0x22, 1.0f / 4096.0f, 0.0f, 0x3c, 4 },
	{ "AVR Temp",			"C", 0x24, 1.0f / 16.0f, -273.15f, 0x3c, 0 },
	{ "PCB Temp",			"C", 0x26, 1.0f / 16.0f, -273.15f, 0x3c, 4 },
	{ "Target Temp",		"C", 0x2A, 1.0f / 16.0f, -273.15f, 0x3e, 4 },
	{ NULL,							0,   0x00, 0.0f },
};

static SBinarySensor g_p9r1Binary[] =
{
    //CRD_STAT0
	{ "PSU_OK_0",		   0x30, 0 },
	//{ "PSU_OK_1",		   0x30, 1 },
	//{ "PSU_OK_2",		   0x30, 2 },
	//{ "PSU_OK_3",		   0x30, 3 },
	//{ "PSU_OK_4",		   0x30, 4 },
	//{ "PSU_OK_5",		   0x30, 5 },
	//{ "PSU_OK_6/PGM2C0", 0x30, 6 },
	//{ "PSU_OK_7/PGM2C1", 0x30, 7 },

    //CRD_STAT1
	{ "FPGADone",          0x31, 0 },
	//{ "FPGA1_DN",        0x31, 1 },
	//{ "FPGA2_DN",        0x31, 2 },
	//{ "FPGA3_DN",        0x31, 3 },
	//{ "BrdCfg_Valid",    0x31, 4 },
	//{ "PSU_AUX",         0x31, 6 },
	//{ "PSU_OFF_REQ",     0x31, 7 },

    //CRD_STAT2
	//{ "GA0",             0x32, 0 },
	//{ "GA1",             0x32, 1 },
	//{ "GA2",             0x32, 2 },
	//{ "GA3",             0x32, 3 },
	//{ "GA4",             0x32, 4 },
	//{ "GAP",             0x32, 5 },
	//{ "USB_CON",         0x32, 6 },
	{ "PCIe Reset",        0x32, 7 },

	//MEZ_STAT0
	{ "FMC Present",       0x2c, 0 },
	{ "FMC VIO En",        0x2c, 1 },
	//{ "M2C_PWR_OK",      0x2c, 2 },
	{ "FMC I2C Er",        0x2c, 3 },
	{ "FMC Chks Er",       0x2c, 4 },
	{ "FMC Data Er",       0x2c, 5 },
	//{ "Present_B",       0x2c, 6 },
	{ "Force 2V5",         0x2c, 7 },

	//Si5338_STAT
	{ "SI5338 OK",         0x34, 0 },
	{ "Intrrgt OK",        0x34, 1 },
	//{ "CFG0 OK",		   0x34, 4 },
	//{ "CFG1 OK",		   0x34, 5 },
	//{ "CFG2 OK",		   0x34, 6 },
	//{ "CFG3 OK",		   0x34, 7 },

/*
	//{ "ADC A0 Under Range",0x36, 0 },
	//{ "ADC A0 Over Range", 0x36, 1 },
	//{ "ADC A0 Critical",   0x36, 2 },
	//{ "ADC A0 OK",		 0x36, 3 },
	{ "ADC A1 Under Range",0x36, 4 },
	{ "ADC A1 Over Range", 0x36, 5 },
	{ "ADC A1 Critical",   0x36, 6 },
	{ "ADC A1 OK",		   0x36, 7 },

	{ "ADC A2 Under Range",0x37, 0 },
	{ "ADC A2 Over Range", 0x37, 1 },
	{ "ADC A2 Critical",   0x37, 2 },
	{ "ADC A2 OK",		   0x37, 3 },
	{ "ADC A3 Under Range",0x37, 4 },
	{ "ADC A3 Over Range", 0x37, 5 },
	{ "ADC A3 Critical",   0x37, 6 },
	{ "ADC A3 OK",		   0x37, 7 },

	{ "ADC A4 Under Range",0x38, 0 },
	{ "ADC A4 Over Range", 0x38, 1 },
	{ "ADC A4 Critical",   0x38, 2 },
	{ "ADC A4 OK",		   0x38, 3 },
	{ "ADC A5 Under Range",0x38, 4 },
	{ "ADC A5 Over Range", 0x38, 5 },
	{ "ADC A5 Critical",   0x38, 6 },
	{ "ADC A5 OK",		   0x38, 7 },

	{ "ADC A6 Under Range",0x39, 0 },
	{ "ADC A6 Over Range", 0x39, 1 },
	{ "ADC A6 Critical",   0x39, 2 },
	{ "ADC A6 OK",		   0x39, 3 },
	{ "ADC A7 Under Range",0x39, 4 },
	{ "ADC A7 Over Range", 0x39, 5 },
	{ "ADC A7 Critical",   0x39, 6 },
	{ "ADC A7 OK",		   0x39, 7 },

	//{ "ADC B0 Under Range",0x3A, 0 },
	//{ "ADC B0 Over Range", 0x3A, 1 },
	//{ "ADC B0 Critical",   0x3A, 2 },
	//{ "ADC B0 OK",		 0x3A, 3 },
	{ "ADC B1 Under Range",0x3A, 4 },
	{ "ADC B1 Over Range", 0x3A, 5 },
	{ "ADC B1 Critical",   0x3A, 6 },
	{ "ADC B1 OK",		   0x3A, 7 },

	{ "ADC B2 Under Range",0x3B, 0 },
	{ "ADC B2 Over Range", 0x3B, 1 },
	{ "ADC B2 Critical",   0x3B, 2 },
	{ "ADC B2 OK",		   0x3B, 3 },
	{ "ADC B3 Under Range",0x3B, 4 },
	{ "ADC B3 Over Range", 0x3B, 5 },
	{ "ADC B3 Critical",   0x3B, 6 },
	{ "ADC B3 OK",		   0x3B, 7 },

	{ "ADC B4 Under Range",0x3C, 0 },
	{ "ADC B4 Over Range", 0x3C, 1 },
	{ "ADC B4 Critical",   0x3C, 2 },
	{ "ADC B4 OK",		   0x3C, 3 },
	{ "ADC B5 Under Range",0x3C, 4 },
	{ "ADC B5 Over Range", 0x3C, 5 },
	{ "ADC B5 Critical",   0x3C, 6 },
	{ "ADC B5 OK",		   0x3C, 7 },

	{ "ADC B6 Under Range",0x3D, 0 },
	{ "ADC B6 Over Range", 0x3D, 1 },
	{ "ADC B6 Critical",   0x3D, 2 },
	{ "ADC B6 OK",		   0x3D, 3 },
	{ "ADC B7 Under Range",0x3D, 4 },
	{ "ADC B7 Over Range", 0x3D, 5 },
	{ "ADC B7 Critical",   0x3D, 6 },
	{ "ADC B7 OK",		   0x3D, 7 },

	{ "TMP0 Under Range",	0x3E, 0 },
	{ "TMP0 Over Range",	0x3E, 1 },
	{ "TMP0 Critical",		0x3E, 2 },
	{ "TMP0 OK",			0x3E, 3 },
	{ "TMP1 Under Range",	0x3E, 4 },
	{ "TMP1 Over Range",	0x3E, 5 },
	{ "TMP1 Critical",		0x3E, 6 },
	{ "TMP1 OK",			0x3E, 7 },

	//{ "TMP2 Under Range",	0x3F, 0 },
	//{ "TMP2 Over Range",	0x3F, 1 },
	//{ "TMP2 Critical",		0x3F, 2 },
	//{ "TMP2 OK",			0x3F, 3 },
	{ "TMP3 Under Range",	0x3F, 4 },
	{ "TMP3 Over Range",	0x3F, 5 },
	{ "TMP3 Critical",		0x3F, 6 },
	{ "TMP3 OK",			0x3F, 7 },
*/
	{ NULL, 1, 0 },
};

#endif

