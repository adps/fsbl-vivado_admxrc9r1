

bool RequestVPD( AVRSysMon_Context* pContext, void( *fnPump )( AVRSysMon_Context* ), unsigned char* pVPD );
bool ReportSensors( AVRSysMon_Context* pContext, void( *fnPump )(AVRSysMon_Context*), bool bServiceMode, unsigned char* pSensorData );
