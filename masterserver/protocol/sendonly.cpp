#include "pch.h"
#include "qa/ptcm2c_answerackntf.h"
#include "qa/ptcm2c_pushquestionntf.h"
#include "qa/ptcm2c_qaoverntf.h"
#include "qa/ptcm2c_qaroomrankntf.h"
#include "qa/ptcm2g_updateguildqaroom.h"
#include "qa/ptcm2c_qaenterroomntf.h"
#include "common/ptcm2p_registmsserver.h"
#include "mail/ptcm2d_addtemplatemail.h"
#include "mail/ptcm2d_mailoperation.h"
#include "chat/ptcm2t_chatforward.h"
#include "chat/ptcm2d_mduploadchatntf.h"
#include "common/ptcm2t_versionnotify.h"
#include "chat/ptcm2c_mcchatofflinenotify.h"
#include "common/ptcm2d_msupdateglobalconfig.h"
#include "levelseal/ptcm2g_mslevelsealntf.h"
#include "friend/ptcm2c_friendopntfnew.h"
#include "friend/ptcm2c_frienddegreeupntfnew.h"
#include "friend/ptcm2c_blacklistntfnew.h"
#include "scene/ptcm2g_noticeguildbossendtogsn.h"
#include "friend/ptcm2d_updatefriendondb.h"
#include "friend/ptcm2g_updatefriendongs.h"
#include "battlewatch/ptcm2g_notifywatchiconnum.h"
#include "common/ptcm2c_rolestatentfnew.h"
#include "common/ptcm2c_hintnotifyms.h"
#include "scene/ptcm2g_synscenerewardm2g.h"
#include "guild/ptcm2g_changeguildbosstime.h"
#include "worldboss/ptcm2g_worldbosssyncgs.h"
#include "common/ptcm2l_mslognotify.h"
#include "social/ptcm2d_mssavesocialinfontf.h"
#include "social/ptcm2c_msreceiveflowerntf.h"
#include "social/ptcm2c_msflowerrainntf.h"
#include "team/ptcm2c_teamchangem2cntf.h"
#include "team/ptcm2c_leaveteamm2cntf.h"
#include "team/ptcm2c_teamfulldatam2cntf.h"
#include "team/ptcm2c_teaminvitem2cnotify.h"
#include "common/ptcm2c_mserrornotify.h"
#include "team/ptcm2c_inviterefusem2cntf.h"
#include "team/ptcm2c_rolematchstatem2cnotify.h"
#include "team/ptcm2c_fastmbconfirmm2cntf.h"
#include "team/ptcm2c_fastmbdismissm2cntf.h"
#include "common/ptcm2i_updateselfserver.h"
#include "ranklist/ptcm2d_saveranklistchangetodb.h"
#include "ranklist/ptcm2d_saveranklisttodb.h"
#include "role/ptcm2d_saverolesummarydata.h"
#include "qa/ptcm2c_qaidnamentf.h"
#include "firstpass/ptcm2g_firstpassnewcommendrewardntf.h"
#include "pk/ptcm2c_pktimeoutm2cntf.h"
#include "guild/ptcm2d_saveguilddata.h"
#include "guild/ptcm2c_loginguildinfo.h"
#include "guild/ptcm2c_sendguildskillinfo.h"
#include "chat/ptcm2d_updateprivatelistntf.h"
#include "guild/ptcm2g_synroleinguild.h"
#include "account/ptcm2d_savepunishdata.h"
#include "common/ptcm2c_mseventnotify.h"
#include "role/ptcm2g_synguildinfo.h"
#include "guild/ptcm2c_synguildarenafightunitnew.h"
#include "guild/ptcm2c_synguildarenabattleinfonew.h"
#include "guild/ptcm2c_noticeguildarenabeginnew.h"
#include "guild/ptcm2c_synguildarenaroleonlinenew.h"
#include "guild/ptcm2c_synguildarenadisplacenew.h"
#include "guild/ptcm2c_notifyguildskilldata.h"
#include "arena/ptcm2g_syncarenags.h"
#include "guild/ptcm2g_synguildsimpledata.h"
#include "guild/ptcm2g_mnewguildbonusntf.h"
#include "guild/ptcm2c_guildsetingnotify.h"
#include "auction/ptcm2d_auctsaleitemdbntf.h"
#include "auction/ptcm2d_aucttraderecorddbntf.h"
#include "guild/ptcm2g_notifyguildcheckinprocess.h"
#include "guild/ptcm2c_guildbestcardsntfms.h"
#include "stage/ptcm2c_derankchangntf.h"
#include "guild/ptcm2g_synfatigueresult.h"
#include "guild/ptcm2c_gmfjoinbattlem2creq.h"
#include "guild/ptcm2g_synguildcheckinhint.h"
#include "desvent/ptcm2d_updatedesignation.h"
#include "skycity/ptcm2c_skycityfinalres.h"
#include "common/ptcm2c_notifyidipmessagems.h"
#include "common/ptcm2c_idippunishinfomsntf.h"
#include "chat/ptcm2c_idipclearchatntf.h"
#include "garden/ptcm2d_savegardeninfo.h"
#include "guild/ptcm2c_guildcardrankntf.h"
#include "guild/ptcm2c_noticeguildladderstart.h"
#include "guild/ptcm2c_guildcardmatchntf.h"
#include "qa/ptcm2g_giveanserqarewardntf.h"
#include "qa/ptcm2g_qaovertogs.h"
#include "broadcast/ptcm2t_broadcastm2t.h"
#include "garden/ptcm2c_spritestatechangentf.h"
#include "garden/ptcm2g_gardeneventntf.h"
#include "team/ptcm2c_invunfstatem2cntf.h"
#include "role/ptcm2g_roleeventsyncgs.h"
#include "role/ptcm2d_delrolentf.h"
#include "guild/ptcm2c_noticeguildarenanexttime.h"
#include "garden/ptcm2c_gardenbanquetntf.h"
#include "common/ptcm2d_savepaylist.h"
#include "guild/ptcm2c_noticeguildwagereward.h"
#include "platfriend/ptcm2d_saveplatfriendinfo.h"
#include "guild/ptcm2c_sendguildbonusntf.h"
#include "auction/ptcm2d_guildauctupdatentf.h"
#include "reswar/ptcm2c_reswarguildbriefntf.h"
#include "guild/ptcm2c_synguilintegralstate.h"
#include "reswar/ptcm2c_reswarstatentf.h"
#include "guild/ptcm2c_synguildintegralstate.h"
#include "common/ptcm2g_syncservertime.h"
#include "reswar/ptcm2c_reswarranksimpleinfontf.h"
#include "reswar/ptcm2c_guildbuffsimpleinfontf.h"
#include "reswar/ptcm2c_guildbuffsimpleitemntf.h"
#include "common/ptcm2g_worldlevelntf.h"
#include "reswar/ptcm2c_halliconmntf.h"
#include "gm/ptcm2d_saveplatnoticeptf.h"
#include "reswar/ptcm2c_reswarminedatantf.h"
#include "guild/ptcm2g_noticeguildinheritnuminfo.h"
#include "chat/ptcm2c_largeroomrolentf.h"
#include "chat/ptcm2c_largeroomloginparamntf.h"
#include "chat/ptcm2f_leavefmlargeroom.h"
#include "reswar/ptcm2c_reswartimentf.h"
#include "chat/ptcm2f_fmshowntf.h"
#include "reswar/ptcm2c_guildbuffcdparamntf.h"
#include "scene/ptcm2g_msdestoryscene.h"
#include "mentorship/ptcm2c_notifymentorapply.h"
#include "partner/ptcm2c_updatepartnertoclient.h"
#include "partner/ptcm2g_updatepartnertogs.h"
#include "partner/ptcm2c_makepartnerresultntf.h"
#include "invfight/ptcm2c_invfightnotify.h"
#include "chat/ptcm2d_mddelchatntf.h"
#include "mentorship/ptcm2d_savementorinfontf.h"
#include "guildcastle/ptcm2g_gcfsynm2g.h"
#include "pay/ptcm2c_payparameterinfoinvalidntf.h"
#include "guild/ptcm2c_noticeguildterrwar.h"
#include "guild/ptcm2c_noticeguildterrall.h"
#include "guild/ptcm2g_tryteacherinherit.h"
#include "pay/ptcm2g_balanceinfontf.h"
#include "guildbonus/ptcm2c_guildbonusgetall.h"
#include "common/ptcm2n_updateregistercount2ns.h"
#include "reswar/ptcm2c_reswarenemytimentf.h"
#include "chat/ptcm2f_audiochanneltofm.h"
#include "league/ptcm2c_updateleaguebattleseasoninfo.h"
#include "league/ptcm2c_notifyleagueteamcreate.h"
#include "league/ptcm2c_notifyleagueteamdissolve.h"
#include "league/ptcm2c_updateleagueteamstate.h"
#include "league/ptcm2c_leaguebattlestartmatchntf.h"
#include "league/ptcm2c_leaguebattlestopmatchntf.h"
#include "league/ptcm2c_leaguebattlematchtimeoutntf.h"
#include "pay/ptcm2c_midasexceptionntf.h"
#include "league/ptcm2c_updateleagueeleroomstatentf.h"
#include "guild/ptcm2g_synguildbosshp.h"
#include "guild/ptcm2g_synguildbossdie.h"
#include "guild/ptcm2g_synguildbossdoend.h"
#include "chat/ptcm2c_kickfmauchor.h"
#include "guild/ptcm2g_noticeguildbossattr.h"
#include "misc/ptcm2g_controlfunctionntf.h"
#include "auction/ptcm2c_guildauctitemtimefresh.h"
#include "league/ptcm2k_leaguematchworldntf.h"
#include "pokertournament/ptcm2c_pokertournamentendrefund.h"
#include "misc/ptcm2g_clicknoticentf.h"
#include "herobattle/ptcm2k_herobattleworldmatchntf.h"
#include "role/ptcm2d_deletecrossrolesummary.h"
#include "guild/ptcm2c_noticeguildterrend.h"
#include "guild/ptcm2c_noticeguildterrbattlewin.h"
#include "guild/ptcm2c_noticeguildterrbigicon.h"
#include "common/ptcm2k_reportrolesummary2world.h"
#include "common/ptcm2k_getrolesummaryfromworld.h"
#include "militaryrank/ptcm2g_militaryrankchange.h"
#include "common/ptcm2t_notifyrouterinfo2gate.h"
#include "common/ptcm2d_notifyrouterinfo2db.h"
#include "common/ptcm2n_notifyrouterinfo2ctrl.h"
#include "pay/ptcm2g_buypayaileenntf.h"
#include "guild/ptcm2c_modifyguildnamentf.h"
#include "worldboss/ptcm2g_worldbossguildaddattrsyncgsntf.h"
#include "platfriend/ptcm2s_m2suserstatussync.h"
#include "scene/ptcm2g_enterscenefromms.h"
#include "common/ptcm2g_broadcastglobalvalue.h"
#include "pay/ptcm2d_savepayibitem.h"
#include "arenastar/ptcm2c_arenastardatantf.h"
#include "skyteam/ptcm2c_notifyskyteamcreate.h"
#include "skycraft/ptcm2c_skycraftmatchntf.h"
#include "skycraft/ptcm2c_skycrafteliroomntf.h"
#include "pay/ptcm2c_ibgifticonntf.h"
#include "pay/ptcm2c_giftibsucessntf.h"
#include "custombattle/ptcm2d_updatecustombattleondb.h"
#include "custombattle/ptcm2d_updatecustombattleroleondb.h"
#include "custombattle/ptcm2k_reportcustombattleresultworld.h"
#include "custombattle/ptcm2k_custombattledelmatch.h"
#include "custombattle/ptcm2k_transcustombattlematch.h"
#include "team/ptcm2c_teamrequestplatfreind2client.h"
#include "custombattle/ptcm2c_custombattlegmnotify.h"
#include "team/ptcm2c_tarjabriefntf.h"
#include "pk/ptcm2g_pkupdatesynntf.h"
#include "mobabattle/ptcm2d_savemobarolerecord.h"
#include "bigmelee/ptcm2k_bmenterreadyscenentf.h"
#include "militaryrank/ptcm2g_addmilitaryrank.h"
#include "common/ptcm2k_servercrossdatam2w.h"
#include "team/ptcm2h_teamtranms2ts.h"
#include "team/ptcm2g_teamtranms2gs.h"
#include "common/ptcm2n_m2nversoinnotify.h"
#include "groupchat/ptcm2c_groupchatmanager.h"
#include "groupchat/ptcm2c_groupchatquit.h"
#include "groupchat/ptcm2c_groupchatdismiss.h"
#include "wedding/ptcm2c_weddinginvitentf.h"
#include "marriage/ptcm2d_savemarriageinfontf.h"
#include "marriage/ptcm2c_notifymarriageapply.h"
#include "marriage/ptcm2c_notifymarriagedivorceapply.h"
#include "groupchat/ptcm2c_groupchatissuecount.h"
#include "groupchat/ptcm2c_groupchatapply.h"
#include "dbgroupchat/ptcm2d_dbgroupchatsavegroupissuentf.h"
#include "dbgroupchat/ptcm2d_dbgroupchatdelgroupissuentf.h"
#include "dbgroupchat/ptcm2d_dbgroupchatsaveroleissuentf.h"
#include "dbgroupchat/ptcm2d_dbgroupchatdelroleissuentf.h"
#include "dbgroupchat/ptcm2d_dbgroupchatsavegroupntf.h"
#include "dbgroupchat/ptcm2d_dbgroupchatdelgroupntf.h"
#include "dbgroupchat/ptcm2d_dbgroupchatdelrolentf.h"
#include "dbgroupchat/ptcm2d_dbgroupchatsaverolentf.h"
#include "wedding/ptcm2g_weddingcarendm2g.h"
#include "team/ptcm2c_startbattlefailedm2cntf.h"
#include "battlefield/ptcm2k_battlefieldeventreq.h"
#include "dragonguild/ptcm2g_syndragonguildsimpledata.h"
#include "dragonguild/ptcm2c_logindragonguildinfo.h"
#include "dragonguild/ptcm2c_dragonguildsettingchanged.h"
#include "dragonguild/ptcm2d_updatedragonguilddata.h"
#include "dragonguild/ptcm2c_dragonguildnamentf.h"
#include "dragonguild/ptcm2g_notifydragonguildredpoint.h"
#include "goalawards/ptcm2c_goalawardsredpoint.h"
#include "goalawards/ptcm2g_goalawardsserverawards.h"
#include "battlefield/ptcm2g_battlefiedlreadyscenegsntf.h"
#include "dbgoalawards/ptcm2d_dbgoalawardssaventf.h"
#include "npcfeeling/ptcm2g_npcflnotifym2g.h"
#include "battlefield/ptcm2g_battlefailinfogsntf.h"
#include "task/ptcm2c_dailytaskeventntf.h"
#include "pay/ptcm2g_consumerebatentf.h"
#include "task/ptcm2c_taskrefreshntf.h"
#include "guildcastle/ptcm2c_guildcastfeatsntf.h"
#include "pay/ptcm2g_paytotalntf.h"
#include "pk/ptcm2k_pkranklogsyn.h"

void PtcM2C_AnswerAckNtf::Process(UINT32 dwConnID) {}
void PtcM2C_PushQuestionNtf::Process(UINT32 dwConnID) {}
void PtcM2C_QAOverNtf::Process(UINT32 dwConnID) {}
void PtcM2C_QARoomRankNtf::Process(UINT32 dwConnID) {}
void PtcM2G_UpdateGuildQARoom::Process(UINT32 dwConnID) {}
void PtcM2C_QAEnterRoomNtf::Process(UINT32 dwConnID) {}
void PtcM2P_RegistMSServer::Process(UINT32 dwConnID) {}
void PtcM2D_AddTemplateMail::Process(UINT32 dwConnID) {}
void PtcM2D_MailOperation::Process(UINT32 dwConnID) {}
void PtcM2T_chatforward::Process(UINT32 dwConnID) {}
void PtcM2D_MDUpLoadChatNtf::Process(UINT32 dwConnID) {}
void PtcM2T_VersionNotify::Process(UINT32 dwConnID) {}
void PtcM2C_MCChatOffLineNotify::Process(UINT32 dwConnID) {}
void PtcM2D_MsUpdateGlobalConfig::Process(UINT32 dwConnID) {}
void PtcM2G_MsLevelSealNtf::Process(UINT32 dwConnID) {}
void PtcM2C_FriendOpNtfNew::Process(UINT32 dwConnID) {}
void PtcM2C_FriendDegreeUpNtfNew::Process(UINT32 dwConnID) {}
void PtcM2C_BlackListNtfNew::Process(UINT32 dwConnID) {}
void PtcM2G_noticeguildbossendtogsn::Process(UINT32 dwConnID) {}
void PtcM2D_UpdateFriendOnDB::Process(UINT32 dwConnID) {}
void PtcM2G_UpdateFriendOnGS::Process(UINT32 dwConnID) {}
void PtcM2G_NotifyWatchIconNum::Process(UINT32 dwConnID) {}
void PtcM2C_RoleStateNtfNew::Process(UINT32 dwConnID) {}
void PtcM2C_HintNotifyMS::Process(UINT32 dwConnID) {}
void PtcM2G_SynSceneRewardM2G::Process(UINT32 dwConnID) {}
void PtcM2G_ChangeGuildBossTime::Process(UINT32 dwConnID) {}
void PtcM2G_WorldBossSyncGS::Process(UINT32 dwConnID) {}
void PtcM2L_MSLogNotify::Process(UINT32 dwConnID) {}
void PtcM2D_MSSaveSocialInfoNtf::Process(UINT32 dwConnID) {}
void PtcM2C_MSReceiveFlowerNtf::Process(UINT32 dwConnID) {}
void PtcM2C_MSFlowerRainNtf::Process(UINT32 dwConnID) {}
void PtcM2C_TeamChangeM2CNtf::Process(UINT32 dwConnID) {}
void PtcM2C_LeaveTeamM2CNtf::Process(UINT32 dwConnID) {}
void PtcM2C_TeamFullDataM2CNtf::Process(UINT32 dwConnID) {}
void PtcM2C_TeamInviteM2CNotify::Process(UINT32 dwConnID) {}
void PtcM2C_MSErrorNotify::Process(UINT32 dwConnID) {}
void PtcM2C_InviteRefuseM2CNtf::Process(UINT32 dwConnID) {}
void PtcM2C_RoleMatchStateM2CNotify::Process(UINT32 dwConnID) {}
void PtcM2C_FastMBConfirmM2CNtf::Process(UINT32 dwConnID) {}
void PtcM2C_fastMBDismissM2CNtf::Process(UINT32 dwConnID) {}
void PtcM2I_UpdateSelfServer::Process(UINT32 dwConnID) {}
void PtcM2D_SaveRankListChangeToDB::Process(UINT32 dwConnID) {}
void PtcM2D_SaveRankListToDB::Process(UINT32 dwConnID) {}
void PtcM2D_SaveRoleSummaryData::Process(UINT32 dwConnID) {}
void PtcM2C_QAIDNameNtf::Process(UINT32 dwConnID) {}
void PtcM2G_FirstPassNewCommendRewardNtf::Process(UINT32 dwConnID) {}
void PtcM2C_PkTimeoutM2CNtf::Process(UINT32 dwConnID) {}
void PtcM2D_SaveGuildData::Process(UINT32 dwConnID) {}
void PtcM2C_LoginGuildInfo::Process(UINT32 dwConnID) {}
void PtcM2C_SendGuildSkillInfo::Process(UINT32 dwConnID) {}
void PtcM2D_UpdatePrivateListNtf::Process(UINT32 dwConnID) {}
void PtcM2G_SynRoleInGuild::Process(UINT32 dwConnID) {}
void PtcM2D_SavePunishData::Process(UINT32 dwConnID) {}
void PtcM2C_MSEventNotify::Process(UINT32 dwConnID) {}
void PtcM2G_SynGuildInfo::Process(UINT32 dwConnID) {}
void PtcM2C_SynGuildArenaFightUnitNew::Process(UINT32 dwConnID) {}
void PtcM2C_SynGuildArenaBattleInfoNew::Process(UINT32 dwConnID) {}
void PtcM2C_NoticeGuildArenaBeginNew::Process(UINT32 dwConnID) {}
void PtcM2C_SynGuildArenaRoleOnlineNew::Process(UINT32 dwConnID) {}
void PtcM2C_synguildarenadisplaceNew::Process(UINT32 dwConnID) {}
void PtcM2C_NotifyGuildSkillData::Process(UINT32 dwConnID) {}
void PtcM2G_SyncArenaGs::Process(UINT32 dwConnID) {}
void PtcM2G_SynGuildSimpleData::Process(UINT32 dwConnID) {}
void PtcM2G_MNewGuildBonusNtf::Process(UINT32 dwConnID) {}
void PtcM2C_GuildSetingNotify::Process(UINT32 dwConnID) {}
void PtcM2D_AuctSaleItemDBNtf::Process(UINT32 dwConnID) {}
void PtcM2D_AuctTradeRecordDBNtf::Process(UINT32 dwConnID) {}
void PtcM2G_NotifyGuildCheckinProcess::Process(UINT32 dwConnID) {}
void PtcM2C_GuildBestCardsNtfMs::Process(UINT32 dwConnID) {}
void PtcM2C_DERankChangNtf::Process(UINT32 dwConnID) {}
void PtcM2G_SynFatigueResult::Process(UINT32 dwConnID) {}
void PtcM2C_GmfJoinBattleM2CReq::Process(UINT32 dwConnID) {}
void PtcM2G_SynGuildCheckinHint::Process(UINT32 dwConnID) {}
void PtcM2D_UpdateDesignation::Process(UINT32 dwConnID) {}
void PtcM2C_SkyCityFinalRes::Process(UINT32 dwConnID) {}
void PtcM2C_NotifyIdipMessageMs::Process(UINT32 dwConnID) {}
void PtcM2C_IdipPunishInfoMsNtf::Process(UINT32 dwConnID) {}
void PtcM2C_IdipClearChatNtf::Process(UINT32 dwConnID) {}
void PtcM2D_SaveGardenInfo::Process(UINT32 dwConnID) {}
void PtcM2C_GuildCardRankNtf::Process(UINT32 dwConnID) {}
void PtcM2C_NoticeGuildLadderStart::Process(UINT32 dwConnID) {}
void PtcM2C_GuildCardMatchNtf::Process(UINT32 dwConnID) {}
void PtcM2G_GiveAnserQARewardNtf::Process(UINT32 dwConnID) {}
void PtcM2G_QAOverToGS::Process(UINT32 dwConnID) {}
void PtcM2T_BroadCastM2T::Process(UINT32 dwConnID) {}
void PtcM2C_SpriteStateChangeNtf::Process(UINT32 dwConnID) {}
void PtcM2G_GardenEventNtf::Process(UINT32 dwConnID) {}
void PtcM2C_InvUnfStateM2CNtf::Process(UINT32 dwConnID) {}
void PtcM2G_RoleEventSyncGS::Process(UINT32 dwConnID) {}
void PtcM2D_DelRoleNtf::Process(UINT32 dwConnID) {}
void PtcM2C_NoticeGuildArenaNextTime::Process(UINT32 dwConnID) {}
void PtcM2C_GardenBanquetNtf::Process(UINT32 dwConnID) {}
void PtcM2D_SavePayList::Process(UINT32 dwConnID) {}
void PtcM2C_NoticeGuildWageReward::Process(UINT32 dwConnID) {}
void PtcM2D_SavePlatFriendInfo::Process(UINT32 dwConnID) {}
void PtcM2C_SendGuildBonusNtf::Process(UINT32 dwConnID) {}
void PtcM2D_GuildAuctUpdateNtf::Process(UINT32 dwConnID) {}
void PtcM2C_ResWarGuildBriefNtf::Process(UINT32 dwConnID) {}
void PtcM2C_SynGuilIntegralState::Process(UINT32 dwConnID) {}
void PtcM2C_ResWarStateNtf::Process(UINT32 dwConnID) {}
void PtcM2C_SynGuildIntegralState::Process(UINT32 dwConnID) {}
void PtcM2G_syncservertime::Process(UINT32 dwConnID) {}
void PtcM2C_ResWarRankSimpleInfoNtf::Process(UINT32 dwConnID) {}
void PtcM2C_GuildBuffSimpleInfoNtf::Process(UINT32 dwConnID) {}
void PtcM2C_GuildBuffSimpleItemNtf::Process(UINT32 dwConnID) {}
void PtcM2G_WorldLevelNtf::Process(UINT32 dwConnID) {}
void PtcM2C_HallIconMNtf::Process(UINT32 dwConnID) {}
void PtcM2D_SavePlatNoticePtf::Process(UINT32 dwConnID) {}
void PtcM2C_ResWarMineDataNtf::Process(UINT32 dwConnID) {}
void PtcM2G_NoticeGuildInheritNumInfo::Process(UINT32 dwConnID) {}
void PtcM2C_LargeRoomRoleNtf::Process(UINT32 dwConnID) {}
void PtcM2C_LargeRoomLoginParamNtf::Process(UINT32 dwConnID) {}
void PtcM2F_LeaveFMLargeRoom::Process(UINT32 dwConnID) {}
void PtcM2C_ResWarTimeNtf::Process(UINT32 dwConnID) {}
void PtcM2F_FMShowNtf::Process(UINT32 dwConnID) {}
void PtcM2C_GuildBuffCDParamNtf::Process(UINT32 dwConnID) {}
void PtcM2G_MSDestoryScene::Process(UINT32 dwConnID) {}
void PtcM2C_NotifyMentorApply::Process(UINT32 dwConnID) {}
void PtcM2C_UpdatePartnerToClient::Process(UINT32 dwConnID) {}
void PtcM2G_UpdatePartnerToGS::Process(UINT32 dwConnID) {}
void PtcM2C_MakePartnerResultNtf::Process(UINT32 dwConnID) {}
void PtcM2C_InvFightNotify::Process(UINT32 dwConnID) {}
void PtcM2D_MDDelChatNtf::Process(UINT32 dwConnID) {}
void PtcM2D_SaveMentorInfoNtf::Process(UINT32 dwConnID) {}
void PtcM2G_GCFSynM2G::Process(UINT32 dwConnID) {}
void PtcM2C_PayParameterInfoInvalidNtf::Process(UINT32 dwConnID) {}
void PtcM2C_NoticeGuildTerrWar::Process(UINT32 dwConnID) {}
void PtcM2C_NoticeGuildTerrall::Process(UINT32 dwConnID) {}
void PtcM2G_TryTeacherInherit::Process(UINT32 dwConnID) {}
void PtcM2G_BalanceInfoNtf::Process(UINT32 dwConnID) {}
void PtcM2C_GuildBonusGetAll::Process(UINT32 dwConnID) {}
void PtcM2N_UpdateRegisterCount2Ns::Process(UINT32 dwConnID) {}
void PtcM2C_ResWarEnemyTimeNtf::Process(UINT32 dwConnID) {}
void PtcM2F_AudioChannelToFm::Process(UINT32 dwConnID) {}
void PtcM2C_UpdateLeagueBattleSeasonInfo::Process(UINT32 dwConnID) {}
void PtcM2C_NotifyLeagueTeamCreate::Process(UINT32 dwConnID) {}
void PtcM2C_NotifyLeagueTeamDissolve::Process(UINT32 dwConnID) {}
void PtcM2C_UpdateLeagueTeamState::Process(UINT32 dwConnID) {}
void PtcM2C_LeagueBattleStartMatchNtf::Process(UINT32 dwConnID) {}
void PtcM2C_LeagueBattleStopMatchNtf::Process(UINT32 dwConnID) {}
void PtcM2C_LeagueBattleMatchTimeoutNtf::Process(UINT32 dwConnID) {}
void PtcM2C_MidasExceptionNtf::Process(UINT32 dwConnID) {}
void PtcM2C_UpdateLeagueEleRoomStateNtf::Process(UINT32 dwConnID) {}
void PtcM2G_SynGuildBossHp::Process(UINT32 dwConnID) {}
void PtcM2G_SynGuildBossDie::Process(UINT32 dwConnID) {}
void PtcM2G_SynGuildBossDoEnd::Process(UINT32 dwConnID) {}
void PtcM2C_KickFMAuchor::Process(UINT32 dwConnID) {}
void PtcM2G_NoticeGuildBossAttr::Process(UINT32 dwConnID) {}
void PtcM2G_ControlFunctionNtf::Process(UINT32 dwConnID) {}
void PtcM2C_GuildAuctItemTimeFresh::Process(UINT32 dwConnID) {}
void PtcM2K_LeagueMatchWorldNtf::Process(UINT32 dwConnID) {}
void PtcM2C_PokerTournamentEndReFund::Process(UINT32 dwConnID) {}
void PtcM2G_ClickNoticeNtf::Process(UINT32 dwConnID) {}
void PtcM2K_HeroBattleWorldMatchNtf::Process(UINT32 dwConnID) {}
void PtcM2D_DeleteCrossRoleSummary::Process(UINT32 dwConnID) {}
void PtcM2C_NoticeGuildTerrEnd::Process(UINT32 dwConnID) {}
void PtcM2C_NoticeGuildTerrBattleWin::Process(UINT32 dwConnID) {}
void PtcM2C_NoticeGuildTerrBigIcon::Process(UINT32 dwConnID) {}
void PtcM2K_ReportRoleSummary2World::Process(UINT32 dwConnID) {}
void PtcM2K_GetRoleSummaryFromWorld::Process(UINT32 dwConnID) {}
void PtcM2G_MilitaryRankChange::Process(UINT32 dwConnID) {}
void PtcM2T_NotifyRouterInfo2Gate::Process(UINT32 dwConnID) {}
void PtcM2D_NotifyRouterInfo2DB::Process(UINT32 dwConnID) {}
void PtcM2N_NotifyRouterInfo2Ctrl::Process(UINT32 dwConnID) {}
void PtcM2G_BuyPayAileenNtf::Process(UINT32 dwConnID) {}
void PtcM2C_ModifyGuildNameNtf::Process(UINT32 dwConnID) {}
void PtcM2G_WorldBossGuildAddAttrSyncGsNtf::Process(UINT32 dwConnID) {}
void PtcM2S_M2SUserStatusSync::Process(UINT32 dwConnID) {}
void PtcM2G_EnterSceneFromMs::Process(UINT32 dwConnID) {}
void PtcM2G_BroadCastGlobalValue::Process(UINT32 dwConnID) {}
void PtcM2D_SavePayIbItem::Process(UINT32 dwConnID) {}
void PtcM2C_ArenaStarDataNtf::Process(UINT32 dwConnID) {}
void PtcM2C_NotifySkyTeamCreate::Process(UINT32 dwConnID) {}
void PtcM2C_SkyCraftMatchNtf::Process(UINT32 dwConnID) {}
void PtcM2C_SkyCraftEliRoomNtf::Process(UINT32 dwConnID) {}
void PtcM2C_IBGiftIconNtf::Process(UINT32 dwConnID) {}
void PtcM2C_GiftIBSucessNtf::Process(UINT32 dwConnID) {}
void PtcM2D_UpdateCustomBattleOnDB::Process(UINT32 dwConnID) {}
void PtcM2D_UpdateCustomBattleRoleOnDB::Process(UINT32 dwConnID) {}
void PtcM2K_ReportCustomBattleResultWorld::Process(UINT32 dwConnID) {}
void PtcM2K_CustomBattleDelMatch::Process(UINT32 dwConnID) {}
void PtcM2K_TransCustomBattleMatch::Process(UINT32 dwConnID) {}
void PtcM2C_TeamRequestPlatFreind2Client::Process(UINT32 dwConnID) {}
void PtcM2C_CustomBattleGMNotify::Process(UINT32 dwConnID) {}
void PtcM2C_TarjaBriefNtf::Process(UINT32 dwConnID) {}
void PtcM2G_PkUpdateSynNtf::Process(UINT32 dwConnID) {}
void PtcM2D_SaveMobaRoleRecord::Process(UINT32 dwConnID) {}
void PtcM2K_BMEnterReadySceneNtf::Process(UINT32 dwConnID) {}
void PtcM2G_AddMilitaryRank::Process(UINT32 dwConnID) {}
void PtcM2K_ServerCrossDataM2W::Process(UINT32 dwConnID) {}
void PtcM2H_TeamTranMs2Ts::Process(UINT32 dwConnID) {}
void PtcM2G_TeamTranMs2Gs::Process(UINT32 dwConnID) {}
void PtcM2N_M2NVersoinNotify::Process(UINT32 dwConnID) {}
void PtcM2C_GroupChatManager::Process(UINT32 dwConnID) {}
void PtcM2C_GroupChatQuit::Process(UINT32 dwConnID) {}
void PtcM2C_GroupChatDismiss::Process(UINT32 dwConnID) {}
void PtcM2C_WeddingInviteNtf::Process(UINT32 dwConnID) {}
void PtcM2D_SaveMarriageInfoNtf::Process(UINT32 dwConnID) {}
void PtcM2C_NotifyMarriageApply::Process(UINT32 dwConnID) {}
void PtcM2C_NotifyMarriageDivorceApply::Process(UINT32 dwConnID) {}
void PtcM2C_GroupChatIssueCount::Process(UINT32 dwConnID) {}
void PtcM2C_GroupChatApply::Process(UINT32 dwConnID) {}
void PtcM2D_DBGroupChatSaveGroupIssueNtf::Process(UINT32 dwConnID) {}
void PtcM2D_DBGroupChatDelGroupIssueNtf::Process(UINT32 dwConnID) {}
void PtcM2D_DBGroupChatSaveRoleIssueNtf::Process(UINT32 dwConnID) {}
void PtcM2D_DBGroupChatDelRoleIssueNtf::Process(UINT32 dwConnID) {}
void PtcM2D_DBGroupChatSaveGroupNtf::Process(UINT32 dwConnID) {}
void PtcM2D_DBGroupChatDelGroupNtf::Process(UINT32 dwConnID) {}
void PtcM2D_DBGroupChatDelRoleNtf::Process(UINT32 dwConnID) {}
void PtcM2D_DBGroupChatSaveRoleNtf::Process(UINT32 dwConnID) {}
void PtcM2G_WeddingCarEndM2G::Process(UINT32 dwConnID) {}
void PtcM2C_StartBattleFailedM2CNtf::Process(UINT32 dwConnID) {}
void PtcM2K_BattleFieldEventReq::Process(UINT32 dwConnID) {}
void PtcM2G_SynDragonGuildSimpleData::Process(UINT32 dwConnID) {}
void PtcM2C_LoginDragonGuildInfo::Process(UINT32 dwConnID) {}
void PtcM2C_DragonGuildSettingChanged::Process(UINT32 dwConnID) {}
void PtcM2D_UpdateDragonGuildData::Process(UINT32 dwConnID) {}
void PtcM2C_DragonGuildNameNtf::Process(UINT32 dwConnID) {}
void PtcM2G_NotifyDragonGuildRedPoint::Process(UINT32 dwConnID) {}
void PtcM2C_GoalAwardsRedPoint::Process(UINT32 dwConnID) {}
void PtcM2G_GoalAwardsServerAwards::Process(UINT32 dwConnID) {}
void PtcM2G_BattleFiedlReadySceneGsNtf::Process(UINT32 dwConnID) {}
void PtcM2D_DBGoalAwardsSaveNtf::Process(UINT32 dwConnID) {}
void PtcM2G_NpcFlNotifyM2G::Process(UINT32 dwConnID) {}
void PtcM2G_BattleFailInfoGsNtf::Process(UINT32 dwConnID) {}
void PtcM2C_DailyTaskEventNtf::Process(UINT32 dwConnID) {}
void PtcM2G_ConsumeRebateNtf::Process(UINT32 dwConnID) {}
void PtcM2C_TaskRefreshNtf::Process(UINT32 dwConnID) {}
void PtcM2C_GuildCastFeatsNtf::Process(UINT32 dwConnID) {}
void PtcM2G_PayTotalNtf::Process(UINT32 dwConnID) {}
void PtcM2K_PkRankLogSyn::Process(UINT32 dwConnID) {}

