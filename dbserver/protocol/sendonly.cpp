#include "pch.h"
#include "common/ptcd2l_dblognotify.h"
#include "common/ptcd2p_registdbserver.h"
#include "designation/ptcd2g_d2gdesignationntf.h"
#include "common/ptcd2n_reportregistercount.h"
#include "common/ptcd2n_mysqlconnectlost.h"

void PtcD2L_DBLogNotify::Process(UINT32 dwConnID) {}
void PtcD2P_RegistDBServer::Process(UINT32 dwConnID) {}
void PtcD2G_D2GDesignationNtf::Process(UINT32 dwConnID) {}
void PtcD2N_ReportRegisterCount::Process(UINT32 dwConnID) {}
void PtcD2N_MysqlConnectLost::Process(UINT32 dwConnID) {}

