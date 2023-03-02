#include "pch.h"
#include "forward.h"
#include "config.h"
#include "ptcregister.h"
#include "crpc.h"
#include "protocol.h"

// generate by ProtoGen at date: 2017/11/6 17:39:44

#include "common/ptcg2d_registgameserver.h"
#include "common/ptcg2d_updatefield.h"
#include "common/ptcg2d_queryglobalconfig.h"
#include "reward/ptcg2d_queryreward.h"
#include "reward/ptcg2d_removereward.h"
#include "reward/ptcg2d_addreward.h"
#include "guild/ptcg2d_loadguilddatareq.h"
#include "guild/ptcg2d_updateguild.h"
#include "common/ptci2d_registloginserver.h"
#include "common/ptci2d_changeserverstatentf.h"
#include "mail/ptcm2d_addtemplatemail.h"
#include "mail/ptcm2d_mailoperation.h"
#include "chat/ptcm2d_mduploadchatntf.h"
#include "common/ptcm2d_msupdateglobalconfig.h"
#include "friend/ptcm2d_updatefriendondb.h"
#include "common/ptcn2d_onlinenumntf.h"
#include "social/ptcm2d_mssavesocialinfontf.h"
#include "ranklist/ptcm2d_saveranklistchangetodb.h"
#include "ranklist/ptcm2d_saveranklisttodb.h"
#include "role/ptcm2d_saverolesummarydata.h"
#include "guild/ptcm2d_saveguilddata.h"
#include "chat/ptcm2d_updateprivatelistntf.h"
#include "account/ptcm2d_savepunishdata.h"
#include "auction/ptcm2d_auctsaleitemdbntf.h"
#include "auction/ptcm2d_aucttraderecorddbntf.h"
#include "desvent/ptcm2d_updatedesignation.h"
#include "garden/ptcm2d_savegardeninfo.h"
#include "role/ptcm2d_delrolentf.h"
#include "common/ptcm2d_savepaylist.h"
#include "platfriend/ptcm2d_saveplatfriendinfo.h"
#include "auction/ptcm2d_guildauctupdatentf.h"
#include "gm/ptcm2d_saveplatnoticeptf.h"
#include "chat/ptcm2d_mddelchatntf.h"
#include "mentorship/ptcm2d_savementorinfontf.h"
#include "role/ptcm2d_deletecrossrolesummary.h"
#include "account/ptcn2d_accountlogout.h"
#include "common/ptcm2d_notifyrouterinfo2db.h"
#include "pay/ptcm2d_savepayibitem.h"
#include "custombattle/ptcm2d_updatecustombattleondb.h"
#include "custombattle/ptcm2d_updatecustombattleroleondb.h"
#include "mobabattle/ptcm2d_savemobarolerecord.h"
#include "marriage/ptcm2d_savemarriageinfontf.h"
#include "dbgroupchat/ptcm2d_dbgroupchatsavegroupissuentf.h"
#include "dbgroupchat/ptcm2d_dbgroupchatdelgroupissuentf.h"
#include "dbgroupchat/ptcm2d_dbgroupchatsaveroleissuentf.h"
#include "dbgroupchat/ptcm2d_dbgroupchatdelroleissuentf.h"
#include "dbgroupchat/ptcm2d_dbgroupchatsavegroupntf.h"
#include "dbgroupchat/ptcm2d_dbgroupchatdelgroupntf.h"
#include "dbgroupchat/ptcm2d_dbgroupchatdelrolentf.h"
#include "dbgroupchat/ptcm2d_dbgroupchatsaverolentf.h"
#include "dragonguild/ptcm2d_updatedragonguilddata.h"
#include "dbgoalawards/ptcm2d_dbgoalawardssaventf.h"
#include "common/ptcd2l_dblognotify.h"
#include "common/ptcd2p_registdbserver.h"
#include "designation/ptcd2g_d2gdesignationntf.h"
#include "common/ptcd2n_reportregistercount.h"
#include "common/ptcd2n_mysqlconnectlost.h"
#include "gm/rpcp2d_gmhttpcommand.h"
#include "login/rpcm2d_readaccountdata.h"
#include "role/rpcn2d_createroleondbnew.h"
#include "role/rpcm2d_readroledata.h"
#include "role/rpcg2d_saveroledata.h"
#include "role/rpcm2d_mssaveroledata.h"
#include "role/rpcm2d_msgetallrole.h"
#include "common/rpcm2d_dbreload.h"
#include "login/rpcn2d_nsreadaccountdata.h"
#include "role/rpcn2d_nsreadroledata.h"
#include "common/rpcd2r_registerdb2router.h"
#include "db/rpcm2d_updatepartnerdata.h"
#include "pay/rpcm2d_savepayaccessdata.h"
#include "db/rpcm2d_saveleagueteamdata.h"
#include "role/rpcg2d_readhiddenroles.h"
#include "common/rpcm2d_mssavedbend.h"
#include "role/rpcm2d_queryrolesummary.h"
#include "garden/rpcm2d_querygardeninfo.h"
#include "pay/rpcm2d_savegiftibitem.h"
#include "arenastar/rpcm2d_arenastarroledatasave.h"
#include "db/rpcm2d_skyteamdata2db.h"
#include "pay/rpcm2d_savegiftibhist.h"
#include "db/rpcm2d_updaterolereportdata.h"
#include "mobabattle/rpcm2d_querymobarolerecord.h"
#include "task/rpcm2d_updateroletaskdata.h"
#include "role/rpcm2d_readaccountdatanew.h"
#include "mail/rpcm2d_readmailfromdb.h"


static BaseLink *FindLink(const char *linkName)
{
	return Config::GetConfig()->FindLinkHandler(linkName);
}

void CProtocolRegister::Regiter() 
{
	CProtocol::Register(new PtcG2D_RegistGameServer());
	CProtocol::Register(new PtcG2D_UpdateField());
	CProtocol::Register(new PtcG2D_QueryGlobalConfig());
	CProtocol::Register(new PtcG2D_QueryReward());
	CProtocol::Register(new PtcG2D_RemoveReward());
	CProtocol::Register(new PtcG2D_AddReward());
	CProtocol::Register(new PtcG2D_LoadGuildDataReq());
	CProtocol::Register(new PtcG2D_UpdateGuild());
	CProtocol::Register(new PtcI2D_RegistLoginServer());
	CProtocol::Register(new PtcI2D_ChangeServerStateNtf());
	CProtocol::Register(new PtcM2D_AddTemplateMail());
	CProtocol::Register(new PtcM2D_MailOperation());
	CProtocol::Register(new PtcM2D_MDUpLoadChatNtf());
	CProtocol::Register(new PtcM2D_MsUpdateGlobalConfig());
	CProtocol::Register(new PtcM2D_UpdateFriendOnDB());
	CProtocol::Register(new PtcN2D_OnlineNumNtf());
	CProtocol::Register(new PtcM2D_MSSaveSocialInfoNtf());
	CProtocol::Register(new PtcM2D_SaveRankListChangeToDB());
	CProtocol::Register(new PtcM2D_SaveRankListToDB());
	CProtocol::Register(new PtcM2D_SaveRoleSummaryData());
	CProtocol::Register(new PtcM2D_SaveGuildData());
	CProtocol::Register(new PtcM2D_UpdatePrivateListNtf());
	CProtocol::Register(new PtcM2D_SavePunishData());
	CProtocol::Register(new PtcM2D_AuctSaleItemDBNtf());
	CProtocol::Register(new PtcM2D_AuctTradeRecordDBNtf());
	CProtocol::Register(new PtcM2D_UpdateDesignation());
	CProtocol::Register(new PtcM2D_SaveGardenInfo());
	CProtocol::Register(new PtcM2D_DelRoleNtf());
	CProtocol::Register(new PtcM2D_SavePayList());
	CProtocol::Register(new PtcM2D_SavePlatFriendInfo());
	CProtocol::Register(new PtcM2D_GuildAuctUpdateNtf());
	CProtocol::Register(new PtcM2D_SavePlatNoticePtf());
	CProtocol::Register(new PtcM2D_MDDelChatNtf());
	CProtocol::Register(new PtcM2D_SaveMentorInfoNtf());
	CProtocol::Register(new PtcM2D_DeleteCrossRoleSummary());
	CProtocol::Register(new PtcN2D_AccountLogout());
	CProtocol::Register(new PtcM2D_NotifyRouterInfo2DB());
	CProtocol::Register(new PtcM2D_SavePayIbItem());
	CProtocol::Register(new PtcM2D_UpdateCustomBattleOnDB());
	CProtocol::Register(new PtcM2D_UpdateCustomBattleRoleOnDB());
	CProtocol::Register(new PtcM2D_SaveMobaRoleRecord());
	CProtocol::Register(new PtcM2D_SaveMarriageInfoNtf());
	CProtocol::Register(new PtcM2D_DBGroupChatSaveGroupIssueNtf());
	CProtocol::Register(new PtcM2D_DBGroupChatDelGroupIssueNtf());
	CProtocol::Register(new PtcM2D_DBGroupChatSaveRoleIssueNtf());
	CProtocol::Register(new PtcM2D_DBGroupChatDelRoleIssueNtf());
	CProtocol::Register(new PtcM2D_DBGroupChatSaveGroupNtf());
	CProtocol::Register(new PtcM2D_DBGroupChatDelGroupNtf());
	CProtocol::Register(new PtcM2D_DBGroupChatDelRoleNtf());
	CProtocol::Register(new PtcM2D_DBGroupChatSaveRoleNtf());
	CProtocol::Register(new PtcM2D_UpdateDragonGuildData());
	CProtocol::Register(new PtcM2D_DBGoalAwardsSaveNtf());
	CRpc::RegistRpc(RpcP2D_GMHttpCommand::CreateRpc());
	CRpc::RegistRpc(RpcM2D_ReadAccountData::CreateRpc());
	CRpc::RegistRpc(RpcN2D_CreateRoleOnDBNew::CreateRpc());
	CRpc::RegistRpc(RpcM2D_ReadRoleData::CreateRpc());
	CRpc::RegistRpc(RpcG2D_SaveRoleData::CreateRpc());
	CRpc::RegistRpc(RpcM2D_MsSaveRoleData::CreateRpc());
	CRpc::RegistRpc(RpcM2D_MsGetAllRole::CreateRpc());
	CRpc::RegistRpc(RpcM2D_DbReload::CreateRpc());
	CRpc::RegistRpc(RpcN2D_NsReadAccountData::CreateRpc());
	CRpc::RegistRpc(RpcN2D_NsReadRoleData::CreateRpc());
	CRpc::RegistRpc(RpcD2R_RegisterDb2Router::CreateRpc());
	CRpc::RegistRpc(RpcM2D_UpdatePartnerData::CreateRpc());
	CRpc::RegistRpc(RpcM2D_SavePayAccessData::CreateRpc());
	CRpc::RegistRpc(RpcM2D_SaveLeagueTeamData::CreateRpc());
	CRpc::RegistRpc(RpcG2D_ReadHiddenRoles::CreateRpc());
	CRpc::RegistRpc(RpcM2D_MsSaveDBEnd::CreateRpc());
	CRpc::RegistRpc(RpcM2D_QueryRolesummary::CreateRpc());
	CRpc::RegistRpc(RpcM2D_QueryGardenInfo::CreateRpc());
	CRpc::RegistRpc(RpcM2D_SaveGiftIbItem::CreateRpc());
	CRpc::RegistRpc(RpcM2D_ArenaStarRoleDataSave::CreateRpc());
	CRpc::RegistRpc(RpcM2D_SkyTeamData2DB::CreateRpc());
	CRpc::RegistRpc(RpcM2D_SaveGiftIbHist::CreateRpc());
	CRpc::RegistRpc(RpcM2D_UpdateRoleReportData::CreateRpc());
	CRpc::RegistRpc(RpcM2D_QueryMobaRoleRecord::CreateRpc());
	CRpc::RegistRpc(RpcM2D_UpdateRoleTaskData::CreateRpc());
	CRpc::RegistRpc(RpcM2D_ReadAccountDataNew::CreateRpc());
	CRpc::RegistRpc(RpcM2D_ReadMailFromDB::CreateRpc());


	CProtocol::RegistProtoName(PTCG2D_REGISTGAMESERVER_TYPE, "PTCG2D_REGISTGAMESERVER");
	CProtocol::RegistProtoName(PTCG2D_UPDATEFIELD_TYPE, "PTCG2D_UPDATEFIELD");
	CProtocol::RegistProtoName(PTCG2D_QUERYGLOBALCONFIG_TYPE, "PTCG2D_QUERYGLOBALCONFIG");
	CProtocol::RegistProtoName(PTCG2D_QUERYREWARD_TYPE, "PTCG2D_QUERYREWARD");
	CProtocol::RegistProtoName(PTCG2D_REMOVEREWARD_TYPE, "PTCG2D_REMOVEREWARD");
	CProtocol::RegistProtoName(PTCG2D_ADDREWARD_TYPE, "PTCG2D_ADDREWARD");
	CProtocol::RegistProtoName(PTCG2D_LOADGUILDDATAREQ_TYPE, "PTCG2D_LOADGUILDDATAREQ");
	CProtocol::RegistProtoName(PTCG2D_UPDATEGUILD_TYPE, "PTCG2D_UPDATEGUILD");
	CProtocol::RegistProtoName(PTCI2D_REGISTLOGINSERVER_TYPE, "PTCI2D_REGISTLOGINSERVER");
	CProtocol::RegistProtoName(PTCI2D_CHANGESERVERSTATENTF_TYPE, "PTCI2D_CHANGESERVERSTATENTF");
	CProtocol::RegistProtoName(PTCM2D_ADDTEMPLATEMAIL_TYPE, "PTCM2D_ADDTEMPLATEMAIL");
	CProtocol::RegistProtoName(PTCM2D_MAILOPERATION_TYPE, "PTCM2D_MAILOPERATION");
	CProtocol::RegistProtoName(PTCM2D_MDUPLOADCHATNTF_TYPE, "PTCM2D_MDUPLOADCHATNTF");
	CProtocol::RegistProtoName(PTCM2D_MSUPDATEGLOBALCONFIG_TYPE, "PTCM2D_MSUPDATEGLOBALCONFIG");
	CProtocol::RegistProtoName(PTCM2D_UPDATEFRIENDONDB_TYPE, "PTCM2D_UPDATEFRIENDONDB");
	CProtocol::RegistProtoName(PTCN2D_ONLINENUMNTF_TYPE, "PTCN2D_ONLINENUMNTF");
	CProtocol::RegistProtoName(PTCM2D_MSSAVESOCIALINFONTF_TYPE, "PTCM2D_MSSAVESOCIALINFONTF");
	CProtocol::RegistProtoName(PTCM2D_SAVERANKLISTCHANGETODB_TYPE, "PTCM2D_SAVERANKLISTCHANGETODB");
	CProtocol::RegistProtoName(PTCM2D_SAVERANKLISTTODB_TYPE, "PTCM2D_SAVERANKLISTTODB");
	CProtocol::RegistProtoName(PTCM2D_SAVEROLESUMMARYDATA_TYPE, "PTCM2D_SAVEROLESUMMARYDATA");
	CProtocol::RegistProtoName(PTCM2D_SAVEGUILDDATA_TYPE, "PTCM2D_SAVEGUILDDATA");
	CProtocol::RegistProtoName(PTCM2D_UPDATEPRIVATELISTNTF_TYPE, "PTCM2D_UPDATEPRIVATELISTNTF");
	CProtocol::RegistProtoName(PTCM2D_SAVEPUNISHDATA_TYPE, "PTCM2D_SAVEPUNISHDATA");
	CProtocol::RegistProtoName(PTCM2D_AUCTSALEITEMDBNTF_TYPE, "PTCM2D_AUCTSALEITEMDBNTF");
	CProtocol::RegistProtoName(PTCM2D_AUCTTRADERECORDDBNTF_TYPE, "PTCM2D_AUCTTRADERECORDDBNTF");
	CProtocol::RegistProtoName(PTCM2D_UPDATEDESIGNATION_TYPE, "PTCM2D_UPDATEDESIGNATION");
	CProtocol::RegistProtoName(PTCM2D_SAVEGARDENINFO_TYPE, "PTCM2D_SAVEGARDENINFO");
	CProtocol::RegistProtoName(PTCM2D_DELROLENTF_TYPE, "PTCM2D_DELROLENTF");
	CProtocol::RegistProtoName(PTCM2D_SAVEPAYLIST_TYPE, "PTCM2D_SAVEPAYLIST");
	CProtocol::RegistProtoName(PTCM2D_SAVEPLATFRIENDINFO_TYPE, "PTCM2D_SAVEPLATFRIENDINFO");
	CProtocol::RegistProtoName(PTCM2D_GUILDAUCTUPDATENTF_TYPE, "PTCM2D_GUILDAUCTUPDATENTF");
	CProtocol::RegistProtoName(PTCM2D_SAVEPLATNOTICEPTF_TYPE, "PTCM2D_SAVEPLATNOTICEPTF");
	CProtocol::RegistProtoName(PTCM2D_MDDELCHATNTF_TYPE, "PTCM2D_MDDELCHATNTF");
	CProtocol::RegistProtoName(PTCM2D_SAVEMENTORINFONTF_TYPE, "PTCM2D_SAVEMENTORINFONTF");
	CProtocol::RegistProtoName(PTCM2D_DELETECROSSROLESUMMARY_TYPE, "PTCM2D_DELETECROSSROLESUMMARY");
	CProtocol::RegistProtoName(PTCN2D_ACCOUNTLOGOUT_TYPE, "PTCN2D_ACCOUNTLOGOUT");
	CProtocol::RegistProtoName(PTCM2D_NOTIFYROUTERINFO2DB_TYPE, "PTCM2D_NOTIFYROUTERINFO2DB");
	CProtocol::RegistProtoName(PTCM2D_SAVEPAYIBITEM_TYPE, "PTCM2D_SAVEPAYIBITEM");
	CProtocol::RegistProtoName(PTCM2D_UPDATECUSTOMBATTLEONDB_TYPE, "PTCM2D_UPDATECUSTOMBATTLEONDB");
	CProtocol::RegistProtoName(PTCM2D_UPDATECUSTOMBATTLEROLEONDB_TYPE, "PTCM2D_UPDATECUSTOMBATTLEROLEONDB");
	CProtocol::RegistProtoName(PTCM2D_SAVEMOBAROLERECORD_TYPE, "PTCM2D_SAVEMOBAROLERECORD");
	CProtocol::RegistProtoName(PTCM2D_SAVEMARRIAGEINFONTF_TYPE, "PTCM2D_SAVEMARRIAGEINFONTF");
	CProtocol::RegistProtoName(PTCM2D_DBGROUPCHATSAVEGROUPISSUENTF_TYPE, "PTCM2D_DBGROUPCHATSAVEGROUPISSUENTF");
	CProtocol::RegistProtoName(PTCM2D_DBGROUPCHATDELGROUPISSUENTF_TYPE, "PTCM2D_DBGROUPCHATDELGROUPISSUENTF");
	CProtocol::RegistProtoName(PTCM2D_DBGROUPCHATSAVEROLEISSUENTF_TYPE, "PTCM2D_DBGROUPCHATSAVEROLEISSUENTF");
	CProtocol::RegistProtoName(PTCM2D_DBGROUPCHATDELROLEISSUENTF_TYPE, "PTCM2D_DBGROUPCHATDELROLEISSUENTF");
	CProtocol::RegistProtoName(PTCM2D_DBGROUPCHATSAVEGROUPNTF_TYPE, "PTCM2D_DBGROUPCHATSAVEGROUPNTF");
	CProtocol::RegistProtoName(PTCM2D_DBGROUPCHATDELGROUPNTF_TYPE, "PTCM2D_DBGROUPCHATDELGROUPNTF");
	CProtocol::RegistProtoName(PTCM2D_DBGROUPCHATDELROLENTF_TYPE, "PTCM2D_DBGROUPCHATDELROLENTF");
	CProtocol::RegistProtoName(PTCM2D_DBGROUPCHATSAVEROLENTF_TYPE, "PTCM2D_DBGROUPCHATSAVEROLENTF");
	CProtocol::RegistProtoName(PTCM2D_UPDATEDRAGONGUILDDATA_TYPE, "PTCM2D_UPDATEDRAGONGUILDDATA");
	CProtocol::RegistProtoName(PTCM2D_DBGOALAWARDSSAVENTF_TYPE, "PTCM2D_DBGOALAWARDSSAVENTF");
	CProtocol::RegistProtoName(PTCD2L_DBLOGNOTIFY_TYPE, "PTCD2L_DBLOGNOTIFY");
	CProtocol::RegistProtoName(PTCD2P_REGISTDBSERVER_TYPE, "PTCD2P_REGISTDBSERVER");
	CProtocol::RegistProtoName(PTCD2G_D2GDESIGNATIONNTF_TYPE, "PTCD2G_D2GDESIGNATIONNTF");
	CProtocol::RegistProtoName(PTCD2N_REPORTREGISTERCOUNT_TYPE, "PTCD2N_REPORTREGISTERCOUNT");
	CProtocol::RegistProtoName(PTCD2N_MYSQLCONNECTLOST_TYPE, "PTCD2N_MYSQLCONNECTLOST");
	CProtocol::RegistProtoName(RPCP2D_GMHTTPCOMMAND_TYPE, "RPCP2D_GMHTTPCOMMAND");
	CProtocol::RegistProtoName(RPCM2D_READACCOUNTDATA_TYPE, "RPCM2D_READACCOUNTDATA");
	CProtocol::RegistProtoName(RPCN2D_CREATEROLEONDBNEW_TYPE, "RPCN2D_CREATEROLEONDBNEW");
	CProtocol::RegistProtoName(RPCM2D_READROLEDATA_TYPE, "RPCM2D_READROLEDATA");
	CProtocol::RegistProtoName(RPCG2D_SAVEROLEDATA_TYPE, "RPCG2D_SAVEROLEDATA");
	CProtocol::RegistProtoName(RPCM2D_MSSAVEROLEDATA_TYPE, "RPCM2D_MSSAVEROLEDATA");
	CProtocol::RegistProtoName(RPCM2D_MSGETALLROLE_TYPE, "RPCM2D_MSGETALLROLE");
	CProtocol::RegistProtoName(RPCM2D_DBRELOAD_TYPE, "RPCM2D_DBRELOAD");
	CProtocol::RegistProtoName(RPCN2D_NSREADACCOUNTDATA_TYPE, "RPCN2D_NSREADACCOUNTDATA");
	CProtocol::RegistProtoName(RPCN2D_NSREADROLEDATA_TYPE, "RPCN2D_NSREADROLEDATA");
	CProtocol::RegistProtoName(RPCD2R_REGISTERDB2ROUTER_TYPE, "RPCD2R_REGISTERDB2ROUTER");
	CProtocol::RegistProtoName(RPCM2D_UPDATEPARTNERDATA_TYPE, "RPCM2D_UPDATEPARTNERDATA");
	CProtocol::RegistProtoName(RPCM2D_SAVEPAYACCESSDATA_TYPE, "RPCM2D_SAVEPAYACCESSDATA");
	CProtocol::RegistProtoName(RPCM2D_SAVELEAGUETEAMDATA_TYPE, "RPCM2D_SAVELEAGUETEAMDATA");
	CProtocol::RegistProtoName(RPCG2D_READHIDDENROLES_TYPE, "RPCG2D_READHIDDENROLES");
	CProtocol::RegistProtoName(RPCM2D_MSSAVEDBEND_TYPE, "RPCM2D_MSSAVEDBEND");
	CProtocol::RegistProtoName(RPCM2D_QUERYROLESUMMARY_TYPE, "RPCM2D_QUERYROLESUMMARY");
	CProtocol::RegistProtoName(RPCM2D_QUERYGARDENINFO_TYPE, "RPCM2D_QUERYGARDENINFO");
	CProtocol::RegistProtoName(RPCM2D_SAVEGIFTIBITEM_TYPE, "RPCM2D_SAVEGIFTIBITEM");
	CProtocol::RegistProtoName(RPCM2D_ARENASTARROLEDATASAVE_TYPE, "RPCM2D_ARENASTARROLEDATASAVE");
	CProtocol::RegistProtoName(RPCM2D_SKYTEAMDATA2DB_TYPE, "RPCM2D_SKYTEAMDATA2DB");
	CProtocol::RegistProtoName(RPCM2D_SAVEGIFTIBHIST_TYPE, "RPCM2D_SAVEGIFTIBHIST");
	CProtocol::RegistProtoName(RPCM2D_UPDATEROLEREPORTDATA_TYPE, "RPCM2D_UPDATEROLEREPORTDATA");
	CProtocol::RegistProtoName(RPCM2D_QUERYMOBAROLERECORD_TYPE, "RPCM2D_QUERYMOBAROLERECORD");
	CProtocol::RegistProtoName(RPCM2D_UPDATEROLETASKDATA_TYPE, "RPCM2D_UPDATEROLETASKDATA");
	CProtocol::RegistProtoName(RPCM2D_READACCOUNTDATANEW_TYPE, "RPCM2D_READACCOUNTDATANEW");
	CProtocol::RegistProtoName(RPCM2D_READMAILFROMDB_TYPE, "RPCM2D_READMAILFROMDB");

}