#include "resps.h"

extern bool g_bEnableRespVobose;

#include "../avrsysmon2_configs.h"

#ifdef FULL_RESPS_FLASH
RESP_FUNC( Resp_FlashApplicationRead )
{
    static unsigned char pData[512];
    static unsigned char chStatus;
    if( *nProgress == 0 )
    {
        chStatus = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    }
    else if( *nProgress < 512 )
    {
        pData[*nProgress-1] = nByte;
        (*nProgress)++;
        *bFinished = false;
        return !bEoT;
    }
    else
    {
        pData[*nProgress-1] = nByte;
        (*nProgress)++;
        *bFinished = true;
        //unsigned int nErrors = 0;
        if( g_bEnableRespVobose )
        {
            //std::cout << "\r\nFlash Data RXed. Status: " << std::dec << (unsigned int)chStatus << std::left << "\r\n";
            for( unsigned int n = 0; n < 32; n++ )
            {
                //std::cout << std::dec << std::right << std::setfill(' ') << std::setw(3) << (unsigned int)(n * 16) << ":" << std::hex << std::left;
                for( unsigned int i = 0; i < 16; i++ )
                {
                    //std::cout << (unsigned int)pData[i+n*16] << " ";
                }
                //std::cout << "\r\n";
            }
        }
        return bEoT;
    }
}

RESP_FUNC( Resp_FlashApplicationWrite )
{
    if( *nProgress == 0 )
    {
        if( g_bEnableRespVobose )
        {
            //std::cout << "\r\nFlash Write Responce: " << std::dec << (unsigned int)nByte << "\r\n";
        }
        (*nProgress)++;
        *bFinished = true;
        return bEoT;
    }
    return false;
}

#endif
