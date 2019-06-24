#ifndef _AVRBOOT_H_
#define _AVRBOOT_H_

#define AVRBOOT

typedef enum _EBootCheckType
{
	BootCheckNormal,
	BootCheckVPD,
} EBootCheckType;

typedef enum _EBoardType
{
	ADMXRC7Z1,
	ADMXRC7Z2,
	ADMVPX39Z2,
	BoardUnknown,
} EBoardType;

typedef enum _EFPGAType
{
	ZC7045,
	ZC7100,
	ZCU15,
	FPGAUnknown,
} EFPGAType;

typedef enum _EMemTypePart
{
	MT41K128M16,
	MT41K256M16,
} EMemTypePart;

void AVRBootCheck( EBootCheckType eBootType );

extern EBoardType g_eBoardType;
extern EFPGAType g_eFPGAType;

#endif
