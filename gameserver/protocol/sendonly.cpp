#include "pch.h"
#include "login/ptcg2c_notifyaccountdata.h"
#include "common/ptcg2d_registgameserver.h"
#include "scene/ptcg2c_enterscenentf.h"
#include "scene/ptcg2c_leavescenentf.h"
#include "scene/ptcg2c_unitappear.h"
#include "scene/ptcg2c_unitdisappear.h"
#include "common/ptcg2d_updatefield.h"
#include "skill/ptcg2c_skillresult.h"
#include "scene/ptcg2c_deathnotify.h"
#include "scene/ptcg2c_attributechangenotify.h"
#include "role/ptcg2c_levelchangentf.h"
#include "common/ptcg2c_itemchangedntf.h"
#include "role/ptcg2c_opensystemntf.h"
#include "battle/ptcg2c_battleresultntf.h"
#include "common/ptcg2c_completeachivement.h"
#include "skill/ptcg2c_skillchangedntf.h"
#include "common/ptcg2c_gserrornotify.h"
#include "common/ptcg2d_queryglobalconfig.h"
#include "scene/ptcg2c_changefashionnotify.h"
#include "checkin/ptcg2c_checkininfonotify.h"
#include "activity/ptcg2c_activityrolenotify.h"
#include "common/ptcg2t_opengate.h"
#include "pay/ptcg2c_updatepayinfo.h"
#include "reward/ptcg2c_rewardchangedntf.h"
#include "team/ptcg2c_leaveteam.h"
#include "battle/ptcg2c_revivenotify.h"
#include "pay/ptcg2c_updatebuygoldandfatigueinfo.h"
#include "battle/ptcg2c_revivecountdown.h"
#include "role/ptcg2c_fatiguerecovertimenotify.h"
#include "scene/ptcg2c_enterscenecooldownnotify.h"
#include "reward/ptcg2d_queryreward.h"
#include "reward/ptcg2d_removereward.h"
#include "reward/ptcg2d_addreward.h"
#include "log/ptcg2l_gslognotify.h"
#include "fashoin/ptcg2c_fashoinchangedntf.h"
#include "guild/ptcg2c_guildnotifymemberchanged.h"
#include "guild/ptcg2c_guildsettingchangednotify.h"
#include "guild/ptcg2d_loadguilddatareq.h"
#include "guild/ptcg2d_updateguild.h"
#include "guild/ptcg2c_guildcheckinboxntf.h"
#include "guild/ptcg2c_newguildbonusntf.h"
#include "guild/ptcg2c_guildbestcardsntf.h"
#include "scene/ptcg2c_executelevelscriptntf.h"
#include "reward/ptcg2c_onlinerewardntf.h"
#include "reward/ptcg2c_nextdayrewardntf.h"
#include "common/ptcg2c_hintnotify.h"
#include "scene/ptcg2c_positiondebug.h"
#include "battle/ptcg2c_enemydorpdoodadntf.h"
#include "task/ptcg2c_takerandomtask.h"
#include "battle/ptcg2c_buffnotify.h"
#include "battle/ptcg2c_teamselectnotify.h"
#include "team/ptcg2c_teaminvitenotify.h"
#include "battle/ptcg2c_fetchenemydoodadntf.h"
#include "pk/ptcg2c_pkpreparentf.h"
#include "pk/ptcg2c_pktimeoutntf.h"
#include "battle/ptcg2c_guildgoblinkillntf.h"
#include "scene/ptcg2c_scenefinishstatechanged.h"
#include "pk/ptcg2c_pkroleinfontf.h"
#include "common/ptcg2c_itemcircledrawresult.h"
#include "scene/ptcg2c_scenedamagerankntf.h"
#include "battle/ptcg2c_sceneleftdoodad.h"
#include "item/ptcg2c_changesupplementntf.h"
#include "common/ptcg2w_registgameservertowebserver.h"
#include "action/ptcg2c_correctposition.h"
#include "common/ptcg2c_teleportnotice.h"
#include "scene/ptcg2c_allymatchroleidnotify.h"
#include "team/ptcg2c_teamfulldatantf.h"
#include "pet/ptcg2c_petchangenotfiy.h"
#include "findback/ptcg2c_expfindbackntf.h"
#include "tower/ptcg2c_towersceneinfontf.h"
#include "designation/ptcg2c_coverdesignationntf.h"
#include "achievement/ptcg2c_reachachieventf.h"
#include "designation/ptcg2c_reachdesignationntf.h"
#include "levelseal/ptcg2c_levelsealntf.h"
#include "common/ptcg2c_reconnectsyncnotify.h"
#include "common/ptcg2c_outlookchangentf.h"
#include "reward/ptcg2c_fivedayrewardntf.h"
#include "action/ptcg2c_syncstepnotify.h"
#include "tsssdk/ptcg2c_tsssdkantidatantf.h"
#include "qa/ptcg2m_gmcloseqaroom.h"
#include "qa/ptcg2c_qanotify.h"
#include "pvp/ptcg2c_pvpbattlebeginntf.h"
#include "pvp/ptcg2c_pvpbattleendntf.h"
#include "pvp/ptcg2c_pvpbattlekill.h"
#include "social/ptcg2c_flowerrankrewardntf.h"
#include "team/ptcg2c_fastmbconfirmntf.h"
#include "team/ptcg2c_fastmbdismissntf.h"
#include "role/ptcg2c_selectrolentf.h"
#include "common/ptcg2t_changegsline.h"
#include "social/ptcg2c_receiveflowerntf.h"
#include "social/ptcg2c_flowerrainntf.h"
#include "team/ptcg2c_startbattlefailedntf.h"
#include "battlewatch/ptcg2c_watchbattleinfontf.h"
#include "guild/ptcg2c_gmfjoinbattlereq.h"
#include "guild/ptcg2c_gmfonebattleendntf.h"
#include "guild/ptcg2c_gmfbasedatantf.h"
#include "chat/ptcg2c_worldchannellefttimesntf.h"
#include "guild/ptcg2c_noticeguildbossend.h"
#include "guild/ptcg2c_updateguildarenastate.h"
#include "guild/ptcg2c_guildbosstimeout.h"
#include "role/ptcg2m_updaterolesummary.h"
#include "guild/ptcg2c_synguildarenabattleinfo.h"
#include "guild/ptcg2c_synguildarenafightunit.h"
#include "activity/ptcg2c_mulactivitystatechange.h"
#include "guild/ptcg2c_gmfallfightendntf.h"
#include "worldboss/ptcg2c_worldbossstatentf.h"
#include "battlewatch/ptcg2c_notifyroleempty2watcher.h"
#include "scene/ptcg2c_battlewatcherntf.h"
#include "guild/ptcg2c_noticeguildarenabegin.h"
#include "guild/ptcg2c_notifyguildbossaddattr.h"
#include "battlewatch/ptcg2c_notifywatchdata.h"
#include "scene/ptcg2m_scenedestroyed.h"
#include "battle/ptcg2c_notifycliententerfight.h"
#include "chat/ptcg2m_noticeforwardntf.h"
#include "battlewatch/ptcg2m_syncliveinfotoms.h"
#include "guild/ptcg2c_synguildarenaroleonline.h"
#include "battle/ptcg2c_clientonlybuffnotify.h"
#include "guild/ptcg2m_rolesfinishscenetomaster.h"
#include "scene/ptcg2m_sceneendtomaster.h"
#include "guildcamp/ptcg2m_updateguildcampresult.h"
#include "guild/ptcg2c_synguildarenadisplace.h"
#include "chat/ptcg2m_reportwatchntf.h"
#include "battle/ptcg2c_ontransferwall.h"
#include "levelseal/ptcg2m_gmlevelsealinfo.h"
#include "mail/ptcg2m_addmailgs.h"
#include "battlewatch/ptcg2c_notifywatchiconnum2client.h"
#include "guild/ptcg2c_gmfwaitotherload.h"
#include "guild/ptcg2c_gmfwaitfightbegin.h"
#include "chat/ptcg2m_gmsendchat.h"
#include "guild/ptcg2m_addguildbossattr.h"
#include "common/ptcg2m_firstenterscenentf.h"
#include "guild/ptcg2m_gmsetguildbosstime.h"
#include "scene/ptcg2m_synscenerewardg2m.h"
#include "guild/ptcg2m_noticeguildbossdie.h"
#include "guild/ptcg2m_createguildbosssingle.h"
#include "worldboss/ptcg2m_worldbosssyncms.h"
#include "worldboss/ptcg2c_worldbossattrntf.h"
#include "chat/ptcg2c_joinroomreply.h"
#include "chat/ptcg2c_updatevoiproommemberntf.h"
#include "pay/ptcg2c_payallinfontf.h"
#include "reward/ptcg2c_loginreward2cntf.h"
#include "common/ptcg2m_reloadversionntf.h"
#include "battle/ptcg2c_aidebuginfo.h"
#include "battle/ptcg2c_startrollntf.h"
#include "pet/ptcg2c_noticehungrydown.h"
#include "firstpass/ptcg2c_firstpassrewardntf.h"
#include "team/ptcg2m_teameventnotify.h"
#include "title/ptcg2c_titlechangenotify.h"
#include "findback/ptcg2c_itemfindbackntf.h"
#include "sprite/ptcg2c_spritechangedntf.h"
#include "battle/ptcg2c_fightgroupchangentf.h"
#include "stage/ptcg2c_bossrushonefinishntf.h"
#include "atlas/ptcg2c_synatlasattr.h"
#include "superrisk/ptcg2c_playdicentf.h"
#include "stage/ptcg2m_deprogresssyn.h"
#include "ibshop/ptcg2c_ibshopicon.h"
#include "enhance/ptcg2c_notifyenhancesuit.h"
#include "superrisk/ptcg2c_riskbuyntf.h"
#include "superrisk/ptcg2c_riskrecoverdicentf.h"
#include "title/ptcg2m_getmaxtitlentf.h"
#include "guild/ptcg2m_reqguildsimpledata.h"
#include "guild/ptcg2m_synguildbosshurt.h"
#include "guild/ptcg2m_synaddguildactivity.h"
#include "tower/ptcg2c_towerfirstpassrewardntf.h"
#include "guild/ptcg2m_gmfupdatedatag2m.h"
#include "guild/ptcg2m_synguildcontribute.h"
#include "activity/ptcg2c_halliconsntf.h"
#include "task/ptcg2c_updatetaskstatus.h"
#include "skycity/ptcg2c_skycityestimateres.h"
#include "skycity/ptcg2c_skycitytimeres.h"
#include "common/ptcg2c_notifyidipmessagegs.h"
#include "common/ptcg2c_idippunishinfontf.h"
#include "skycity/ptcg2c_skycityteamres.h"
#include "guild/ptcg2m_notifyscenepkwin.h"
#include "scene/ptcg2c_gmfkicknty.h"
#include "skycity/ptcg2m_skycityteamresultntf.h"
#include "scene/ptcg2c_skycitybattledatantf.h"
#include "scene/ptcg2c_gmfbattlestatentf.h"
#include "broadcast/ptcg2m_broadcast.h"
#include "spactivity/ptcg2c_spactivitychangentf.h"
#include "spactivity/ptcg2c_spactivityoffsetdayntf.h"
#include "battle/ptcg2c_dpsnotify.h"
#include "skill/ptcg2c_skillcoolntf.h"
#include "garden/ptcg2c_gardenplanteventnotice.h"
#include "garden/ptcg2c_gardenbanquetnotice.h"
#include "guild/ptcg2c_gmfguildcombatntf.h"
#include "guild/ptcg2c_gpronebattleendntf.h"
#include "guild/ptcg2c_gprallfightendntf.h"
#include "scene/ptcg2m_scenecreated.h"
#include "login/ptcg2m_rolelogin.h"
#include "login/ptcg2m_rolelogout.h"
#include "scene/ptcg2m_rolechangescenentf.h"
#include "scene/ptcg2m_enterscenereqtoms.h"
#include "reswar/ptcg2m_reswarteamresultntf.h"
#include "reswar/ptcg2c_reswarbattledatantf.h"
#include "crossscene/ptcg2k_roleentercrossscenentf.h"
#include "crossscene/ptcg2k_crossscenedestoryed.h"
#include "reswar/ptcg2c_reswarteamresone.h"
#include "pay/ptcg2c_paymemberprivilegentf.h"
#include "battle/ptcg2c_killenemyscorentf.h"
#include "common/ptcg2c_serveropendayntf.h"
#include "scene/ptcg2n_scenedestroyedtons.h"
#include "guild/ptcg2c_synguildinheritexp.h"
#include "guild/ptcg2c_noticedoingguildinherit.h"
#include "guild/ptcg2c_synguildinheritnuminfo.h"
#include "skill/ptcg2c_cdcall.h"
#include "guild/ptcg2m_noticeguildinheritend.h"
#include "common/ptcg2m_mscrashrecoverg2m.h"
#include "battlewatch/ptcg2m_syncallliveinfotoms.h"
#include "common/ptcg2m_enterhallnotice.h"
#include "action/ptcg2c_syncmovenotify.h"
#include "role/ptcg2c_changenamecountntf.h"
#include "scene/ptcg2m_sceneemptynotify.h"
#include "common/ptcg2c_antiaddictionremindntf.h"
#include "horserace/ptcg2c_horseawardallntf.h"
#include "horserank/ptcg2c_horserankntf.h"
#include "horserace/ptcg2c_horsewaittimentf.h"
#include "horserace/ptcg2c_horseanimationntf.h"
#include "task/ptcg2m_removeaskitemntf.h"
#include "horse/ptcg2c_horsefinalntf.h"
#include "partner/ptcg2m_addpartnerlivenessg2m.h"
#include "horse/ptcg2c_horsecountdowntimentf.h"
#include "battle/ptcg2c_doodaditemaddntf.h"
#include "guilfcastle/ptcg2c_gcfzhanlingnotify.h"
#include "invfight/ptcg2c_invfightbefenterscenentf.h"
#include "herobattle/ptcg2c_herobattledatantf.h"
#include "scene/ptcg2c_sceneprepareinfontf.h"
#include "herobattle/ptcg2c_herobattleteammsgntf.h"
#include "herobattle/ptcg2c_herobattlesyncntf.h"
#include "herobattle/ptcg2c_herobattleincirclentf.h"
#include "guildcastle/ptcg2m_gcfsyng2m.h"
#include "herobattle/ptcg2c_herobattleovertime.h"
#include "herobattle/ptcg2c_herobattleteamrolentf.h"
#include "guildcastle/ptcg2c_gcfsyng2cntf.h"
#include "pay/ptcg2m_payadddiamondntf.h"
#include "pay/ptcg2m_payconsumentf.h"
#include "battle/ptcg2c_doodaditemusentf.h"
#include "battle/ptcg2c_specialstatentf.h"
#include "herobattle/ptcg2c_closechooseherontf.h"
#include "common/ptcg2r_broadcastg2r.h"
#include "battle/ptcg2c_levelscriptstatentf.h"
#include "scene/ptcg2c_scenestatentf.h"
#include "role/ptcg2c_queryrolestateack.h"
#include "ibshop/ptcg2c_ibshophasbuyntf.h"
#include "role/ptcg2c_skillinitcoolntf.h"
#include "herobattle/ptcg2c_herobattlecanusehero.h"
#include "leaguebattle/ptcg2c_leaguebattleloadinfontf.h"
#include "leaguebattle/ptcg2c_leaguebattlebasedatantf.h"
#include "leaguebattle/ptcg2c_leaguebattleoneresultnft.h"
#include "leaguebattle/ptcg2c_leaguebattleresultntf.h"
#include "pandora/ptcg2c_pushpraisentf.h"
#include "herobattle/ptcg2c_herobattletipsntf.h"
#include "leaguebattle/ptcg2c_leaguebattlestatentf.h"
#include "leaguebattle/ptcg2m_leaguebattleupdatedatag2m.h"
#include "leaguebattle/ptcg2m_leagueeliminationupdateg2m.h"
#include "guild/ptcg2m_synguildbossattr.h"
#include "role/ptcg2m_levelchangentfms.h"
#include "leaguebattle/ptcg2k_leagueeliminationupdateg2k.h"
#include "role/ptcg2m_opensystemntfms.h"
#include "broadcast/ptcg2t_broadcastg2t.h"
#include "common/ptcg2c_countdownntf.h"
#include "pay/ptcg2c_allgiftibitemntf.h"
#include "pay/ptcg2c_giftibbackinfontf.h"
#include "lottery/ptcg2c_sharerandomgiftntf.h"
#include "worldboss/ptcg2c_worldbossguildaddattrsyncclientntf.h"
#include "common/ptcg2m_updateglobalvalue.h"
#include "militaryrank/ptcg2c_militaryrankntf.h"
#include "loginactivity/ptcg2c_loginactivitystatusntf.h"
#include "fashion/ptcg2c_displayadditem.h"
#include "fashion/ptcg2c_updatedisplayitems.h"
#include "guild/ptcg2c_guildcamppartynotify.h"
#include "absparty/ptcg2c_abspartyntf.h"
#include "guild/ptcg2c_guildcamppartytradenotify.h"
#include "custombattle/ptcg2m_reportcustombattleresult.h"
#include "battle/ptcg2m_reportbadplayer2ms.h"
#include "custombattle/ptcg2c_custombattleloadingntf.h"
#include "team/ptcg2m_tarjainfontf.h"
#include "team/ptcg2m_sceneteamresultntf.h"
#include "team/ptcg2m_teamcostleaderresetntf.h"
#include "platformshareresult/ptcg2c_platformshareawardntf.h"
#include "weekend4v4/ptcg2c_weekend4v4roledatantf.h"
#include "mobabattle/ptcg2c_herokillnotify.h"
#include "mobabattle/ptcg2c_mobabattleteamrolentf.h"
#include "mobabattle/ptcg2c_mobarolechangentf.h"
#include "mobabattle/ptcg2c_mobabattleteammsgntf.h"
#include "skill/ptcg2c_transskillnotfiy.h"
#include "battle/ptcg2c_entitytargetchangentf.h"
#include "mobabattle/ptcg2c_mobasignalbroadcast.h"
#include "pet/ptcg2c_petinvitentf.h"
#include "mobabattle/ptcg2m_addmobabattlerecord.h"
#include "chat/ptcg2c_chatnotifyg2c.h"
#include "mobabattle/ptcg2c_mobaminimapntf.h"
#include "session/ptcg2m_clientsessionchangeg2m.h"
#include "login/ptcg2m_roleloginreconnectntf.h"
#include "mobabattle/ptcg2c_mobaaddexpntf.h"
#include "bigmelee/ptcg2c_bmreadytimentf.h"
#include "bigmelee/ptcg2k_bmscenefightinfontf.h"
#include "bigmelee/ptcg2c_bmfighttimentf.h"
#include "bigmelee/ptcg2c_bmrolescenesyncntf.h"
#include "bigmelee/ptcg2c_bigmeleereliventf.h"
#include "bigmelee/ptcg2k_bmleavefightscenentf.h"
#include "scene/ptcg2c_mobahintntf.h"
#include "team/ptcg2m_teamtrangs2ms.h"
#include "tajie/ptcg2c_tajiehelpnotify.h"
#include "common/ptcg2c_notifystartuptypetoclient.h"
#include "herobattle/ptcg2c_herobattleancientpower.h"
#include "battle/ptcg2c_battlestatisticsntf.h"
#include "wedding/ptcg2c_weddingloadinfontf.h"
#include "wedding/ptcg2c_weddingstatentf.h"
#include "wedding/ptcg2c_weddingeventntf.h"
#include "horse/ptcg2c_horsefailtipsntf.h"
#include "skill/ptcg2c_warningrandomntf.h"
#include "wedding/ptcg2m_updateweddingstateg2m.h"
#include "spactivity/ptcg2c_themeactivitychangentf.h"
#include "crossscene/ptcg2k_roleleavecrossscenentf.h"
#include "wedding/ptcg2c_weddingcarntf.h"
#include "spactivity/ptcg2c_backflowopenntf.h"
#include "chat/ptcg2m_privatechat.h"
#include "transform/ptcg2c_transnotify.h"
#include "invfight/ptcg2m_invfightagainreqg2m.h"
#include "invfight/ptcg2k_invfightagainreqg2w.h"
#include "dragonguild/ptcg2m_reqdragonguildsimpledata.h"
#include "dragonguild/ptcg2m_costdragonguildreceivecount.h"
#include "dragonguild/ptcg2m_checkdragonguildtask.h"
#include "battlefield/ptcg2c_battlefieldreadyinfontf.h"
#include "battlefield/ptcg2c_battlefieldreliventf.h"
#include "battlefield/ptcg2c_battlefieldroleagainstntf.h"
#include "goalawards/ptcg2m_goalawardsserverevent.h"
#include "battlefield/ptcg2c_bffighttimentf.h"
#include "pay/ptcg2c_payscorentf.h"
#include "goalawards/ptcg2m_goalawardsserverteam.h"
#include "npcfeeling/ptcg2c_npcflntf.h"
#include "battle/ptcg2c_doodaditemskillsntf.h"
#include "notice/ptcg2c_wordnotify.h"
#include "bigmelee/ptcg2c_bigmeleepointntf.h"
#include "bigmelee/ptcg2c_bigmeleepointoutlookntf.h"
#include "kickaccount/ptcg2c_kickaccountjkydntf.h"
#include "kickaccount/ptcg2m_kickaccountjkydg2m.h"
#include "common/ptcg2c_worldlevelntf2client.h"
#include "stage/ptcg2c_updatestageinfontf.h"

void PtcG2C_NotifyAccountData::Process(UINT32 dwConnID) {}
void PtcG2D_RegistGameServer::Process(UINT32 dwConnID) {}
void PtcG2C_EnterSceneNtf::Process(UINT32 dwConnID) {}
void PtcG2C_LeaveSceneNtf::Process(UINT32 dwConnID) {}
void PtcG2C_UnitAppear::Process(UINT32 dwConnID) {}
void PtcG2C_UnitDisappear::Process(UINT32 dwConnID) {}
void PtcG2D_UpdateField::Process(UINT32 dwConnID) {}
void PtcG2C_SkillResult::Process(UINT32 dwConnID) {}
void PtcG2C_DeathNotify::Process(UINT32 dwConnID) {}
void PtcG2C_AttributeChangeNotify::Process(UINT32 dwConnID) {}
void PtcG2C_LevelChangeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ItemChangedNtf::Process(UINT32 dwConnID) {}
void PtcG2C_OpenSystemNtf::Process(UINT32 dwConnID) {}
void PtcG2C_BattleResultNtf::Process(UINT32 dwConnID) {}
void PtcG2C_CompleteAchivement::Process(UINT32 dwConnID) {}
void PtcG2C_SkillChangedNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GSErrorNotify::Process(UINT32 dwConnID) {}
void PtcG2D_QueryGlobalConfig::Process(UINT32 dwConnID) {}
void PtcG2C_ChangeFashionNotify::Process(UINT32 dwConnID) {}
void PtcG2C_CheckinInfoNotify::Process(UINT32 dwConnID) {}
void PtcG2C_ActivityRoleNotify::Process(UINT32 dwConnID) {}
void PtcG2T_OpenGate::Process(UINT32 dwConnID) {}
void PtcG2C_UpdatePayInfo::Process(UINT32 dwConnID) {}
void PtcG2C_RewardChangedNtf::Process(UINT32 dwConnID) {}
void PtcG2C_LeaveTeam::Process(UINT32 dwConnID) {}
void PtcG2C_ReviveNotify::Process(UINT32 dwConnID) {}
void PtcG2C_UpdateBuyGoldAndFatigueInfo::Process(UINT32 dwConnID) {}
void PtcG2C_ReviveCountdown::Process(UINT32 dwConnID) {}
void PtcG2C_FatigueRecoverTimeNotify::Process(UINT32 dwConnID) {}
void PtcG2C_EnterSceneCoolDownNotify::Process(UINT32 dwConnID) {}
void PtcG2D_QueryReward::Process(UINT32 dwConnID) {}
void PtcG2D_RemoveReward::Process(UINT32 dwConnID) {}
void PtcG2D_AddReward::Process(UINT32 dwConnID) {}
void PtcG2L_GSLogNotify::Process(UINT32 dwConnID) {}
void PtcG2C_FashoinChangedNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GuildNotifyMemberChanged::Process(UINT32 dwConnID) {}
void PtcG2C_GuildSettingChangedNotify::Process(UINT32 dwConnID) {}
void PtcG2D_LoadGuildDataReq::Process(UINT32 dwConnID) {}
void PtcG2D_UpdateGuild::Process(UINT32 dwConnID) {}
void PtcG2C_GuildCheckinBoxNtf::Process(UINT32 dwConnID) {}
void PtcG2C_NewGuildBonusNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GuildBestCardsNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ExecuteLevelScriptNtf::Process(UINT32 dwConnID) {}
void PtcG2C_OnlineRewardNtf::Process(UINT32 dwConnID) {}
void PtcG2C_NextDayRewardNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HintNotify::Process(UINT32 dwConnID) {}
void PtcG2C_PositionDebug::Process(UINT32 dwConnID) {}
void PtcG2C_EnemyDorpDoodadNtf::Process(UINT32 dwConnID) {}
void PtcG2C_TakeRandomTask::Process(UINT32 dwConnID) {}
void PtcG2C_BuffNotify::Process(UINT32 dwConnID) {}
void PtcG2C_TeamSelectNotify::Process(UINT32 dwConnID) {}
void PtcG2C_TeamInviteNotify::Process(UINT32 dwConnID) {}
void PtcG2C_FetchEnemyDoodadNtf::Process(UINT32 dwConnID) {}
void PtcG2C_PkPrepareNtf::Process(UINT32 dwConnID) {}
void PtcG2C_PkTimeoutNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GuildGoblinKillNtf::Process(UINT32 dwConnID) {}
void PtcG2C_SceneFinishStateChanged::Process(UINT32 dwConnID) {}
void PtcG2C_PkRoleInfoNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ItemCircleDrawResult::Process(UINT32 dwConnID) {}
void PtcG2C_SceneDamageRankNtf::Process(UINT32 dwConnID) {}
void PtcG2C_SceneLeftDoodad::Process(UINT32 dwConnID) {}
void PtcG2C_ChangeSupplementNtf::Process(UINT32 dwConnID) {}
void PtcG2W_RegistGameServerToWebServer::Process(UINT32 dwConnID) {}
void PtcG2C_CorrectPosition::Process(UINT32 dwConnID) {}
void PtcG2C_TeleportNotice::Process(UINT32 dwConnID) {}
void PtcG2C_AllyMatchRoleIDNotify::Process(UINT32 dwConnID) {}
void PtcG2C_TeamFullDataNtf::Process(UINT32 dwConnID) {}
void PtcG2C_PetChangeNotfiy::Process(UINT32 dwConnID) {}
void PtcG2C_ExpFindBackNtf::Process(UINT32 dwConnID) {}
void PtcG2C_TowerSceneInfoNtf::Process(UINT32 dwConnID) {}
void PtcG2C_CoverDesignationNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ReachAchieveNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ReachDesignationNtf::Process(UINT32 dwConnID) {}
void PtcG2C_LevelSealNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ReconnectSyncNotify::Process(UINT32 dwConnID) {}
void PtcG2C_OutLookChangeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_FiveDayRewardNTF::Process(UINT32 dwConnID) {}
void PtcG2C_SyncStepNotify::Process(UINT32 dwConnID) {}
void PtcG2C_TssSdkAntiDataNtf::Process(UINT32 dwConnID) {}
void PtcG2M_GMCloseQARoom::Process(UINT32 dwConnID) {}
void PtcG2C_QANotify::Process(UINT32 dwConnID) {}
void PtcG2C_PvpBattleBeginNtf::Process(UINT32 dwConnID) {}
void PtcG2C_PvpBattleEndNtf::Process(UINT32 dwConnID) {}
void PtcG2C_PvpBattleKill::Process(UINT32 dwConnID) {}
void PtcG2C_FlowerRankRewardNtf::Process(UINT32 dwConnID) {}
void PtcG2C_FastMBConfirmNtf::Process(UINT32 dwConnID) {}
void PtcG2C_fastMBDismissNtf::Process(UINT32 dwConnID) {}
void PtcG2C_SelectRoleNtf::Process(UINT32 dwConnID) {}
void PtcG2T_ChangeGsLine::Process(UINT32 dwConnID) {}
void PtcG2C_ReceiveFlowerNtf::Process(UINT32 dwConnID) {}
void PtcG2C_FlowerRainNtf::Process(UINT32 dwConnID) {}
void PtcG2C_StartBattleFailedNtf::Process(UINT32 dwConnID) {}
void PtcG2C_WatchBattleInfoNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GmfJoinBattleReq::Process(UINT32 dwConnID) {}
void PtcG2C_GmfOneBattleEndNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GmfBaseDataNtf::Process(UINT32 dwConnID) {}
void PtcG2C_WorldChannelLeftTimesNtf::Process(UINT32 dwConnID) {}
void PtcG2C_NoticeGuildBossEnd::Process(UINT32 dwConnID) {}
void PtcG2C_UpdateGuildArenaState::Process(UINT32 dwConnID) {}
void PtcG2C_GuildBossTimeOut::Process(UINT32 dwConnID) {}
void PtcG2M_UpdateRoleSummary::Process(UINT32 dwConnID) {}
void PtcG2C_SynGuildArenaBattleInfo::Process(UINT32 dwConnID) {}
void PtcG2C_SynGuildArenaFightUnit::Process(UINT32 dwConnID) {}
void PtcG2C_MulActivityStateChange::Process(UINT32 dwConnID) {}
void PtcG2C_GmfAllFightEndNtf::Process(UINT32 dwConnID) {}
void PtcG2C_WorldBossStateNtf::Process(UINT32 dwConnID) {}
void PtcG2C_NotifyRoleEmpty2Watcher::Process(UINT32 dwConnID) {}
void PtcG2C_BattleWatcherNtf::Process(UINT32 dwConnID) {}
void PtcG2C_NoticeGuildArenaBegin::Process(UINT32 dwConnID) {}
void PtcG2C_NotifyGuildBossAddAttr::Process(UINT32 dwConnID) {}
void PtcG2C_NotifyWatchData::Process(UINT32 dwConnID) {}
void PtcG2M_SceneDestroyed::Process(UINT32 dwConnID) {}
void PtcG2C_NotifyClientEnterFight::Process(UINT32 dwConnID) {}
void PtcG2M_NoticeForwardNtf::Process(UINT32 dwConnID) {}
void PtcG2M_SyncLiveInfoToMs::Process(UINT32 dwConnID) {}
void PtcG2C_SynGuildArenaRoleOnline::Process(UINT32 dwConnID) {}
void PtcG2C_ClientOnlyBuffNotify::Process(UINT32 dwConnID) {}
void PtcG2M_RolesFinishSceneToMaster::Process(UINT32 dwConnID) {}
void PtcG2M_SceneEndToMaster::Process(UINT32 dwConnID) {}
void PtcG2M_UpdateGuildCampResult::Process(UINT32 dwConnID) {}
void PtcG2C_synguildarenadisplace::Process(UINT32 dwConnID) {}
void PtcG2M_ReportWatchNtf::Process(UINT32 dwConnID) {}
void PtcG2C_OnTransferWall::Process(UINT32 dwConnID) {}
void PtcG2M_GmLevelSealInfo::Process(UINT32 dwConnID) {}
void PtcG2M_AddMailGS::Process(UINT32 dwConnID) {}
void PtcG2C_NotifyWatchIconNum2Client::Process(UINT32 dwConnID) {}
void PtcG2C_GmfWaitOtherLoad::Process(UINT32 dwConnID) {}
void PtcG2C_GmfWaitFightBegin::Process(UINT32 dwConnID) {}
void PtcG2M_GMSendChat::Process(UINT32 dwConnID) {}
void PtcG2M_AddGuildBossAttr::Process(UINT32 dwConnID) {}
void PtcG2M_FirstEnterSceneNtf::Process(UINT32 dwConnID) {}
void PtcG2M_GmsetGuildBossTime::Process(UINT32 dwConnID) {}
void PtcG2M_SynSceneRewardG2M::Process(UINT32 dwConnID) {}
void PtcG2M_NoticeGuildBossDie::Process(UINT32 dwConnID) {}
void PtcG2M_CreateGuildBossSingle::Process(UINT32 dwConnID) {}
void PtcG2M_WorldBossSyncMS::Process(UINT32 dwConnID) {}
void PtcG2C_WorldBossAttrNtf::Process(UINT32 dwConnID) {}
void PtcG2C_JoinRoomReply::Process(UINT32 dwConnID) {}
void PtcG2C_UpdateVoipRoomMemberNtf::Process(UINT32 dwConnID) {}
void PtcG2C_PayAllInfoNtf::Process(UINT32 dwConnID) {}
void PtcG2C_LoginReward2CNtf::Process(UINT32 dwConnID) {}
void PtcG2M_ReloadVersionNtf::Process(UINT32 dwConnID) {}
void PtcG2C_AIDebugInfo::Process(UINT32 dwConnID) {}
void PtcG2C_StartRollNtf::Process(UINT32 dwConnID) {}
void PtcG2C_NoticeHungryDown::Process(UINT32 dwConnID) {}
void PtcG2C_FirstPassRewardNtf::Process(UINT32 dwConnID) {}
void PtcG2M_TeamEventNotify::Process(UINT32 dwConnID) {}
void PtcG2C_TitleChangeNotify::Process(UINT32 dwConnID) {}
void PtcG2C_ItemFindBackNtf::Process(UINT32 dwConnID) {}
void PtcG2C_SpriteChangedNtf::Process(UINT32 dwConnID) {}
void PtcG2C_FightGroupChangeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_BossRushOneFinishNtf::Process(UINT32 dwConnID) {}
void PtcG2C_SynAtlasAttr::Process(UINT32 dwConnID) {}
void PtcG2C_PlayDiceNtf::Process(UINT32 dwConnID) {}
void PtcG2M_DEProgressSyn::Process(UINT32 dwConnID) {}
void PtcG2C_IBShopIcon::Process(UINT32 dwConnID) {}
void PtcG2C_NotifyEnhanceSuit::Process(UINT32 dwConnID) {}
void PtcG2C_RiskBuyNtf::Process(UINT32 dwConnID) {}
void PtcG2C_RiskRecoverDiceNtf::Process(UINT32 dwConnID) {}
void PtcG2M_GetMaxTitleNtf::Process(UINT32 dwConnID) {}
void PtcG2M_ReqGuildSimpleData::Process(UINT32 dwConnID) {}
void PtcG2M_SynGuildBossHurt::Process(UINT32 dwConnID) {}
void PtcG2M_SynAddGuildActivity::Process(UINT32 dwConnID) {}
void PtcG2C_TowerFirstPassRewardNtf::Process(UINT32 dwConnID) {}
void PtcG2M_GmfUpdateDataG2M::Process(UINT32 dwConnID) {}
void PtcG2M_SynGuildContribute::Process(UINT32 dwConnID) {}
void PtcG2C_HallIconSNtf::Process(UINT32 dwConnID) {}
void PtcG2C_UpdateTaskStatus::Process(UINT32 dwConnID) {}
void PtcG2C_SkyCityEstimateRes::Process(UINT32 dwConnID) {}
void PtcG2C_SkyCityTimeRes::Process(UINT32 dwConnID) {}
void PtcG2C_NotifyIdipMessageGs::Process(UINT32 dwConnID) {}
void PtcG2C_IdipPunishInfoNtf::Process(UINT32 dwConnID) {}
void PtcG2C_SkyCityTeamRes::Process(UINT32 dwConnID) {}
void PtcG2M_NotifyScenePkWin::Process(UINT32 dwConnID) {}
void PtcG2C_GmfKickNty::Process(UINT32 dwConnID) {}
void PtcG2M_SkyCityTeamResultNtf::Process(UINT32 dwConnID) {}
void PtcG2C_SkyCityBattleDataNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GmfBattleStateNtf::Process(UINT32 dwConnID) {}
void PtcG2M_BroadCast::Process(UINT32 dwConnID) {}
void PtcG2C_SpActivityChangeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_SpActivityOffsetDayNtf::Process(UINT32 dwConnID) {}
void PtcG2C_DPSNotify::Process(UINT32 dwConnID) {}
void PtcG2C_SkillCoolNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GardenPlantEventNotice::Process(UINT32 dwConnID) {}
void PtcG2C_GardenBanquetNotice::Process(UINT32 dwConnID) {}
void PtcG2C_GmfGuildCombatNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GprOneBattleEndNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GprAllFightEndNtf::Process(UINT32 dwConnID) {}
void PtcG2M_SceneCreated::Process(UINT32 dwConnID) {}
void PtcG2M_RoleLogin::Process(UINT32 dwConnID) {}
void PtcG2M_RoleLogout::Process(UINT32 dwConnID) {}
void PtcG2M_RoleChangeSceneNtf::Process(UINT32 dwConnID) {}
void PtcG2M_EnterSceneReqToMs::Process(UINT32 dwConnID) {}
void PtcG2M_ResWarTeamResultNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ResWarBattleDataNtf::Process(UINT32 dwConnID) {}
void PtcG2K_RoleEnterCrossSceneNtf::Process(UINT32 dwConnID) {}
void PtcG2K_CrossSceneDestoryed::Process(UINT32 dwConnID) {}
void PtcG2C_ResWarTeamResOne::Process(UINT32 dwConnID) {}
void PtcG2C_PayMemberPrivilegeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_KillEnemyScoreNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ServerOpenDayNtf::Process(UINT32 dwConnID) {}
void PtcG2N_SceneDestroyedToNs::Process(UINT32 dwConnID) {}
void PtcG2C_synGuildInheritExp::Process(UINT32 dwConnID) {}
void PtcG2C_NoticeDoingGuildInherit::Process(UINT32 dwConnID) {}
void PtcG2C_SynGuildInheritNumInfo::Process(UINT32 dwConnID) {}
void PtcG2C_CdCall::Process(UINT32 dwConnID) {}
void PtcG2M_NoticeGuildInheritEnd::Process(UINT32 dwConnID) {}
void PtcG2M_mscrashrecoverg2m::Process(UINT32 dwConnID) {}
void PtcG2M_SyncAllLiveInfoToMs::Process(UINT32 dwConnID) {}
void PtcG2M_EnterHallNotice::Process(UINT32 dwConnID) {}
void PtcG2C_SyncMoveNotify::Process(UINT32 dwConnID) {}
void PtcG2C_ChangeNameCountNtf::Process(UINT32 dwConnID) {}
void PtcG2M_SceneEmptyNotify::Process(UINT32 dwConnID) {}
void PtcG2C_AntiAddictionRemindNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HorseAwardAllNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HorseRankNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HorseWaitTimeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HorseAnimationNtf::Process(UINT32 dwConnID) {}
void PtcG2M_RemoveAskItemNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HorseFinalNtf::Process(UINT32 dwConnID) {}
void PtcG2M_AddPartnerLivenessG2M::Process(UINT32 dwConnID) {}
void PtcG2C_HorseCountDownTimeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_DoodadItemAddNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GCFZhanLingNotify::Process(UINT32 dwConnID) {}
void PtcG2C_InvFightBefEnterSceneNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HeroBattleDataNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ScenePrepareInfoNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HeroBattleTeamMsgNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HeroBattleSyncNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HeroBattleInCircleNtf::Process(UINT32 dwConnID) {}
void PtcG2M_GCFSynG2M::Process(UINT32 dwConnID) {}
void PtcG2C_HeroBattleOverTime::Process(UINT32 dwConnID) {}
void PtcG2C_HeroBattleTeamRoleNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GCFSynG2CNtf::Process(UINT32 dwConnID) {}
void PtcG2M_PayAddDiamondNtf::Process(UINT32 dwConnID) {}
void PtcG2M_PayConsumeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_DoodadItemUseNtf::Process(UINT32 dwConnID) {}
void PtcG2C_SpecialStateNtf::Process(UINT32 dwConnID) {}
void PtcG2C_CloseChooseHeroNtf::Process(UINT32 dwConnID) {}
void PtcG2R_BroadCastG2R::Process(UINT32 dwConnID) {}
void PtcG2C_LevelScriptStateNtf::Process(UINT32 dwConnID) {}
void PtcG2C_SceneStateNtf::Process(UINT32 dwConnID) {}
void PtcG2C_QueryRoleStateAck::Process(UINT32 dwConnID) {}
void PtcG2C_IBShopHasBuyNtf::Process(UINT32 dwConnID) {}
void PtcG2C_SkillInitCoolNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HeroBattleCanUseHero::Process(UINT32 dwConnID) {}
void PtcG2C_LeagueBattleLoadInfoNtf::Process(UINT32 dwConnID) {}
void PtcG2C_LeagueBattleBaseDataNtf::Process(UINT32 dwConnID) {}
void PtcG2C_LeagueBattleOneResultNft::Process(UINT32 dwConnID) {}
void PtcG2C_LeagueBattleResultNtf::Process(UINT32 dwConnID) {}
void PtcG2C_PushPraiseNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HeroBattleTipsNtf::Process(UINT32 dwConnID) {}
void PtcG2C_LeagueBattleStateNtf::Process(UINT32 dwConnID) {}
void PtcG2M_LeagueBattleUpdateDataG2M::Process(UINT32 dwConnID) {}
void PtcG2M_LeagueEliminationUpdateG2M::Process(UINT32 dwConnID) {}
void PtcG2M_SynGuildBossAttr::Process(UINT32 dwConnID) {}
void PtcG2M_LevelChangeNtfMs::Process(UINT32 dwConnID) {}
void PtcG2K_LeagueEliminationUpdateG2K::Process(UINT32 dwConnID) {}
void PtcG2M_OpenSystemNtfMs::Process(UINT32 dwConnID) {}
void PtcG2T_BroadCastG2T::Process(UINT32 dwConnID) {}
void PtcG2C_CountDownNtf::Process(UINT32 dwConnID) {}
void PtcG2C_AllGiftIBItemNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GiftIBBackInfoNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ShareRandomGiftNtf::Process(UINT32 dwConnID) {}
void PtcG2C_WorldBossGuildAddAttrSyncClientNtf::Process(UINT32 dwConnID) {}
void PtcG2M_UpdateGlobalValue::Process(UINT32 dwConnID) {}
void PtcG2C_MilitaryrankNtf::Process(UINT32 dwConnID) {}
void PtcG2C_LoginActivityStatusNtf::Process(UINT32 dwConnID) {}
void PtcG2C_DisplayAddItem::Process(UINT32 dwConnID) {}
void PtcG2C_UpdateDisplayItems::Process(UINT32 dwConnID) {}
void PtcG2C_guildcamppartyNotify::Process(UINT32 dwConnID) {}
void PtcG2C_AbsPartyNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GuildCampPartyTradeNotify::Process(UINT32 dwConnID) {}
void PtcG2M_ReportCustomBattleResult::Process(UINT32 dwConnID) {}
void PtcG2M_ReportBadPlayer2Ms::Process(UINT32 dwConnID) {}
void PtcG2C_CustomBattleLoadingNtf::Process(UINT32 dwConnID) {}
void PtcG2M_TarjaInfoNtf::Process(UINT32 dwConnID) {}
void PtcG2M_SceneTeamResultNtf::Process(UINT32 dwConnID) {}
void PtcG2M_TeamCostLeaderResetNtf::Process(UINT32 dwConnID) {}
void PtcG2C_PlatformShareAwardNtf::Process(UINT32 dwConnID) {}
void PtcG2C_WeekEnd4v4RoleDataNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HeroKillNotify::Process(UINT32 dwConnID) {}
void PtcG2C_MobaBattleTeamRoleNtf::Process(UINT32 dwConnID) {}
void PtcG2C_MobaRoleChangeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_MobaBattleTeamMsgNtf::Process(UINT32 dwConnID) {}
void PtcG2C_TransSkillNotfiy::Process(UINT32 dwConnID) {}
void PtcG2C_EntityTargetChangeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_MobaSignalBroadcast::Process(UINT32 dwConnID) {}
void PtcG2C_PetInviteNtf::Process(UINT32 dwConnID) {}
void PtcG2M_AddMobaBattleRecord::Process(UINT32 dwConnID) {}
void PtcG2C_ChatNotifyG2C::Process(UINT32 dwConnID) {}
void PtcG2C_MobaMiniMapNtf::Process(UINT32 dwConnID) {}
void PtcG2M_ClientSessionChangeG2M::Process(UINT32 dwConnID) {}
void PtcG2M_RoleLoginReconnectNtf::Process(UINT32 dwConnID) {}
void PtcG2C_MobaAddExpNtf::Process(UINT32 dwConnID) {}
void PtcG2C_BMReadyTimeNtf::Process(UINT32 dwConnID) {}
void PtcG2K_BMSceneFightInfoNtf::Process(UINT32 dwConnID) {}
void PtcG2C_BMFightTimeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_BMRoleSceneSyncNtf::Process(UINT32 dwConnID) {}
void PtcG2C_BigMeleeReliveNtf::Process(UINT32 dwConnID) {}
void PtcG2K_BMLeaveFightSceneNtf::Process(UINT32 dwConnID) {}
void PtcG2C_MobaHintNtf::Process(UINT32 dwConnID) {}
void PtcG2M_TeamTranGs2Ms::Process(UINT32 dwConnID) {}
void PtcG2C_TajieHelpNotify::Process(UINT32 dwConnID) {}
void PtcG2C_NotifyStartUpTypeToClient::Process(UINT32 dwConnID) {}
void PtcG2C_HeroBattleAncientPower::Process(UINT32 dwConnID) {}
void PtcG2C_BattleStatisticsNtf::Process(UINT32 dwConnID) {}
void PtcG2C_WeddingLoadInfoNtf::Process(UINT32 dwConnID) {}
void PtcG2C_WeddingStateNtf::Process(UINT32 dwConnID) {}
void PtcG2C_WeddingEventNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HorseFailTipsNtf::Process(UINT32 dwConnID) {}
void PtcG2C_WarningRandomNtf::Process(UINT32 dwConnID) {}
void PtcG2M_UpdateWeddingStateG2M::Process(UINT32 dwConnID) {}
void PtcG2C_ThemeActivityChangeNtf::Process(UINT32 dwConnID) {}
void PtcG2K_RoleLeaveCrossSceneNtf::Process(UINT32 dwConnID) {}
void PtcG2C_WeddingCarNtf::Process(UINT32 dwConnID) {}
void PtcG2C_BackFlowOpenNtf::Process(UINT32 dwConnID) {}
void PtcG2M_PrivateChat::Process(UINT32 dwConnID) {}
void PtcG2C_TransNotify::Process(UINT32 dwConnID) {}
void PtcG2M_InvfightAgainReqG2M::Process(UINT32 dwConnID) {}
void PtcG2K_InvfightAgainReqG2W::Process(UINT32 dwConnID) {}
void PtcG2M_reqdragonguildsimpledata::Process(UINT32 dwConnID) {}
void PtcG2M_CostDragonGuildReceiveCount::Process(UINT32 dwConnID) {}
void PtcG2M_CheckDragonGuildTask::Process(UINT32 dwConnID) {}
void PtcG2C_BattleFieldReadyInfoNtf::Process(UINT32 dwConnID) {}
void PtcG2C_BattleFieldReliveNtf::Process(UINT32 dwConnID) {}
void PtcG2C_BattleFieldRoleAgainstNtf::Process(UINT32 dwConnID) {}
void PtcG2M_GoalAwardsServerEvent::Process(UINT32 dwConnID) {}
void PtcG2C_BFFightTimeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_PayScoreNtf::Process(UINT32 dwConnID) {}
void PtcG2M_GoalAwardsServerTeam::Process(UINT32 dwConnID) {}
void PtcG2C_NpcFlNtf::Process(UINT32 dwConnID) {}
void PtcG2C_DoodadItemSkillsNtf::Process(UINT32 dwConnID) {}
void PtcG2C_WordNotify::Process(UINT32 dwConnID) {}
void PtcG2C_BigMeleePointNtf::Process(UINT32 dwConnID) {}
void PtcG2C_BigMeleePointOutLookNtf::Process(UINT32 dwConnID) {}
void PtcG2C_KickAccountJkydNtf::Process(UINT32 dwConnID) {}
void PtcG2M_KickAccountJkydG2M::Process(UINT32 dwConnID) {}
void PtcG2C_WorldLevelNtf2Client::Process(UINT32 dwConnID) {}
void PtcG2C_UpdateStageInfoNtf::Process(UINT32 dwConnID) {}

