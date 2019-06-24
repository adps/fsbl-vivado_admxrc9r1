#ifndef _RESPS_USER_EXT
#define _RESPS_USER_EXT

#include "resps.h"


RESP_FUNC( Resp_I2CRead );
RESP_FUNC( Resp_I2CWrite );
RESP_FUNC( Resp_AbortProtection );
RESP_FUNC( Resp_InvokeProtection );
RESP_FUNC( Resp_RequestVPDUpdate );
RESP_FUNC( Resp_RequestMezUpdate );
RESP_FUNC( Resp_RequestSensors );
RESP_FUNC( Resp_RequestBlockBox );
RESP_FUNC( Resp_ForceMezSupply );
RESP_FUNC( Resp_RequestVPDData );
RESP_FUNC( Resp_RequestMezData );
RESP_FUNC( Resp_SetFrequency );
RESP_FUNC( Resp_GetFrequency );
RESP_FUNC( Resp_OverrideSensor );
RESP_FUNC( Resp_SetFrequencyEx );
RESP_FUNC( Resp_GetFrequencyEx );

#endif
