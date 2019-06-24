#include "cmds.h"

#include "../avrsysmon2_configs.h"

#ifdef FULL_CMDS_FLASH
CMD_FUNC( CmdFlashApplicationRead )
{
    if( nCmdLength < 5 )
    {
        return false;
    }
    nCmdLength = 5;
    pData[0] = pHeader[0];
    pData[1] = pHeader[1];
    pData[2] = 0x09; // Flash page size(512)
    pData[3] = 0x00; // Read start of application memroy (page 0)
    pData[4] = 0x00; //
    return true;
}

CMD_FUNC( CmdFlashApplicationWrite )
{
    if( nCmdLength < ( 512 + 5 ) )
    {
        return false;
    }
    nCmdLength = ( 512 + 5 );
    pData[0] = pHeader[0];
    pData[1] = pHeader[1];
    pData[2] = 0x09; // Flash page size(512)
    pData[3] = 0x00; // Read start of application memroy (page 0)
    pData[4] = 0x00; //
    for( unsigned int n = 5; n < (512 + 5 ); n++ )
    {
        pData[n] = 0xad;
    }
    return true;
}

#endif
