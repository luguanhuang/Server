#include "pch.h"
#include "forward.h"
#include "config.h"
#include "ptcregister.h"
#include "crpc.h"
#include "protocol.h"

// generate by ProtoGen at date: 2017/8/22 15:21:06

#include "login/ptct2n_logoutntf.h"
#include "queuing/ptcc2n_checkqueuingreq.h"
#include "common/ptcr2n_crossgsclosentf.h"
#include "scene/ptcg2n_scenedestroyedtons.h"
#include "common/ptcc2n_updatestartuptypentf.h"
#include "common/ptcm2n_updateregistercount2ns.h"
#include "common/ptcd2n_reportregistercount.h"
#include "common/ptcd2n_mysqlconnectlost.h"
#include "common/ptcm2n_notifyrouterinfo2ctrl.h"
#include "common/ptcm2n_m2nversoinnotify.h"
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
#include "login/rpct2n_loginrequestnew.h"
#include "role/rpcc2n_createrolenew.h"
#include "role/rpcn2d_createroleondbnew.h"
#include "role/rpcc2n_selectrolenew.h"
#include "scene/rpcn2g_enterscene.h"
#include "scene/rpcn2g_leavescene.h"
#include "role/rpcc2n_returntoselectrole.h"
#include "scene/rpcg2n_changesceneverify.h"
#include "scene/rpcm2n_createbattlescene.h"
#include "scene/rpcn2g_createbattle.h"
#include "common/rpcn2i_registerns2login.h"
#include "common/rpct2n_registergt2ns.h"
#include "common/rpcg2n_registergs2ns.h"
#include "login/rpcn2i_loginverify.h"
#include "login/rpcn2d_nsreadaccountdata.h"
#include "role/rpcn2d_nsreadroledata.h"
#include "login/rpcm2n_kickaccountfromms.h"
#include "scene/rpcm2n_changescenefromms.h"
#include "common/rpcn2r_registerctrl2router.h"
#include "role/rpcn2m_checkrolename.h"
#include "role/rpcn2m_checkrolebaninfo.h"
#include "common/rpcm2n_nsreload.h"
#include "common/rpcm2n_registerms2ns.h"
#include "login/rpcc2n_loginreconnectreq.h"
#include "login/rpcn2g_notifyroleloginreconnect2gs.h"
#include "gm/rpcm2n_gmdowhitelist.h"


static BaseLink *FindLink(const char *linkName)
{
	return Config::GetConfig()->FindLinkHandler(linkName);
}

void CProtocolRegister::Regiter() 
{
	CProtocol::Register(new PtcT2N_LogoutNtf());
	CProtocol::Register(new PtcC2N_CheckQueuingReq());
	CProtocol::Register(new PtcR2N_CrossGsCloseNtf());
	CProtocol::Register(new PtcG2N_SceneDestroyedToNs());
	CProtocol::Register(new PtcC2N_UpdateStartUpTypeNtf());
	CProtocol::Register(new PtcM2N_UpdateRegisterCount2Ns());
	CProtocol::Register(new PtcD2N_ReportRegisterCount());
	CProtocol::Register(new PtcD2N_MysqlConnectLost());
	CProtocol::Register(new PtcM2N_NotifyRouterInfo2Ctrl());
	CProtocol::Register(new PtcM2N_M2NVersoinNotify());
	CRpc::RegistRpc(RpcT2N_LoginRequestNew::CreateRpc());
	CRpc::RegistRpc(RpcC2N_CreateRoleNew::CreateRpc());
	CRpc::RegistRpc(RpcN2D_CreateRoleOnDBNew::CreateRpc());
	CRpc::RegistRpc(RpcC2N_SelectRoleNew::CreateRpc());
	CRpc::RegistRpc(RpcN2G_EnterScene::CreateRpc());
	CRpc::RegistRpc(RpcN2G_LeaveScene::CreateRpc());
	CRpc::RegistRpc(RpcC2N_ReturnToSelectRole::CreateRpc());
	CRpc::RegistRpc(RpcG2N_ChangeSceneVerify::CreateRpc());
	CRpc::RegistRpc(RpcM2N_CreateBattleScene::CreateRpc());
	CRpc::RegistRpc(RpcN2G_CreateBattle::CreateRpc());
	CRpc::RegistRpc(RpcN2I_RegisterNs2Login::CreateRpc());
	CRpc::RegistRpc(RpcT2N_RegisterGt2Ns::CreateRpc());
	CRpc::RegistRpc(RpcG2N_RegisterGs2Ns::CreateRpc());
	CRpc::RegistRpc(RpcN2I_LoginVerify::CreateRpc());
	CRpc::RegistRpc(RpcN2D_NsReadAccountData::CreateRpc());
	CRpc::RegistRpc(RpcN2D_NsReadRoleData::CreateRpc());
	CRpc::RegistRpc(RpcM2N_KickAccountFromMs::CreateRpc());
	CRpc::RegistRpc(RpcM2N_ChangeSceneFromMs::CreateRpc());
	CRpc::RegistRpc(RpcN2R_RegisterCtrl2Router::CreateRpc());
	CRpc::RegistRpc(RpcN2M_CheckRoleName::CreateRpc());
	CRpc::RegistRpc(RpcN2M_CheckRoleBanInfo::CreateRpc());
	CRpc::RegistRpc(RpcM2N_NsReload::CreateRpc());
	CRpc::RegistRpc(RpcM2N_registerms2ns::CreateRpc());
	CRpc::RegistRpc(RpcC2N_LoginReconnectReq::CreateRpc());
	CRpc::RegistRpc(RpcN2G_NotifyRoleLoginReconnect2Gs::CreateRpc());
	CRpc::RegistRpc(RpcM2N_GMDoWhiteList::CreateRpc());


	CProtocol::RegistProtoName(PTCT2N_LOGOUTNTF_TYPE, "PTCT2N_LOGOUTNTF");
	CProtocol::RegistProtoName(PTCC2N_CHECKQUEUINGREQ_TYPE, "PTCC2N_CHECKQUEUINGREQ");
	CProtocol::RegistProtoName(PTCR2N_CROSSGSCLOSENTF_TYPE, "PTCR2N_CROSSGSCLOSENTF");
	CProtocol::RegistProtoName(PTCG2N_SCENEDESTROYEDTONS_TYPE, "PTCG2N_SCENEDESTROYEDTONS");
	CProtocol::RegistProtoName(PTCC2N_UPDATESTARTUPTYPENTF_TYPE, "PTCC2N_UPDATESTARTUPTYPENTF");
	CProtocol::RegistProtoName(PTCM2N_UPDATEREGISTERCOUNT2NS_TYPE, "PTCM2N_UPDATEREGISTERCOUNT2NS");
	CProtocol::RegistProtoName(PTCD2N_REPORTREGISTERCOUNT_TYPE, "PTCD2N_REPORTREGISTERCOUNT");
	CProtocol::RegistProtoName(PTCD2N_MYSQLCONNECTLOST_TYPE, "PTCD2N_MYSQLCONNECTLOST");
	CProtocol::RegistProtoName(PTCM2N_NOTIFYROUTERINFO2CTRL_TYPE, "PTCM2N_NOTIFYROUTERINFO2CTRL");
	CProtocol::RegistProtoName(PTCM2N_M2NVERSOINNOTIFY_TYPE, "PTCM2N_M2NVERSOINNOTIFY");
	CProtocol::RegistProtoName(PTCN2G_NEWGATECONNECTED_TYPE, "PTCN2G_NEWGATECONNECTED");
	CProtocol::RegistProtoName(PTCN2T_KICKACCOUNT_TYPE, "PTCN2T_KICKACCOUNT");
	CProtocol::RegistProtoName(PTCN2D_ONLINENUMNTF_TYPE, "PTCN2D_ONLINENUMNTF");
	CProtocol::RegistProtoName(PTCN2I_USEDLOGINTOKENNOTIFY_TYPE, "PTCN2I_USEDLOGINTOKENNOTIFY");
	CProtocol::RegistProtoName(PTCN2C_CHECKQUEUINGNTF_TYPE, "PTCN2C_CHECKQUEUINGNTF");
	CProtocol::RegistProtoName(PTCN2R_REPORTSESSIONINFO_TYPE, "PTCN2R_REPORTSESSIONINFO");
	CProtocol::RegistProtoName(PTCN2M_NEWROLECREATED_TYPE, "PTCN2M_NEWROLECREATED");
	CProtocol::RegistProtoName(PTCN2M_UPDATEFUNCTIONOPEN2MS_TYPE, "PTCN2M_UPDATEFUNCTIONOPEN2MS");
	CProtocol::RegistProtoName(PTCN2G_UPDATEFUNCTIONOPEN2GS_TYPE, "PTCN2G_UPDATEFUNCTIONOPEN2GS");
	CProtocol::RegistProtoName(PTCN2T_NOTIFYGTSESSIONLOGIN_TYPE, "PTCN2T_NOTIFYGTSESSIONLOGIN");
	CProtocol::RegistProtoName(PTCN2R_NSCLOSENTF_TYPE, "PTCN2R_NSCLOSENTF");
	CProtocol::RegistProtoName(PTCN2G_UPDATESTARTUPTYPEN2GNTF_TYPE, "PTCN2G_UPDATESTARTUPTYPEN2GNTF");
	CProtocol::RegistProtoName(PTCN2I_UPDATESERVERSTATE2LOGIN_TYPE, "PTCN2I_UPDATESERVERSTATE2LOGIN");
	CProtocol::RegistProtoName(PTCN2D_ACCOUNTLOGOUT_TYPE, "PTCN2D_ACCOUNTLOGOUT");
	CProtocol::RegistProtoName(PTCN2T_SESSIONCHANGE2GT_TYPE, "PTCN2T_SESSIONCHANGE2GT");
	CProtocol::RegistProtoName(RPCT2N_LOGINREQUESTNEW_TYPE, "RPCT2N_LOGINREQUESTNEW");
	CProtocol::RegistProtoName(RPCC2N_CREATEROLENEW_TYPE, "RPCC2N_CREATEROLENEW");
	CProtocol::RegistProtoName(RPCN2D_CREATEROLEONDBNEW_TYPE, "RPCN2D_CREATEROLEONDBNEW");
	CProtocol::RegistProtoName(RPCC2N_SELECTROLENEW_TYPE, "RPCC2N_SELECTROLENEW");
	CProtocol::RegistProtoName(RPCN2G_ENTERSCENE_TYPE, "RPCN2G_ENTERSCENE");
	CProtocol::RegistProtoName(RPCN2G_LEAVESCENE_TYPE, "RPCN2G_LEAVESCENE");
	CProtocol::RegistProtoName(RPCC2N_RETURNTOSELECTROLE_TYPE, "RPCC2N_RETURNTOSELECTROLE");
	CProtocol::RegistProtoName(RPCG2N_CHANGESCENEVERIFY_TYPE, "RPCG2N_CHANGESCENEVERIFY");
	CProtocol::RegistProtoName(RPCM2N_CREATEBATTLESCENE_TYPE, "RPCM2N_CREATEBATTLESCENE");
	CProtocol::RegistProtoName(RPCN2G_CREATEBATTLE_TYPE, "RPCN2G_CREATEBATTLE");
	CProtocol::RegistProtoName(RPCN2I_REGISTERNS2LOGIN_TYPE, "RPCN2I_REGISTERNS2LOGIN");
	CProtocol::RegistProtoName(RPCT2N_REGISTERGT2NS_TYPE, "RPCT2N_REGISTERGT2NS");
	CProtocol::RegistProtoName(RPCG2N_REGISTERGS2NS_TYPE, "RPCG2N_REGISTERGS2NS");
	CProtocol::RegistProtoName(RPCN2I_LOGINVERIFY_TYPE, "RPCN2I_LOGINVERIFY");
	CProtocol::RegistProtoName(RPCN2D_NSREADACCOUNTDATA_TYPE, "RPCN2D_NSREADACCOUNTDATA");
	CProtocol::RegistProtoName(RPCN2D_NSREADROLEDATA_TYPE, "RPCN2D_NSREADROLEDATA");
	CProtocol::RegistProtoName(RPCM2N_KICKACCOUNTFROMMS_TYPE, "RPCM2N_KICKACCOUNTFROMMS");
	CProtocol::RegistProtoName(RPCM2N_CHANGESCENEFROMMS_TYPE, "RPCM2N_CHANGESCENEFROMMS");
	CProtocol::RegistProtoName(RPCN2R_REGISTERCTRL2ROUTER_TYPE, "RPCN2R_REGISTERCTRL2ROUTER");
	CProtocol::RegistProtoName(RPCN2M_CHECKROLENAME_TYPE, "RPCN2M_CHECKROLENAME");
	CProtocol::RegistProtoName(RPCN2M_CHECKROLEBANINFO_TYPE, "RPCN2M_CHECKROLEBANINFO");
	CProtocol::RegistProtoName(RPCM2N_NSRELOAD_TYPE, "RPCM2N_NSRELOAD");
	CProtocol::RegistProtoName(RPCM2N_REGISTERMS2NS_TYPE, "RPCM2N_REGISTERMS2NS");
	CProtocol::RegistProtoName(RPCC2N_LOGINRECONNECTREQ_TYPE, "RPCC2N_LOGINRECONNECTREQ");
	CProtocol::RegistProtoName(RPCN2G_NOTIFYROLELOGINRECONNECT2GS_TYPE, "RPCN2G_NOTIFYROLELOGINRECONNECT2GS");
	CProtocol::RegistProtoName(RPCM2N_GMDOWHITELIST_TYPE, "RPCM2N_GMDOWHITELIST");

}