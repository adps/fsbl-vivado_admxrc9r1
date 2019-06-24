#include "cmds.h"

#include "../avrsysmon2_configs.h"
#ifdef FULL_CMDS_USER_EXT
CMD_FUNC( CmdI2CRead )
{
    if( nCmdLength < 6 )
    {
        return false;
    }
    nCmdLength = 6;
    pData[0] = pHeader[0];
    pData[1] = pHeader[1];
    pData[2] = 0x00; // Bus
    pData[3] = 0x6B; // CA - ETC chip
    pData[4] = 0x05; // A  - ETC
    pData[5] = 0x04; // Bytes
    return true;
}

CMD_FUNC( CmdI2CWrite )
{
    if( nCmdLength < 7 )
    {
        return false;
    }
    nCmdLength = 6;
    pData[0] = pHeader[0];
    pData[1] = pHeader[1];
    pData[2] = 0x03; // Bus
    pData[3] = 0x50; // CA - ETC chip
    pData[4] = 0x00; // A  - ETC
    pData[5] = 0x00; // Bytes
    //pData[6] = 0x00; // Data
    return true;
}

CMD_FUNC( CmdAbortProtectionProtocol )
{
    if( nCmdLength < 2 )
    {
        return false;
    }
    nCmdLength = 2;
    pData[0] = pHeader[0];
    pData[1] = pHeader[1];
    return true;
}

CMD_FUNC( CmdInvokeProtectionProtocol )
{
    if( nCmdLength < 2 )
    {
        return false;
    }
    nCmdLength = 2;
    pData[0] = pHeader[0];
    pData[1] = pHeader[1];
    return true;
}

#endif

CMD_FUNC( CmdRequestVPDUpdate )
{
    if( nCmdLength < 2 )
    {
        return false;
    }
    nCmdLength = 2;
    pData[0] = pHeader[0];
    pData[1] = pHeader[1];
    return true;
}
#ifdef FULL_CMDS_USER_EXT

CMD_FUNC( CmdRequestMezUpdate )
{
    if( nCmdLength < 2 )
    {
        return false;
    }
    nCmdLength = 2;
    pData[0] = pHeader[0];
    pData[1] = pHeader[1];
    return true;
}
#endif
CMD_FUNC( CmdRequestSensors )
{
    if( nCmdLength < 2 )
    {
        return false;
    }
    nCmdLength = 2;
    pData[0] = pHeader[0];
    pData[1] = pHeader[1];
    return true;
}

#ifdef FULL_CMDS_USER_EXT

CMD_FUNC( CmdRequestBlockBox )
{
    if( nCmdLength < 3 )
    {
        return false;
    }
    nCmdLength = 3;
    pData[0] = pHeader[0];
    pData[1] = pHeader[1];
    pData[2] = 0x00;
    return true;
}

CMD_FUNC( CmdForceMezSupply )
{
    if( nCmdLength < 9 )
    {
        return false;
    }
    nCmdLength = 9;
    unsigned short nReq = 0;
    unsigned short nMin = 0;
    unsigned short nMax = 0xFFFFU;
    pData[0] = pHeader[0];
    pData[1] = pHeader[1];
    pData[2] = nReq & 0xFF;
    pData[3] = nReq >> 8;
    pData[4] = nMin & 0xFF;
    pData[5] = nMin >> 8;
    pData[6] = nMax & 0xFF;
    pData[7] = nMax >> 8;
    pData[8] = 0;
    return true;
}
#endif

CMD_FUNC( CmdRequestVPDData )
{
    if( nCmdLength < 2 )
    {
        return false;
    }
    nCmdLength = 2;
    pData[0] = pHeader[0];
    pData[1] = pHeader[1];
    return true;
}

#ifdef FULL_CMDS_USER_EXT
CMD_FUNC( CmdRequestMezData )
{
    if( nCmdLength < 3 )
    {
        return false;
    }
    nCmdLength = 3;
    pData[0] = pHeader[0];
    pData[1] = pHeader[1];
    pData[2] = 0x0;
    return true;
}

CMD_FUNC( CmdSetFrequency )
{
    if( nCmdLength < 7 )
    {
        return false;
    }
    nCmdLength = 7;
    pData[0] = pHeader[0];
    pData[1] = pHeader[1];
    pData[2] = 0x0;
    pData[3] = 0x00;
    pData[4] = 0xE1;
    pData[5] = 0xF5;
    pData[6] = 0x05;
    return true;
}

CMD_FUNC( CmdGetFrequency )
{
    if( nCmdLength < 3 )
    {
        return false;
    }
    nCmdLength = 3;
    pData[0] = pHeader[0];
    pData[1] = pHeader[1];
    pData[2] = 0x0;
    return true;
}

CMD_FUNC( CmdOverrideSensor )
{
    if( nCmdLength < 8 )
    {
        return false;
    }
    nCmdLength = 8;
    pData[0] = pHeader[0];
    pData[1] = pHeader[1];
    static unsigned int nFakeVal = 0;
    pData[2] = 0x0;
    pData[3] = ( nFakeVal >> 0  ) & 0xFF;
    pData[4] = ( nFakeVal >> 8  ) & 0xFF;
    pData[5] = ( nFakeVal >> 16 ) & 0xFF;
    pData[6] = ( nFakeVal >> 24 ) & 0xFF;
    pData[7] = 0x01;
    nFakeVal += 0xFF;
    return true;
}

CMD_FUNC( CmdSetFrequencyEx )
{
	if( nCmdLength < 8 )
	{
		return false;
	}
	nCmdLength = 8;
	pData[0] = pHeader[0];
	pData[1] = pHeader[1];
	pData[2] = 0x0;
	pData[3] = 0x00;
	pData[4] = 0xE1;
	pData[5] = 0xF5;
	pData[6] = 0x05;
	pData[7] = 0x03; // Don't adjust, just touch NMV (used for need boot)
	return true;
}

CMD_FUNC( CmdGetFrequencyEx )
{
	if( nCmdLength < 4 )
	{
		return false;
	}
	nCmdLength = 4;
	pData[0] = pHeader[0];
	pData[1] = pHeader[1];
	pData[2] = 0x0;
	pData[3] = 0x1;
	return true;
}

#endif
