#include "pch.h"
#include "forward.h"
#include "config.h"
#include "ptcregister.h"
#include "crpc.h"
#include "protocol.h"

// generate by ProtoGen at date: 2017/10/24 15:13:26

#include "login/ptcn2i_usedlogintokennotify.h"
#include "common/ptcm2i_updateselfserver.h"
#include "gm/ptcp2i_updateplatnoticeloginntf.h"
#include "common/ptcn2i_updateserverstate2login.h"
#include "gm/ptcp2i_punishaccountntf.h"
#include "common/ptci2d_registloginserver.h"
#include "common/ptci2p_registerloginntf.h"
#include "common/ptci2d_changeserverstatentf.h"
#include "server/ptci2m_serverinfontf.h"
#include "server/ptci2k_serverinfontf2world.h"
#include "idip/ptci2m_banaccountlistntf.h"
#include "idip/ptci2m_delbanaccountntf.h"
#include "login/rpcc2i_querygateip.h"
#include "gm/rpcp2i_loginhttpcommand.h"
#include "common/rpcn2i_registerns2login.h"
#include "common/rpco2i_gmtoolcommand2login.h"
#include "login/rpcn2i_loginverify.h"
#include "common/rpcm2i_registerms2login.h"
#include "gm/rpcm2i_getplatnoticeinfo.h"
#include "platfriend/rpci2s_reqplatusersinfoi2s.h"
#include "role/rpcm2i_getzoneroleinfom2i.h"
#include "role/rpci2m_getzoneroleinfoi2m.h"


static BaseLink *FindLink(const char *linkName)
{
	return Config::GetConfig()->FindLinkHandler(linkName);
}

void CProtocolRegister::Regiter() 
{
	CProtocol::Register(new PtcN2I_UsedLoginTokenNotify());
	CProtocol::Register(new PtcM2I_UpdateSelfServer());
	CProtocol::Register(new PtcP2I_UpdatePlatNoticeLoginNtf());
	CProtocol::Register(new PtcN2I_UpdateServerState2Login());
	CProtocol::Register(new PtcP2I_PunishAccountNtf());
	CRpc::RegistRpc(RpcC2I_QueryGateIP::CreateRpc());
	CRpc::RegistRpc(RpcP2I_LoginHttpCommand::CreateRpc());
	CRpc::RegistRpc(RpcN2I_RegisterNs2Login::CreateRpc());
	CRpc::RegistRpc(RpcO2I_GMToolCommand2Login::CreateRpc());
	CRpc::RegistRpc(RpcN2I_LoginVerify::CreateRpc());
	CRpc::RegistRpc(RpcM2I_RegisterMs2Login::CreateRpc());
	CRpc::RegistRpc(RpcM2I_GetPlatNoticeInfo::CreateRpc());
	CRpc::RegistRpc(RpcI2S_ReqPlatUsersInfoI2S::CreateRpc());
	CRpc::RegistRpc(RpcM2I_GetZoneRoleInfoM2I::CreateRpc());
	CRpc::RegistRpc(RpcI2M_GetZoneRoleInfoI2M::CreateRpc());


	CProtocol::RegistProtoName(PTCN2I_USEDLOGINTOKENNOTIFY_TYPE, "PTCN2I_USEDLOGINTOKENNOTIFY");
	CProtocol::RegistProtoName(PTCM2I_UPDATESELFSERVER_TYPE, "PTCM2I_UPDATESELFSERVER");
	CProtocol::RegistProtoName(PTCP2I_UPDATEPLATNOTICELOGINNTF_TYPE, "PTCP2I_UPDATEPLATNOTICELOGINNTF");
	CProtocol::RegistProtoName(PTCN2I_UPDATESERVERSTATE2LOGIN_TYPE, "PTCN2I_UPDATESERVERSTATE2LOGIN");
	CProtocol::RegistProtoName(PTCP2I_PUNISHACCOUNTNTF_TYPE, "PTCP2I_PUNISHACCOUNTNTF");
	CProtocol::RegistProtoName(PTCI2D_REGISTLOGINSERVER_TYPE, "PTCI2D_REGISTLOGINSERVER");
	CProtocol::RegistProtoName(PTCI2P_REGISTERLOGINNTF_TYPE, "PTCI2P_REGISTERLOGINNTF");
	CProtocol::RegistProtoName(PTCI2D_CHANGESERVERSTATENTF_TYPE, "PTCI2D_CHANGESERVERSTATENTF");
	CProtocol::RegistProtoName(PTCI2M_SERVERINFONTF_TYPE, "PTCI2M_SERVERINFONTF");
	CProtocol::RegistProtoName(PTCI2K_SERVERINFONTF2WORLD_TYPE, "PTCI2K_SERVERINFONTF2WORLD");
	CProtocol::RegistProtoName(PTCI2M_BANACCOUNTLISTNTF_TYPE, "PTCI2M_BANACCOUNTLISTNTF");
	CProtocol::RegistProtoName(PTCI2M_DELBANACCOUNTNTF_TYPE, "PTCI2M_DELBANACCOUNTNTF");
	CProtocol::RegistProtoName(RPCC2I_QUERYGATEIP_TYPE, "RPCC2I_QUERYGATEIP");
	CProtocol::RegistProtoName(RPCP2I_LOGINHTTPCOMMAND_TYPE, "RPCP2I_LOGINHTTPCOMMAND");
	CProtocol::RegistProtoName(RPCN2I_REGISTERNS2LOGIN_TYPE, "RPCN2I_REGISTERNS2LOGIN");
	CProtocol::RegistProtoName(RPCO2I_GMTOOLCOMMAND2LOGIN_TYPE, "RPCO2I_GMTOOLCOMMAND2LOGIN");
	CProtocol::RegistProtoName(RPCN2I_LOGINVERIFY_TYPE, "RPCN2I_LOGINVERIFY");
	CProtocol::RegistProtoName(RPCM2I_REGISTERMS2LOGIN_TYPE, "RPCM2I_REGISTERMS2LOGIN");
	CProtocol::RegistProtoName(RPCM2I_GETPLATNOTICEINFO_TYPE, "RPCM2I_GETPLATNOTICEINFO");
	CProtocol::RegistProtoName(RPCI2S_REQPLATUSERSINFOI2S_TYPE, "RPCI2S_REQPLATUSERSINFOI2S");
	CProtocol::RegistProtoName(RPCM2I_GETZONEROLEINFOM2I_TYPE, "RPCM2I_GETZONEROLEINFOM2I");
	CProtocol::RegistProtoName(RPCI2M_GETZONEROLEINFOI2M_TYPE, "RPCI2M_GETZONEROLEINFOI2M");

}