#include "cmds.h"

#include "../avrsysmon2_configs.h"

CMD_FUNC( CmdServiceModeRequest )
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

CMD_FUNC( CmdApplicationModeReqest )
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

CMD_FUNC( CmdVersionQuery )
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

#ifdef FULL_CMDS_USER
CMD_FUNC( CmdLoopback32x8 )
{
    if( nCmdLength < 34 )
    {
        return false;
    }
    nCmdLength = 34;
    pData[0] = pHeader[0];
    pData[1] = pHeader[1];
    for( unsigned int n = 0; n < 32; n++ )
    {
        pData[2+n] = (unsigned char)(n + 1);
    }
    return true;
}
#endif
CMD_FUNC( CmdGetProductID )
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
