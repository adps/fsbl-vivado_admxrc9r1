#ifndef _CMDS
#define _CMDS

#include "cpp_types.h"

struct SCmdRequest
{
    unsigned char m_pHeader[2];
    bool (*m_fpt)( unsigned char*, unsigned short*, unsigned char* );
    // Header, In/Out buffer size, buffer
    const char* m_szName;
};

#define CMD_LIST_START(x) struct SCmdRequest x[] = {
#define CMD_LIST_END { { 0x00, 0x00 } , (bool(*)( unsigned char*, unsigned short*, unsigned char* ) ) 0x0 } };
#define CMD_FUNC(x) bool x ( unsigned char* pHeader, unsigned short* nCmdLength, unsigned char* pData )

#include "cmds_user.h"
#include "cmds_user_ext.h"
#include "cmds_flash.h"

#include "../avrsysmon2_configs.h"

static CMD_LIST_START( g_pCommandRequest )
    // User
    { { 0x03, 0x80 }, CmdServiceModeRequest, "Service Mode Request" },
    { { 0x03, 0x82 }, CmdApplicationModeReqest, "Application Mode Reqest" },
    { { 0x03, 0x83 }, CmdVersionQuery, "Version Query" },
#ifdef FULL_CMDS_USER
    { { 0x03, 0x84 }, CmdLoopback32x8, "Loopback32x8" },
#endif
    { { 0x03, 0x86 }, CmdGetProductID, "Get Product ID" },
    // User Ext (Application mode only)
#ifdef FULL_CMDS_USER_EXT
    { { 0x05, 0x80 }, CmdI2CRead, "I2C Read" },
    { { 0x05, 0x82 }, CmdI2CWrite, "I2C Write" },
    { { 0x05, 0x85 }, CmdAbortProtectionProtocol, "Abort Protection Protocol" },
    { { 0x05, 0x86 }, CmdInvokeProtectionProtocol, "Invoke Protection Protocol" },
#endif
    { { 0x05, 0x87 }, CmdRequestVPDUpdate, "Request VPD Update" },
#ifdef FULL_CMDS_USER_EXT
    { { 0x05, 0x88 }, CmdRequestMezUpdate, "Request Mez Update" },
#endif
    { { 0x05, 0x89 }, CmdRequestSensors,  "Request Sensors" },
#ifdef FULL_CMDS_USER_EXT
    { { 0x05, 0x8A }, CmdRequestBlockBox, "Request BlockBox" },
    { { 0x05, 0x8B }, CmdForceMezSupply, "Force Mez Supply" },
#endif
    { { 0x05, 0x8C }, CmdRequestVPDData, "Request VPD Data" },
#ifdef FULL_CMDS_USER_EXT
    { { 0x05, 0x8D }, CmdRequestMezData, "Request Mez Data" },
    { { 0x05, 0x8E }, CmdSetFrequency, "Set Frequency" },
    { { 0x05, 0x8F }, CmdGetFrequency, "Get Frequency" },
    { { 0x05, 0x81 }, CmdOverrideSensor, "Override Sensor" },
	{ { 0x05, 0x83 }, CmdSetFrequencyEx, "Set FrequencyEx" },
	{ { 0x05, 0x84 }, CmdGetFrequencyEx, "Get FrequencyEx" },
#endif
    // Flash Commands (Service mode only)
#ifdef FULL_CMDS_FLASH
    { { 0x06, 0x80 }, CmdFlashApplicationRead, "Flash Application Read" },
    { { 0x06, 0x82 }, CmdFlashApplicationWrite, "Flash Application Write" },
#endif
CMD_LIST_END

#endif
