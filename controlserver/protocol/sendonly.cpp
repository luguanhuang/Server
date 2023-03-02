#include "pch.h"
#include "common/ptcn2g_newgateconnected.h"
#include "login/ptcn2t_kickaccount.h"
#include "common/ptcn2d_onlinenumntf.h"
#include "login/ptcn2i_usedlogintokennotify.h"
#include "queuing/ptcn2c_checkqueuingntf.h"
#include "session/ptcn2r_reportsessioninfo.h"
#include "role/ptcn2m_newrolecreated.h"
#include "common/ptcn2m_updatefunctionopen2ms.h"
#include "common/ptcn2g_updatefunctionopen2gs.h"
#include "login/ptcn2t_notifygtsessionlogin.h"
#include "common/ptcn2r_nsclosentf.h"
#include "common/ptcn2g_updatestartuptypen2gntf.h"
#include "common/ptcn2i_updateserverstate2login.h"
#include "account/ptcn2d_accountlogout.h"
#include "session/ptcn2t_sessionchange2gt.h"

void PtcN2G_NewGateConnected::Process(UINT32 dwConnID) {}
void PtcN2T_KickAccount::Process(UINT32 dwConnID) {}
void PtcN2D_OnlineNumNtf::Process(UINT32 dwConnID) {}
void PtcN2I_UsedLoginTokenNotify::Process(UINT32 dwConnID) {}
void PtcN2C_CheckQueuingNtf::Process(UINT32 dwConnID) {}
void PtcN2R_ReportSessionInfo::Process(UINT32 dwConnID) {}
void PtcN2M_NewRoleCreated::Process(UINT32 dwConnID) {}
void PtcN2M_UpdateFunctionOpen2Ms::Process(UINT32 dwConnID) {}
void PtcN2G_UpdateFunctionOpen2Gs::Process(UINT32 dwConnID) {}
void PtcN2T_NotifyGtSessionLogin::Process(UINT32 dwConnID) {}
void PtcN2R_NsCloseNtf::Process(UINT32 dwConnID) {}
void PtcN2G_UpdateStartUpTypeN2GNtf::Process(UINT32 dwConnID) {}
void PtcN2I_UpdateServerState2Login::Process(UINT32 dwConnID) {}
void PtcN2D_AccountLogout::Process(UINT32 dwConnID) {}
void PtcN2T_SessionChange2Gt::Process(UINT32 dwConnID) {}

