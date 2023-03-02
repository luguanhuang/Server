#include "pch.h"
#include "forward.h"
#include "config.h"
#include "ptcregister.h"
#include "crpc.h"
#include "protocol.h"

// generate by ProtoGen at date: 2017/10/26 12:00:50

#include "common/ptct2g_registserverntf.h"
#include "scene/ptcc2g_enterscenereq.h"
#include "scene/ptcc2g_leavescenereq.h"
#include "action/ptcc2g_moveoperationreq.h"
#include "skill/ptcc2g_castskill.h"
#include "common/ptcc2g_updatetutorial.h"
#include "battle/ptcc2g_roledeathinsoloscene.h"
#include "scene/ptcc2g_enterscenecooldownquery.h"
#include "common/ptcc2g_operaterecordntf.h"
#include "reward/ptcc2g_onlinerewardreport.h"
#include "reward/ptcc2g_nextdayrewardreport.h"
#include "common/ptcc2g_delaynotify.h"
#include "skill/ptcc2g_skillresultreq.h"
#include "skill/ptcc2g_skillbulletfirereport.h"
#include "skill/ptcc2g_skillbulletresultreq.h"
#include "scene/ptcc2g_addlevelscriptextstring.h"
#include "scene/ptcc2g_scenedamagerankreport.h"
#include "login/ptct2g_reconnectntf.h"
#include "item/ptcc2g_changesupplementreport.h"
#include "common/ptcc2g_qteoperation.h"
#include "battle/ptcc2g_autofightntf.h"
#include "scene/ptcc2g_syncscenefinish.h"
#include "tsssdk/ptcc2g_tsssdksendanti2server.h"
#include "team/ptcc2g_fmbrefuse.h"
#include "common/ptcn2g_newgateconnected.h"
#include "qa/ptcm2g_updateguildqaroom.h"
#include "designation/ptcd2g_d2gdesignationntf.h"
#include "levelseal/ptcm2g_mslevelsealntf.h"
#include "scene/ptcm2g_noticeguildbossendtogsn.h"
#include "friend/ptcm2g_updatefriendongs.h"
#include "battlewatch/ptcm2g_notifywatchiconnum.h"
#include "scene/ptcm2g_synscenerewardm2g.h"
#include "guild/ptcm2g_changeguildbosstime.h"
#include "worldboss/ptcm2g_worldbosssyncgs.h"
#include "chat/ptcc2g_joinroom.h"
#include "chat/ptcc2g_quitroom.h"
#include "firstpass/ptcm2g_firstpassnewcommendrewardntf.h"
#include "guild/ptcm2g_synroleinguild.h"
#include "role/ptcm2g_synguildinfo.h"
#include "superrisk/ptcc2g_playdiceover.h"
#include "ibshop/ptcc2g_removeibshopicon.h"
#include "chat/ptcc2g_setvoipmemberstate.h"
#include "arena/ptcm2g_syncarenags.h"
#include "guild/ptcm2g_synguildsimpledata.h"
#include "sprite/ptcc2g_openspriteeggntf.h"
#include "guild/ptcm2g_mnewguildbonusntf.h"
#include "guild/ptcm2g_notifyguildcheckinprocess.h"
#include "guild/ptcm2g_synfatigueresult.h"
#include "guild/ptcm2g_synguildcheckinhint.h"
#include "qa/ptcm2g_giveanserqarewardntf.h"
#include "qa/ptcm2g_qaovertogs.h"
#include "garden/ptcm2g_gardeneventntf.h"
#include "role/ptcm2g_roleeventsyncgs.h"
#include "common/ptck2g_newrouterntf.h"
#include "battle/ptcc2g_battlelogreport.h"
#include "guildcamp/ptcc2g_clickguildcamp.h"
#include "common/ptcm2g_syncservertime.h"
#include "common/ptcm2g_worldlevelntf.h"
#include "common/ptcr2g_notifyserverclose.h"
#include "guild/ptcc2g_syndoingguildinherit.h"
#include "guild/ptcm2g_noticeguildinheritnuminfo.h"
#include "common/ptcn2g_updatefunctionopen2gs.h"
#include "common/ptcr2g_mscrashrecoverr2g.h"
#include "scene/ptcm2g_msdestoryscene.h"
#include "role/ptcc2g_picurlntf.h"
#include "partner/ptcm2g_updatepartnertogs.h"
#include "guildcastle/ptcm2g_gcfsynm2g.h"
#include "common/ptcn2g_updatestartuptypen2gntf.h"
#include "guild/ptcm2g_tryteacherinherit.h"
#include "pay/ptcm2g_balanceinfontf.h"
#include "common/ptcr2g_notifyserverconnect.h"
#include "role/ptcc2g_queryrolestatereq.h"
#include "reddot/ptcc2g_cancelreddotreq.h"
#include "guild/ptcm2g_synguildbosshp.h"
#include "guild/ptcm2g_synguildbossdie.h"
#include "guild/ptcm2g_synguildbossdoend.h"
#include "guild/ptcm2g_noticeguildbossattr.h"
#include "misc/ptcm2g_controlfunctionntf.h"
#include "misc/ptcm2g_clicknoticentf.h"
#include "common/ptck2g_syncservertime2crossgs.h"
#include "common/ptcc2g_closehintntf.h"
#include "common/ptck2g_broadcastworld2gs.h"
#include "militaryrank/ptcm2g_militaryrankchange.h"
#include "pay/ptcm2g_buypayaileenntf.h"
#include "worldboss/ptcm2g_worldbossguildaddattrsyncgsntf.h"
#include "scene/ptcm2g_enterscenefromms.h"
#include "common/ptcm2g_broadcastglobalvalue.h"
#include "loginactivity/ptcc2g_loginactivityawardreq.h"
#include "pk/ptcm2g_pkupdatesynntf.h"
#include "platformshareresult/ptcc2g_notifyplatshareresult.h"
#include "militaryrank/ptcm2g_addmilitaryrank.h"
#include "common/ptck2g_servercrossdataw2crossg.h"
#include "team/ptcm2g_teamtranms2gs.h"
#include "battle/ptcc2g_battlestatisticsreport.h"
#include "misc/ptcc2g_sharereport.h"
#include "wedding/ptcm2g_weddingcarendm2g.h"
#include "bigmelee/ptck2g_bigmeleeendntf.h"
#include "invfight/ptcc2g_invfightagainreqc2g.h"
#include "dragonguild/ptcm2g_syndragonguildsimpledata.h"
#include "dragonguild/ptcm2g_notifydragonguildredpoint.h"
#include "goalawards/ptcm2g_goalawardsserverawards.h"
#include "battlefield/ptcm2g_battlefiedlreadyscenegsntf.h"
#include "npcfeeling/ptcm2g_npcflnotifym2g.h"
#include "battlefield/ptcm2g_battlefailinfogsntf.h"
#include "common/ptcc2g_updatefreeflowhintinfo.h"
#include "pay/ptcm2g_consumerebatentf.h"
#include "pay/ptcm2g_paytotalntf.h"
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
#include "common/rpcc2g_synctime.h"
#include "skill/rpcc2g_skilllevelup.h"
#include "common/rpcc2g_fetchachivementreward.h"
#include "battle/rpcc2g_selectchestreward.h"
#include "skill/rpcc2g_resetskill.h"
#include "role/rpcc2g_chooseprofession.h"
#include "skill/rpcc2g_bindskill.h"
#include "task/rpcc2g_taskoperate.h"
#include "lottery/rpcc2g_querylotterycd.h"
#include "lottery/rpcc2g_drawlottery.h"
#include "enhance/rpcc2g_enhanceitem.h"
#include "scene/rpcc2g_queryscenedaycount.h"
#include "common/rpcc2g_gmcommand.h"
#include "enhance/rpcc2g_decomposeequipment.h"
#include "emblem/rpcc2g_emblemlevelup.h"
#include "common/rpcc2g_useitem.h"
#include "jade/rpcc2g_jadeoperation.h"
#include "jade/rpcc2g_jadecompose.h"
#include "checkin/rpcc2g_checkin.h"
#include "activity/rpcc2g_getactivitychest.h"
#include "fashion/rpcc2g_fashioncompose.h"
#include "jade/rpcc2g_buyjadeslot.h"
#include "reward/rpcc2g_getsystemreward.h"
#include "battle/rpcc2g_sweep.h"
#include "battle/rpcc2g_revive.h"
#include "shop/rpcc2g_queryshopitem.h"
#include "shop/rpcc2g_buyshopitem.h"
#include "pay/rpcc2g_buygoldandfatigue.h"
#include "common/rpcc2g_addtempattr.h"
#include "common/rpcc2g_queryopengameactivitytime.h"
#include "social/rpcc2g_showflowerpage.h"
#include "social/rpcc2g_sendflower.h"
#include "common/rpcc2g_fetchchapterchest.h"
#include "guild/rpcc2g_startguildcard.h"
#include "guild/rpcc2g_queryguildcard.h"
#include "guild/rpcc2g_changeguildcard.h"
#include "guild/rpcc2g_endguildcard.h"
#include "guild/rpcc2g_getguildcheckinbox.h"
#include "scene/rpcc2g_stagecountreset.h"
#include "guild/rpcc2g_getguildbonuslist.h"
#include "guild/rpcc2g_getguildbonusdetailinfo.h"
#include "guild/rpcc2g_getguildbonusreward.h"
#include "scene/rpcc2g_openscenechest.h"
#include "scene/rpcc2g_reportbattle.h"
#include "reward/rpcc2g_getonlinereward.h"
#include "reward/rpcc2g_getnextdayreward.h"
#include "battle/rpcc2g_fetchenemydoodadreq.h"
#include "guild/rpcc2g_guildgoblininfo.h"
#include "item/rpcc2g_usesupplement.h"
#include "common/rpcc2g_setroleconfig.h"
#include "slotattr/rpcc2g_levelupslotattr.h"
#include "pet/rpcc2g_petoperation.h"
#include "team/rpcc2g_buyteamscenecount.h"
#include "findback/rpcc2g_expfindback.h"
#include "activity/rpcc2g_gettoweractivitytop.h"
#include "designation/rpcc2g_getdesignationreq.h"
#include "designation/rpcc2g_setdesignationreq.h"
#include "designation/rpcc2g_getclassifydesignationreq.h"
#include "achievement/rpcc2g_getachievebrifinforeq.h"
#include "achievement/rpcc2g_getachieveclassifyinforeq.h"
#include "battle/rpcc2g_reqplayerautoplay.h"
#include "achievement/rpcc2g_getachieverewardreq.h"
#include "levelseal/rpcc2g_getlevelsealinfo.h"
#include "levelseal/rpcc2g_levelsealbuttonstatus.h"
#include "achievement/rpcc2g_getachievepointrewardreq.h"
#include "reward/rpcc2g_reqgetloginreward.h"
#include "reward/rpcc2g_fivedayrewardreq.h"
#include "guild/rpcc2g_guildcheckinbonusinfo.h"
#include "guild/rpcc2g_sendguildbonus.h"
#include "guild/rpcc2g_askforcheckinbonus.h"
#include "guild/rpcc2g_thanksforbonus.h"
#include "pvp/rpcc2g_pvpallreq.h"
#include "qa/rpcc2g_agreeqareq.h"
#include "social/rpcc2g_getflowerlefttime.h"
#include "social/rpcc2g_getflower.h"
#include "social/rpcc2g_getflowerreward.h"
#include "social/rpcc2g_getflowerrewardlist.h"
#include "equip/rpcc2g_enhancetranster.h"
#include "pvp/rpcc2g_pvpnowalldata.h"
#include "dragon/rpcc2g_getdragontopinfo.h"
#include "common/rpcg2m_registergs2ms.h"
#include "team/rpcc2g_teaminvitelistreq.h"
#include "common/rpcg2t_registergs2gt.h"
#include "common/rpct2g_registergt2gs.h"
#include "scene/rpcn2g_enterscene.h"
#include "scene/rpcn2g_leavescene.h"
#include "role/rpcg2d_saveroledata.h"
#include "emblem/rpcc2g_identifyemblem.h"
#include "qa/rpcc2g_openguildqareq.h"
#include "qa/rpcc2g_getguildqadatareq.h"
#include "activity/rpcc2g_mulactivityreq.h"
#include "battlewatch/rpcc2g_getwatchinfobyid.h"
#include "battlewatch/rpcc2g_getmywatchrecord.h"
#include "battlewatch/rpcc2g_enterwatchbattle.h"
#include "battlewatch/rpcc2g_changewatchrole.h"
#include "battlewatch/rpcc2g_commendwatchbattle.h"
#include "common/rpcc2g_bossrushreq.h"
#include "scene/rpcc2g_doenterscene.h"
#include "scene/rpcg2n_changesceneverify.h"
#include "jade/rpcc2g_takeoffalljade.h"
#include "activity/rpcc2g_sweeptower.h"
#include "activity/rpcc2g_resettower.h"
#include "gm/rpcm2g_gshttpcommandnew.h"
#include "goddesstrial/rpcc2g_getgoddesstrialrewards.h"
#include "scene/rpcn2g_createbattle.h"
#include "chat/rpcm2g_chatcansend.h"
#include "battlewatch/rpcg2m_getwatchinfobytype.h"
#include "battlewatch/rpcg2m_enterwatchbattlems.h"
#include "levelseal/rpcg2m_inclevelsealbosscount.h"
#include "guild/rpcm2g_noticeguildbossdoendtogsn.h"
#include "common/rpcc2g_querypowerpoint.h"
#include "pay/rpcc2g_getpayallinfo.h"
#include "pay/rpcc2g_paycardaward.h"
#include "pay/rpcc2g_payclick.h"
#include "activity/rpcc2g_getactivityinfo.h"
#include "social/rpcg2m_msgetflwoerrankreward.h"
#include "social/rpcg2m_mssendflower.h"
#include "social/rpcm2g_getflowerrewardstate.h"
#include "team/rpcm2g_teamsynm2c.h"
#include "team/rpcm2g_teamstartbattlem2g.h"
#include "team/rpcm2g_teamrequestm2g.h"
#include "pet/rpcc2g_synpetinfo.h"
#include "battle/rpcc2g_chooserollreq.h"
#include "pay/rpcc2g_payfirstaward.h"
#include "pay/rpcc2g_growthfundaward.h"
#include "pay/rpcc2g_buyviplevelgift.h"
#include "title/rpcc2g_titlelevelup.h"
#include "firstpass/rpcc2g_firstpassinforeq.h"
#include "firstpass/rpcc2g_getfirstpassreward.h"
#include "firstpass/rpcc2g_commendfirstpass.h"
#include "firstpass/rpcg2m_mscommendfirstpass.h"
#include "firstpass/rpcc2g_firstpassgettoproleinfo.h"
#include "firstpass/rpcg2m_getrankinfo.h"
#include "pvp/rpcg2m_pvpallg2mreq.h"
#include "common/rpcg2m_gmcommandg2m.h"
#include "team/rpcm2g_msusesummaryreq.h"
#include "team/rpcm2g_teamchangeexpcheck.h"
#include "ranklist/rpcg2m_updatemsranklist.h"
#include "pk/rpcg2m_pkrankreq.h"
#include "pk/rpcm2g_pkreqm2g.h"
#include "guild/rpcm2g_createguildcost.h"
#include "auction/rpcm2g_auctionallm2greq.h"
#include "guild/rpcm2g_enterguildpower.h"
#include "role/rpcc2g_changeoutlookop.h"
#include "findback/rpcc2g_itemfindbackinfo.h"
#include "findback/rpcc2g_itemfindback.h"
#include "atlas/rpcc2g_activatatlas.h"
#include "atlas/rpcc2g_autobreakatlas.h"
#include "atlas/rpcc2g_breakatlas.h"
#include "ibshop/rpcc2g_buyibitem.h"
#include "ibshop/rpcc2g_queryibitem.h"
#include "sprite/rpcc2g_spriteoperation.h"
#include "scene/rpcc2g_deprogressreq.h"
#include "superrisk/rpcc2g_getriskmapinfos.h"
#include "superrisk/rpcc2g_playdicerequest.h"
#include "superrisk/rpcc2g_changeriskboxstate.h"
#include "superrisk/rpcc2g_reenterriskbattle.h"
#include "levelseal/rpcc2g_levelsealexchange.h"
#include "superrisk/rpcc2g_playdiceover.h"
#include "guild/rpcg2m_opguildreqdata.h"
#include "superrisk/rpcc2g_riskbuyrequest.h"
#include "smelt/rpcc2g_smeltitem.h"
#include "arena/rpcm2g_startarenags.h"
#include "activity/rpcc2g_refreshsweepreward.h"
#include "activity/rpcc2g_getsweeptowerreward.h"
#include "role/rpcm2g_reqguilddarerewardrec.h"
#include "guild/rpcg2m_checkguilddarereward.h"
#include "guild/rpcc2g_learnguildskill.h"
#include "arena/rpcg2m_endareanms.h"
#include "guild/rpcm2g_reqroleguildskill.h"
#include "guild/rpcg2m_mguildbonusoperatereq.h"
#include "scene/rpcc2g_queryscenetime.h"
#include "guild/rpcm2g_reqguildactivity.h"
#include "guild/rpcm2g_reqguildcheckin.h"
#include "guild/rpcm2g_noticeguildcheckin.h"
#include "guild/rpcg2m_notifycheckinbox.h"
#include "guild/rpcg2m_guildcardsyncms.h"
#include "activity/rpcc2g_gettowerfirstpassreward.h"
#include "guild/rpcm2g_reqrecvfatigue.h"
#include "designation/rpcm2g_designationeventreq.h"
#include "common/rpcm2g_gsreload.h"
#include "guild/rpcg2m_reqguildladderreward.h"
#include "common/rpcm2g_msconsumeitems.h"
#include "scene/rpcc2g_gmfreadyreq.h"
#include "battle/rpcc2g_inspirereq.h"
#include "guild/rpcc2g_reqguildbosstimeleft.h"
#include "scene/rpcc2g_skycityallinforeq.h"
#include "qa/rpcg2m_enterqaroom.h"
#include "qa/rpcg2m_createguildqaroom.h"
#include "spactivity/rpcc2g_getspactivityreward.h"
#include "spactivity/rpcc2g_getspactivitybigprize.h"
#include "garden/rpcm2g_gardenplayevent.h"
#include "garden/rpcm2g_gardenplantchangestage.h"
#include "guild/rpcm2g_getguildladderrewardtime.h"
#include "login/rpct2g_reconnectgs.h"
#include "levelseal/rpcc2g_getlevelsealselfgift.h"
#include "guild/rpcm2g_queryrolecardmatch.h"
#include "common/rpcg2r_registergs2router.h"
#include "common/rpcg2k_registergs2world.h"
#include "battle/rpck2g_createbattlecross.h"
#include "lottery/rpcc2g_goldclick.h"
#include "common/rpcg2n_registergs2ns.h"
#include "mail/rpcm2g_givemailprize.h"
#include "auction/rpcm2g_guildauctreqm2c.h"
#include "reswar/rpcc2g_reswarallinforeqone.h"
#include "auction/rpcm2g_guildauctfailback.h"
#include "designation/rpcg2m_gsdesignationeventreq.h"
#include "item/rpcc2g_itembuffop.h"
#include "dance/rpcc2g_justdance.h"
#include "task/rpcc2g_getdailytaskinfo.h"
#include "task/rpcc2g_getdailytaskreward.h"
#include "task/rpcc2g_dailytaskaskhelp.h"
#include "task/rpcg2m_askguildhelp.h"
#include "task/rpcm2g_costdonateitem.h"
#include "task/rpcm2g_querydonateinfo.h"
#include "guild/rpcm2g_tryaddguildinherit.h"
#include "qqvip/rpcc2g_queryqqvipinfo.h"
#include "task/rpcc2g_dailytaskgiveup.h"
#include "common/rpcm2g_crashrecoverm2g.h"
#include "role/rpcm2g_fetchroleinfoonms.h"
#include "reswar/rpcc2g_reswarbuff.h"
#include "team/rpcg2m_tryteamcostreward.h"
#include "role/rpcm2g_changenamegs.h"
#include "qqvip/rpcc2g_queryqqfriendsvipinfo.h"
#include "guild/rpcm2g_reqguildinherittimes.h"
#include "equip/rpcc2g_enchantequip.h"
#include "partner/rpcm2g_makepartnerm2g.h"
#include "partner/rpcc2g_takepartnerchest.h"
#include "partner/rpcg2m_getpartnerinfog2m.h"
#include "partner/rpcm2g_getpartnerrecordm2g.h"
#include "herobattle/rpcc2g_getherobattleinfo.h"
#include "horse/rpcc2g_horsereconnect.h"
#include "photo/rpcc2g_photographeffect.h"
#include "photo/rpcg2m_getcharm.h"
#include "invfight/rpcm2g_invfightm2greq.h"
#include "partner/rpcm2g_buypartneritemm2g.h"
#include "pokertournament/rpcm2g_pokertournamentsignupcost.h"
#include "pandora/rpcc2g_pandoralottery.h"
#include "chat/rpcc2g_queryclientip.h"
#include "herobattle/rpcc2g_setheroinherobattle.h"
#include "pay/rpcm2g_paygetbalance.h"
#include "pay/rpcm2g_payconsumeaddgoods.h"
#include "guildbonus/rpcc2g_getguildbonusleft.h"
#include "herobattle/rpcc2g_getherobattlegamerecord.h"
#include "herobattle/rpcc2g_buyheroinherobattle.h"
#include "herobattle/rpcc2g_getherobattleweekreward.h"
#include "castle/rpcm2g_gcfcommonreqm2g.h"
#include "garden/rpcg2m_getgardeninfo.h"
#include "atlas/rpcc2g_atlasupstar.h"
#include "leaguebattle/rpcc2g_leaguebattlereadyreq.h"
#include "equip/rpcc2g_forgeequip.h"
#include "role/rpcc2g_changeprofession.h"
#include "leaguebattle/rpcg2m_leaguerankupdateg2m.h"
#include "role/rpcg2m_changeprofessionms.h"
#include "role/rpcg2d_readhiddenroles.h"
#include "holidaystage/rpcc2g_getholidaystageinfo.h"
#include "item/rpcc2g_itemsell.h"
#include "equip/rpcc2g_enchanttransfer.h"
#include "role/rpcc2g_personalcareer.h"
#include "role/rpcg2m_personalcareertotalinfo.h"
#include "role/rpcc2g_changedeclaration.h"
#include "skill/rpcc2g_changeskillset.h"
#include "leaguebattle/rpcg2k_leaguerankupdateg2k.h"
#include "misc/rpcm2g_checknotice.h"
#include "pay/rpcc2g_getpayreward.h"
#include "activity/rpcg2m_getmulactopeninfo.h"
#include "common/rpck2g_crossgsreload.h"
#include "militaryrank/rpcg2m_getmilitaryrank.h"
#include "pay/rpcc2g_payfrienditem.h"
#include "pay/rpcg2m_uppayfrienditem.h"
#include "spactivity/rpcg2m_syncactivityend2ms.h"
#include "item/rpcc2g_itemcompose.h"
#include "spactivity/rpcc2g_getbackflowspacttime.h"
#include "worldboss/rpcm2g_worldbossguildaddattrgs.h"
#include "spactivity/rpcc2g_argentaactivity.h"
#include "guild/rpcm2g_modifygsguildname.h"
#include "lottery/rpcc2g_getplatformsharechest.h"
#include "loginactivity/rpcc2g_loginactivity.h"
#include "pay/rpcc2g_getgrowthfundtime.h"
#include "ibshop/rpcm2g_giftibitemreq.h"
#include "fashion/rpcc2g_activatefashioncharm.h"
#include "pay/rpcg2m_paydegreereq.h"
#include "guild/rpcc2g_getguildcampartyranditem.h"
#include "guild/rpcc2g_talkguildcampsprite.h"
#include "guild/rpcc2g_guildcamppartyreqexchange.h"
#include "guild/rpcc2g_getguildcamppartyexchangeinfo.h"
#include "custombattle/rpcm2g_custombattlegsop.h"
#include "guild/rpcc2g_guildcampexchangeoperate.h"
#include "guild/rpcc2g_getguildcamppartyreward.h"
#include "pay/rpcm2g_giftibreqgsgoods.h"
#include "artifact/rpcc2g_artifactcompose.h"
#include "guild/rpcc2g_replypartyexchangeitemopt.h"
#include "absparty/rpcc2g_absenterscene.h"
#include "enchant/rpcc2g_enchantactiveattribute.h"
#include "emblem/rpcc2g_buyextraskillemblemslot.h"
#include "battlewatch/rpcc2g_changelivevisible.h"
#include "custombattle/rpcg2m_custombattleresultms.h"
#include "custombattle/rpcg2k_custombattleresultworld.h"
#include "battle/rpcc2g_reportbadplayer.h"
#include "team/rpcm2g_teamtranms2gs.h"
#include "guild/rpcc2g_getguildpartyreceiveinfo.h"
#include "smelt/rpcc2g_returnsmeltstone.h"
#include "match/rpcm2g_kmatchcommonreqm2c.h"
#include "platformshareresult/rpcc2g_getplatshareaward.h"
#include "weekend4v4/rpcc2g_weekend4v4getinfo.h"
#include "scene/rpcc2g_scenemobaop.h"
#include "mobabattle/rpcc2g_mobasignaling.h"
#include "fashion/rpcc2g_activatehaircolor.h"
#include "guild/rpcm2g_tryteacherinheritnew.h"
#include "common/rpcc2g_dhrreqc2g.h"
#include "pet/rpcc2g_petoperationother.h"
#include "login/rpcn2g_notifyroleloginreconnect2gs.h"
#include "bigmelee/rpcg2k_reportbmroleinfo.h"
#include "common/rpcm2g_msgiveitem.h"
#include "team/rpcm2g_teamstartbattlem2gcross.h"
#include "sprite/rpcc2g_buyspriteegg.h"
#include "enhance/rpcc2g_getenhanceattr.h"
#include "spactivity/rpcc2g_getancienttimesaward.h"
#include "herobattle/rpcc2g_selectheroancientpower.h"
#include "marriage/rpcm2g_checkmarriagecouple.h"
#include "wedding/rpcc2g_weddingoperator.h"
#include "spactivity/rpcc2g_tactenterscene.h"
#include "dragongroup/rpcc2g_dragongrouprolelist.h"
#include "dragongroup/rpcc2g_dragongrouprecord.h"
#include "marriage/rpcm2g_givemarriagelvrerewardm2g.h"
#include "wedding/rpcm2g_holdweddingcheckm2g.h"
#include "artifact/rpcc2g_artifactdeitystoveop.h"
#include "wedding/rpcm2g_weddingoperm2g.h"
#include "spactivity/rpcc2g_themeactivityhint.h"
#include "pay/rpcc2g_backflowbuyop.h"
#include "transform/rpcc2g_transformop.h"
#include "equip/rpcc2g_upgradeequip.h"
#include "fashion/rpcc2g_getfashionsynthesisinfo.h"
#include "fashion/rpcc2g_fashionibshopbuy.h"
#include "dragonguild/rpcm2g_createdragonguildcost.h"
#include "dragonguild/rpcm2g_getrolepower.h"
#include "dragonguild/rpcm2g_getdragonguildrecordm2g.h"
#include "dragonguild/rpcg2m_getdragonguildinfog2m.h"
#include "dragonguild/rpcc2g_takedragonguildchest.h"
#include "dragonguild/rpcm2g_modifydragonguildnamecost.h"
#include "dragonguild/rpcc2g_getdragonguildtaskchest.h"
#include "dragonguild/rpcg2m_getdragonguildtaskchestg2m.h"
#include "dragonguild/rpcm2g_getdragonguildrecieveinfo.h"
#include "dragonguild/rpcm2g_buydragonguilditemm2g.h"
#include "dragonguild/rpcg2m_checkdragonguildredpointg2m.h"
#include "battlefield/rpcc2g_battlefieldawardnumreq.h"
#include "battlefield/rpcc2g_battlefieldrankreq.h"
#include "task/rpcc2g_getweeklytaskinfo.h"
#include "task/rpcc2g_getweeklytaskreward.h"
#include "task/rpcc2g_refreshweeklytask.h"
#include "battlefield/rpcc2g_battlefieldroleagainstreq.h"
#include "competedragon/rpcc2g_getcompetedragoninfo.h"
#include "pay/rpcc2g_setpreshow.h"
#include "equip/rpcc2g_fuseequip.h"
#include "task/rpcc2g_turnoverweeklytaskitem.h"
#include "task/rpcg2m_taskdonateg2m.h"
#include "spactivity/rpcc2g_getluckyactivityinfo.h"
#include "spactivity/rpcc2g_buydraw.h"
#include "spactivity/rpcc2g_lotterydraw.h"
#include "npcfeeling/rpcm2g_npcflreqm2g.h"
#include "bigmelee/rpcg2k_bmranklistreq.h"
#include "task/rpcm2g_dailytaskrefreshm2g.h"
#include "task/rpcg2m_dailytaskrefreshg2m.h"
#include "survive/rpcc2g_survivereqc2g.h"
#include "survive/rpcg2m_survivesynpointg2m.h"
#include "battle/rpcc2g_doodaditemallskillreq.h"
#include "scene/rpcc2g_peerbox.h"
#include "scene/rpcc2g_queryboxs.h"
#include "pay/rpcc2g_activatepreshow.h"
#include "guild/rpcc2g_guildpartysummonspirit.h"
#include "task/rpcg2m_getdailytaskg2m.h"
#include "fashion/rpcc2g_choosespecialeffects.h"
#include "team/rpcg2k_teamgotonextscenereq.h"
#include "scene/rpcc2g_enterleisurescene.h"
#include "spactivity/rpcc2g_backflowactivityoperation.h"
#include "activity/rpcc2g_getnewzonebenefit.h"
#include "activity/rpcg2m_getnewzonebenefitg2m.h"
#include "activity/rpcc2g_selectchargebackrole.h"


static BaseLink *FindLink(const char *linkName)
{
	return Config::GetConfig()->FindLinkHandler(linkName);
}

void CProtocolRegister::Regiter() 
{
	CProtocol::Register(new PtcT2G_RegistServerNtf());
	CProtocol::Register(new PtcC2G_EnterSceneReq());
	CProtocol::Register(new PtcC2G_LeaveSceneReq());
	CProtocol::Register(new PtcC2G_MoveOperationReq());
	CProtocol::Register(new PtcC2G_CastSkill());
	CProtocol::Register(new PtcC2G_UpdateTutorial());
	CProtocol::Register(new PtcC2G_RoleDeathInSoloScene());
	CProtocol::Register(new PtcC2G_EnterSceneCoolDownQuery());
	CProtocol::Register(new PtcC2G_OperateRecordNtf());
	CProtocol::Register(new PtcC2G_OnlineRewardReport());
	CProtocol::Register(new PtcC2G_NextDayRewardReport());
	CProtocol::Register(new PtcC2G_DelayNotify());
	CProtocol::Register(new PtcC2G_SkillResultReq());
	CProtocol::Register(new PtcC2G_SkillBulletFireReport());
	CProtocol::Register(new PtcC2G_SkillBulletResultReq());
	CProtocol::Register(new PtcC2G_AddLevelScriptExtString());
	CProtocol::Register(new PtcC2G_SceneDamageRankReport());
	CProtocol::Register(new PtcT2G_ReconnectNtf());
	CProtocol::Register(new PtcC2G_ChangeSupplementReport());
	CProtocol::Register(new PtcC2G_QTEOperation());
	CProtocol::Register(new PtcC2G_AutoFightNTF());
	CProtocol::Register(new PtcC2G_SyncSceneFinish());
	CProtocol::Register(new PtcC2G_TssSdkSendAnti2Server());
	CProtocol::Register(new PtcC2G_FMBRefuse());
	CProtocol::Register(new PtcN2G_NewGateConnected());
	CProtocol::Register(new PtcM2G_UpdateGuildQARoom());
	CProtocol::Register(new PtcD2G_D2GDesignationNtf());
	CProtocol::Register(new PtcM2G_MsLevelSealNtf());
	CProtocol::Register(new PtcM2G_noticeguildbossendtogsn());
	CProtocol::Register(new PtcM2G_UpdateFriendOnGS());
	CProtocol::Register(new PtcM2G_NotifyWatchIconNum());
	CProtocol::Register(new PtcM2G_SynSceneRewardM2G());
	CProtocol::Register(new PtcM2G_ChangeGuildBossTime());
	CProtocol::Register(new PtcM2G_WorldBossSyncGS());
	CProtocol::Register(new PtcC2G_JoinRoom());
	CProtocol::Register(new PtcC2G_QuitRoom());
	CProtocol::Register(new PtcM2G_FirstPassNewCommendRewardNtf());
	CProtocol::Register(new PtcM2G_SynRoleInGuild());
	CProtocol::Register(new PtcM2G_SynGuildInfo());
	CProtocol::Register(new PtcC2G_PlayDiceOver());
	CProtocol::Register(new PtcC2G_RemoveIBShopIcon());
	CProtocol::Register(new PtcC2G_SetVoipMemberState());
	CProtocol::Register(new PtcM2G_SyncArenaGs());
	CProtocol::Register(new PtcM2G_SynGuildSimpleData());
	CProtocol::Register(new PtcC2G_OpenSpriteEggNtf());
	CProtocol::Register(new PtcM2G_MNewGuildBonusNtf());
	CProtocol::Register(new PtcM2G_NotifyGuildCheckinProcess());
	CProtocol::Register(new PtcM2G_SynFatigueResult());
	CProtocol::Register(new PtcM2G_SynGuildCheckinHint());
	CProtocol::Register(new PtcM2G_GiveAnserQARewardNtf());
	CProtocol::Register(new PtcM2G_QAOverToGS());
	CProtocol::Register(new PtcM2G_GardenEventNtf());
	CProtocol::Register(new PtcM2G_RoleEventSyncGS());
	CProtocol::Register(new PtcK2G_NewRouterNtf());
	CProtocol::Register(new PtcC2G_BattleLogReport());
	CProtocol::Register(new PtcC2G_ClickGuildCamp());
	CProtocol::Register(new PtcM2G_syncservertime());
	CProtocol::Register(new PtcM2G_WorldLevelNtf());
	CProtocol::Register(new PtcR2G_NotifyServerClose());
	CProtocol::Register(new PtcC2G_SynDoingGuildInherit());
	CProtocol::Register(new PtcM2G_NoticeGuildInheritNumInfo());
	CProtocol::Register(new PtcN2G_UpdateFunctionOpen2Gs());
	CProtocol::Register(new PtcR2G_mscrashrecoverr2g());
	CProtocol::Register(new PtcM2G_MSDestoryScene());
	CProtocol::Register(new PtcC2G_PicUrlNtf());
	CProtocol::Register(new PtcM2G_UpdatePartnerToGS());
	CProtocol::Register(new PtcM2G_GCFSynM2G());
	CProtocol::Register(new PtcN2G_UpdateStartUpTypeN2GNtf());
	CProtocol::Register(new PtcM2G_TryTeacherInherit());
	CProtocol::Register(new PtcM2G_BalanceInfoNtf());
	CProtocol::Register(new PtcR2G_NotifyServerConnect());
	CProtocol::Register(new PtcC2G_QueryRoleStateReq());
	CProtocol::Register(new PtcC2G_CancelRedDotReq());
	CProtocol::Register(new PtcM2G_SynGuildBossHp());
	CProtocol::Register(new PtcM2G_SynGuildBossDie());
	CProtocol::Register(new PtcM2G_SynGuildBossDoEnd());
	CProtocol::Register(new PtcM2G_NoticeGuildBossAttr());
	CProtocol::Register(new PtcM2G_ControlFunctionNtf());
	CProtocol::Register(new PtcM2G_ClickNoticeNtf());
	CProtocol::Register(new PtcK2G_SyncServerTime2CrossGs());
	CProtocol::Register(new PtcC2G_CloseHintNtf());
	CProtocol::Register(new PtcK2G_BroadCastWorld2Gs());
	CProtocol::Register(new PtcM2G_MilitaryRankChange());
	CProtocol::Register(new PtcM2G_BuyPayAileenNtf());
	CProtocol::Register(new PtcM2G_WorldBossGuildAddAttrSyncGsNtf());
	CProtocol::Register(new PtcM2G_EnterSceneFromMs());
	CProtocol::Register(new PtcM2G_BroadCastGlobalValue());
	CProtocol::Register(new PtcC2G_LoginActivityAwardReq());
	CProtocol::Register(new PtcM2G_PkUpdateSynNtf());
	CProtocol::Register(new PtcC2G_NotifyPlatShareResult());
	CProtocol::Register(new PtcM2G_AddMilitaryRank());
	CProtocol::Register(new PtcK2G_ServerCrossDataW2CrossG());
	CProtocol::Register(new PtcM2G_TeamTranMs2Gs());
	CProtocol::Register(new PtcC2G_BattleStatisticsReport());
	CProtocol::Register(new PtcC2G_ShareReport());
	CProtocol::Register(new PtcM2G_WeddingCarEndM2G());
	CProtocol::Register(new PtcK2G_BigMeleeEndNtf());
	CProtocol::Register(new PtcC2G_InvfightAgainReqC2G());
	CProtocol::Register(new PtcM2G_SynDragonGuildSimpleData());
	CProtocol::Register(new PtcM2G_NotifyDragonGuildRedPoint());
	CProtocol::Register(new PtcM2G_GoalAwardsServerAwards());
	CProtocol::Register(new PtcM2G_BattleFiedlReadySceneGsNtf());
	CProtocol::Register(new PtcM2G_NpcFlNotifyM2G());
	CProtocol::Register(new PtcM2G_BattleFailInfoGsNtf());
	CProtocol::Register(new PtcC2G_UpdateFreeflowHintInfo());
	CProtocol::Register(new PtcM2G_ConsumeRebateNtf());
	CProtocol::Register(new PtcM2G_PayTotalNtf());
	CRpc::RegistRpc(RpcC2G_SyncTime::CreateRpc());
	CRpc::RegistRpc(RpcC2G_SkillLevelup::CreateRpc());
	CRpc::RegistRpc(RpcC2G_FetchAchivementReward::CreateRpc());
	CRpc::RegistRpc(RpcC2G_SelectChestReward::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ResetSkill::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ChooseProfession::CreateRpc());
	CRpc::RegistRpc(RpcC2G_BindSkill::CreateRpc());
	CRpc::RegistRpc(RpcC2G_TaskOperate::CreateRpc());
	CRpc::RegistRpc(RpcC2G_QueryLotteryCD::CreateRpc());
	CRpc::RegistRpc(RpcC2G_DrawLottery::CreateRpc());
	CRpc::RegistRpc(RpcC2G_EnhanceItem::CreateRpc());
	CRpc::RegistRpc(RpcC2G_QuerySceneDayCount::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GMCommand::CreateRpc());
	CRpc::RegistRpc(RpcC2G_DecomposeEquipment::CreateRpc());
	CRpc::RegistRpc(RpcC2G_EmblemLevelUp::CreateRpc());
	CRpc::RegistRpc(RpcC2G_UseItem::CreateRpc());
	CRpc::RegistRpc(RpcC2G_JadeOperation::CreateRpc());
	CRpc::RegistRpc(RpcC2G_JadeCompose::CreateRpc());
	CRpc::RegistRpc(RpcC2G_Checkin::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetActivityChest::CreateRpc());
	CRpc::RegistRpc(RpcC2G_FashionCompose::CreateRpc());
	CRpc::RegistRpc(RpcC2G_BuyJadeSlot::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetSystemReward::CreateRpc());
	CRpc::RegistRpc(RpcC2G_Sweep::CreateRpc());
	CRpc::RegistRpc(RpcC2G_Revive::CreateRpc());
	CRpc::RegistRpc(RpcC2G_QueryShopItem::CreateRpc());
	CRpc::RegistRpc(RpcC2G_BuyShopItem::CreateRpc());
	CRpc::RegistRpc(RpcC2G_BuyGoldAndFatigue::CreateRpc());
	CRpc::RegistRpc(RpcC2G_AddTempAttr::CreateRpc());
	CRpc::RegistRpc(RpcC2G_QueryOpenGameActivityTime::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ShowFlowerPage::CreateRpc());
	CRpc::RegistRpc(RpcC2G_SendFlower::CreateRpc());
	CRpc::RegistRpc(RpcC2G_FetchChapterChest::CreateRpc());
	CRpc::RegistRpc(RpcC2G_StartGuildCard::CreateRpc());
	CRpc::RegistRpc(RpcC2G_QueryGuildCard::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ChangeGuildCard::CreateRpc());
	CRpc::RegistRpc(RpcC2G_EndGuildCard::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetGuildCheckinBox::CreateRpc());
	CRpc::RegistRpc(RpcC2G_StageCountReset::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetGuildBonusList::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetGuildBonusDetailInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetGuildBonusReward::CreateRpc());
	CRpc::RegistRpc(RpcC2G_OpenSceneChest::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ReportBattle::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetOnlineReward::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetNextDayReward::CreateRpc());
	CRpc::RegistRpc(RpcC2G_FetchEnemyDoodadReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GuildGoblinInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2G_UseSupplement::CreateRpc());
	CRpc::RegistRpc(RpcC2G_SetRoleConfig::CreateRpc());
	CRpc::RegistRpc(RpcC2G_LevelUpSlotAttr::CreateRpc());
	CRpc::RegistRpc(RpcC2G_PetOperation::CreateRpc());
	CRpc::RegistRpc(RpcC2G_BuyTeamSceneCount::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ExpFindBack::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetTowerActivityTop::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetDesignationReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_SetDesignationReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetClassifyDesignationReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetAchieveBrifInfoReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetAchieveClassifyInfoReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ReqPlayerAutoPlay::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetAchieveRewardReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetLevelSealInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2G_LevelSealButtonStatus::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetAchievePointRewardReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ReqGetLoginReward::CreateRpc());
	CRpc::RegistRpc(RpcC2G_FiveDayRewardReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GuildCheckInBonusInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2G_SendGuildBonus::CreateRpc());
	CRpc::RegistRpc(RpcC2G_AskForCheckInBonus::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ThanksForBonus::CreateRpc());
	CRpc::RegistRpc(RpcC2G_PvpAllReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_AgreeQAReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetFlowerLeftTime::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetFlower::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetFlowerReward::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetFlowerRewardList::CreateRpc());
	CRpc::RegistRpc(RpcC2G_EnhanceTranster::CreateRpc());
	CRpc::RegistRpc(RpcC2G_PvpNowAllData::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetDragonTopInfo::CreateRpc());
	CRpc::RegistRpc(RpcG2M_RegisterGs2Ms::CreateRpc());
	CRpc::RegistRpc(RpcC2G_TeamInviteListReq::CreateRpc());
	CRpc::RegistRpc(RpcG2T_RegisterGs2Gt::CreateRpc());
	CRpc::RegistRpc(RpcT2G_RegisterGt2Gs::CreateRpc());
	CRpc::RegistRpc(RpcN2G_EnterScene::CreateRpc());
	CRpc::RegistRpc(RpcN2G_LeaveScene::CreateRpc());
	CRpc::RegistRpc(RpcG2D_SaveRoleData::CreateRpc());
	CRpc::RegistRpc(RpcC2G_IdentifyEmblem::CreateRpc());
	CRpc::RegistRpc(RpcC2G_OpenGuildQAReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetGuildQADataReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_MulActivityReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetWatchInfoByID::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetMyWatchRecord::CreateRpc());
	CRpc::RegistRpc(RpcC2G_EnterWatchBattle::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ChangeWatchRole::CreateRpc());
	CRpc::RegistRpc(RpcC2G_CommendWatchBattle::CreateRpc());
	CRpc::RegistRpc(RpcC2G_BossRushReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_DoEnterScene::CreateRpc());
	CRpc::RegistRpc(RpcG2N_ChangeSceneVerify::CreateRpc());
	CRpc::RegistRpc(RpcC2G_TakeOffAllJade::CreateRpc());
	CRpc::RegistRpc(RpcC2G_SweepTower::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ResetTower::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GSHttpCommandNew::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetGoddessTrialRewards::CreateRpc());
	CRpc::RegistRpc(RpcN2G_CreateBattle::CreateRpc());
	CRpc::RegistRpc(RpcM2G_ChatCanSend::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GetWatchInfoByType::CreateRpc());
	CRpc::RegistRpc(RpcG2M_EnterWatchBattleMs::CreateRpc());
	CRpc::RegistRpc(RpcG2M_IncLevelSealBossCount::CreateRpc());
	CRpc::RegistRpc(RpcM2G_NoticeGuildBossDoEndToGSN::CreateRpc());
	CRpc::RegistRpc(RpcC2G_QueryPowerPoint::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetPayAllInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2G_PayCardAward::CreateRpc());
	CRpc::RegistRpc(RpcC2G_PayClick::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetActivityInfo::CreateRpc());
	CRpc::RegistRpc(RpcG2M_MSGetFlwoerRankReward::CreateRpc());
	CRpc::RegistRpc(RpcG2M_MSSendFlower::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GetFlowerRewardState::CreateRpc());
	CRpc::RegistRpc(RpcM2G_TeamSynM2C::CreateRpc());
	CRpc::RegistRpc(RpcM2G_TeamStartBattleM2G::CreateRpc());
	CRpc::RegistRpc(RpcM2G_TeamRequestM2G::CreateRpc());
	CRpc::RegistRpc(RpcC2G_SynPetInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ChooseRollReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_PayFirstAward::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GrowthFundAward::CreateRpc());
	CRpc::RegistRpc(RpcC2G_BuyVipLevelGift::CreateRpc());
	CRpc::RegistRpc(RpcC2G_TitleLevelUp::CreateRpc());
	CRpc::RegistRpc(RpcC2G_FirstPassInfoReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetFirstPassReward::CreateRpc());
	CRpc::RegistRpc(RpcC2G_CommendFirstPass::CreateRpc());
	CRpc::RegistRpc(RpcG2M_MSCommendFirstPass::CreateRpc());
	CRpc::RegistRpc(RpcC2G_FirstPassGetTopRoleInfo::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GetRankInfo::CreateRpc());
	CRpc::RegistRpc(RpcG2M_PvpAllG2MReq::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GMCommandG2M::CreateRpc());
	CRpc::RegistRpc(RpcM2G_MSUseSummaryReq::CreateRpc());
	CRpc::RegistRpc(RpcM2G_TeamChangeExpCheck::CreateRpc());
	CRpc::RegistRpc(RpcG2M_UpdateMSRankList::CreateRpc());
	CRpc::RegistRpc(RpcG2M_PkRankReq::CreateRpc());
	CRpc::RegistRpc(RpcM2G_PkReqM2G::CreateRpc());
	CRpc::RegistRpc(RpcM2G_CreateGuildCost::CreateRpc());
	CRpc::RegistRpc(RpcM2G_AuctionAllM2GReq::CreateRpc());
	CRpc::RegistRpc(RpcM2G_EnterGuildPower::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ChangeOutLookOp::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ItemFindBackInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ItemFindBack::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ActivatAtlas::CreateRpc());
	CRpc::RegistRpc(RpcC2G_AutoBreakAtlas::CreateRpc());
	CRpc::RegistRpc(RpcC2G_breakAtlas::CreateRpc());
	CRpc::RegistRpc(RpcC2G_BuyIBItem::CreateRpc());
	CRpc::RegistRpc(RpcC2G_QueryIBItem::CreateRpc());
	CRpc::RegistRpc(RpcC2G_SpriteOperation::CreateRpc());
	CRpc::RegistRpc(RpcC2G_DEProgressReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetRiskMapInfos::CreateRpc());
	CRpc::RegistRpc(RpcC2G_PlayDiceRequest::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ChangeRiskBoxState::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ReEnterRiskBattle::CreateRpc());
	CRpc::RegistRpc(RpcC2G_LevelSealExchange::CreateRpc());
	CRpc::RegistRpc(RpcC2G_PlayDiceOver::CreateRpc());
	CRpc::RegistRpc(RpcG2M_OpGuildReqData::CreateRpc());
	CRpc::RegistRpc(RpcC2G_RiskBuyRequest::CreateRpc());
	CRpc::RegistRpc(RpcC2G_SmeltItem::CreateRpc());
	CRpc::RegistRpc(RpcM2G_StartArenaGs::CreateRpc());
	CRpc::RegistRpc(RpcC2G_RefreshSweepReward::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetSweepTowerReward::CreateRpc());
	CRpc::RegistRpc(RpcM2G_ReqGuildDareRewardRec::CreateRpc());
	CRpc::RegistRpc(RpcG2M_CheckGuildDareReward::CreateRpc());
	CRpc::RegistRpc(RpcC2G_LearnGuildSkill::CreateRpc());
	CRpc::RegistRpc(RpcG2M_EndAreanMs::CreateRpc());
	CRpc::RegistRpc(RpcM2G_ReqRoleGuildSkill::CreateRpc());
	CRpc::RegistRpc(RpcG2M_MGuildBonusOperateReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_QuerySceneTime::CreateRpc());
	CRpc::RegistRpc(RpcM2G_ReqGuildActivity::CreateRpc());
	CRpc::RegistRpc(RpcM2G_ReqGuildCheckin::CreateRpc());
	CRpc::RegistRpc(RpcM2G_NoticeGuildCheckin::CreateRpc());
	CRpc::RegistRpc(RpcG2M_NotifyCheckinBox::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GuildCardSyncMs::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetTowerFirstPassReward::CreateRpc());
	CRpc::RegistRpc(RpcM2G_ReqRecvFatigue::CreateRpc());
	CRpc::RegistRpc(RpcM2G_DesignationEventReq::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GsReload::CreateRpc());
	CRpc::RegistRpc(RpcG2M_ReqGuildLadderReward::CreateRpc());
	CRpc::RegistRpc(RpcM2G_MSConsumeItems::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GmfReadyReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_InspireReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ReqGuildBossTimeLeft::CreateRpc());
	CRpc::RegistRpc(RpcC2G_SkyCityAllInfoReq::CreateRpc());
	CRpc::RegistRpc(RpcG2M_EnterQARoom::CreateRpc());
	CRpc::RegistRpc(RpcG2M_CreateGuildQARoom::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetSpActivityReward::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetSpActivityBigPrize::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GardenPlayEvent::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GardenPlantChangeStage::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GetGuildLadderRewardTime::CreateRpc());
	CRpc::RegistRpc(RpcT2G_ReconnectGs::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetLevelSealSelfGift::CreateRpc());
	CRpc::RegistRpc(RpcM2G_QueryRoleCardMatch::CreateRpc());
	CRpc::RegistRpc(RpcG2R_RegisterGs2Router::CreateRpc());
	CRpc::RegistRpc(RpcG2K_RegisterGs2World::CreateRpc());
	CRpc::RegistRpc(RpcK2G_CreateBattleCross::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GoldClick::CreateRpc());
	CRpc::RegistRpc(RpcG2N_RegisterGs2Ns::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GiveMailPrize::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GuildAuctReqM2C::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ResWarAllInfoReqOne::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GuildAuctFailBack::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GSDesignationEventReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ItemBuffOp::CreateRpc());
	CRpc::RegistRpc(RpcC2G_JustDance::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetDailyTaskInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetDailyTaskReward::CreateRpc());
	CRpc::RegistRpc(RpcC2G_DailyTaskAskHelp::CreateRpc());
	CRpc::RegistRpc(RpcG2M_AskGuildHelp::CreateRpc());
	CRpc::RegistRpc(RpcM2G_CostDonateItem::CreateRpc());
	CRpc::RegistRpc(RpcM2G_QueryDonateInfo::CreateRpc());
	CRpc::RegistRpc(RpcM2G_tryAddGuildInherit::CreateRpc());
	CRpc::RegistRpc(RpcC2G_QueryQQVipInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2G_DailyTaskGiveUp::CreateRpc());
	CRpc::RegistRpc(RpcM2G_CrashRecoverM2G::CreateRpc());
	CRpc::RegistRpc(RpcM2G_FetchRoleInfoOnMs::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ResWarBuff::CreateRpc());
	CRpc::RegistRpc(RpcG2M_TryTeamCostReward::CreateRpc());
	CRpc::RegistRpc(RpcM2G_ChangeNameGs::CreateRpc());
	CRpc::RegistRpc(RpcC2G_QueryQQFriendsVipInfo::CreateRpc());
	CRpc::RegistRpc(RpcM2G_ReqGuildInheritTimes::CreateRpc());
	CRpc::RegistRpc(RpcC2G_EnchantEquip::CreateRpc());
	CRpc::RegistRpc(RpcM2G_MakePartnerM2G::CreateRpc());
	CRpc::RegistRpc(RpcC2G_TakePartnerChest::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GetPartnerInfoG2M::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GetPartnerRecordM2G::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetHeroBattleInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2G_HorseReConnect::CreateRpc());
	CRpc::RegistRpc(RpcC2G_PhotographEffect::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GetCharm::CreateRpc());
	CRpc::RegistRpc(RpcM2G_InvFightM2GReq::CreateRpc());
	CRpc::RegistRpc(RpcM2G_BuyPartnerItemM2G::CreateRpc());
	CRpc::RegistRpc(RpcM2G_PokerTournamentSignUpCost::CreateRpc());
	CRpc::RegistRpc(RpcC2G_PandoraLottery::CreateRpc());
	CRpc::RegistRpc(RpcC2G_QueryClientIp::CreateRpc());
	CRpc::RegistRpc(RpcC2G_SetHeroInHeroBattle::CreateRpc());
	CRpc::RegistRpc(RpcM2G_PayGetBalance::CreateRpc());
	CRpc::RegistRpc(RpcM2G_PayConsumeAddGoods::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetGuildBonusLeft::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetHeroBattleGameRecord::CreateRpc());
	CRpc::RegistRpc(RpcC2G_BuyHeroInHeroBattle::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetHeroBattleWeekReward::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GCFCommonReqM2G::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GetGardenInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2G_AtlasUpStar::CreateRpc());
	CRpc::RegistRpc(RpcC2G_LeagueBattleReadyReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ForgeEquip::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ChangeProfession::CreateRpc());
	CRpc::RegistRpc(RpcG2M_LeagueRankUpdateG2M::CreateRpc());
	CRpc::RegistRpc(RpcG2M_ChangeProfessionMs::CreateRpc());
	CRpc::RegistRpc(RpcG2D_ReadHiddenRoles::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetHolidayStageInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ItemSell::CreateRpc());
	CRpc::RegistRpc(RpcC2G_EnchantTransfer::CreateRpc());
	CRpc::RegistRpc(RpcC2G_PersonalCareer::CreateRpc());
	CRpc::RegistRpc(RpcG2M_PersonalCareerTotalInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ChangeDeclaration::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ChangeSkillSet::CreateRpc());
	CRpc::RegistRpc(RpcG2K_LeagueRankUpdateG2K::CreateRpc());
	CRpc::RegistRpc(RpcM2G_CheckNotice::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetPayReward::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GetMulActOpenInfo::CreateRpc());
	CRpc::RegistRpc(RpcK2G_CrossGsReload::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GetMilitaryRank::CreateRpc());
	CRpc::RegistRpc(RpcC2G_PayFriendItem::CreateRpc());
	CRpc::RegistRpc(RpcG2M_UpPayFriendItem::CreateRpc());
	CRpc::RegistRpc(RpcG2M_SyncActivityEnd2MS::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ItemCompose::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetBackFlowSpActTime::CreateRpc());
	CRpc::RegistRpc(RpcM2G_WorldBossGuildAddAttrGs::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ArgentaActivity::CreateRpc());
	CRpc::RegistRpc(RpcM2G_ModifyGsGuildName::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetPlatformShareChest::CreateRpc());
	CRpc::RegistRpc(RpcC2G_LoginActivity::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetGrowthFundTime::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GiftIbItemReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ActivateFashionCharm::CreateRpc());
	CRpc::RegistRpc(RpcG2M_PayDegreeReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetGuildCamPartyRandItem::CreateRpc());
	CRpc::RegistRpc(RpcC2G_TalkGuildCampSprite::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GuildCampPartyReqExchange::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetGuildCampPartyExchangeInfo::CreateRpc());
	CRpc::RegistRpc(RpcM2G_CustomBattleGsOp::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GuildCampExchangeOperate::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetGuildCampPartyReward::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GiftIbReqGSGoods::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ArtifactCompose::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ReplyPartyExchangeItemOpt::CreateRpc());
	CRpc::RegistRpc(RpcC2G_AbsEnterScene::CreateRpc());
	CRpc::RegistRpc(RpcC2G_EnchantActiveAttribute::CreateRpc());
	CRpc::RegistRpc(RpcC2G_BuyExtraSkillEmblemSlot::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ChangeLiveVisible::CreateRpc());
	CRpc::RegistRpc(RpcG2M_CustomBattleResultMs::CreateRpc());
	CRpc::RegistRpc(RpcG2K_CustomBattleResultWorld::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ReportBadPlayer::CreateRpc());
	CRpc::RegistRpc(RpcM2G_TeamTranMs2Gs::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetGuildPartyReceiveInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ReturnSmeltStone::CreateRpc());
	CRpc::RegistRpc(RpcM2G_KMatchCommonReqM2C::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetPlatShareAward::CreateRpc());
	CRpc::RegistRpc(RpcC2G_WeekEnd4v4GetInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2G_SceneMobaOp::CreateRpc());
	CRpc::RegistRpc(RpcC2G_MobaSignaling::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ActivateHairColor::CreateRpc());
	CRpc::RegistRpc(RpcM2G_TryTeacherInheritNew::CreateRpc());
	CRpc::RegistRpc(RpcC2G_DHRReqC2G::CreateRpc());
	CRpc::RegistRpc(RpcC2G_PetOperationOther::CreateRpc());
	CRpc::RegistRpc(RpcN2G_NotifyRoleLoginReconnect2Gs::CreateRpc());
	CRpc::RegistRpc(RpcG2K_ReportBMRoleInfo::CreateRpc());
	CRpc::RegistRpc(RpcM2G_MsGiveItem::CreateRpc());
	CRpc::RegistRpc(RpcM2G_TeamStartBattleM2GCross::CreateRpc());
	CRpc::RegistRpc(RpcC2G_BuySpriteEgg::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetEnhanceAttr::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetAncientTimesAward::CreateRpc());
	CRpc::RegistRpc(RpcC2G_SelectHeroAncientPower::CreateRpc());
	CRpc::RegistRpc(RpcM2G_CheckMarriageCouple::CreateRpc());
	CRpc::RegistRpc(RpcC2G_WeddingOperator::CreateRpc());
	CRpc::RegistRpc(RpcC2G_TactEnterScene::CreateRpc());
	CRpc::RegistRpc(RpcC2G_DragonGroupRoleList::CreateRpc());
	CRpc::RegistRpc(RpcC2G_DragonGroupRecord::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GiveMarriageLvRerewardM2G::CreateRpc());
	CRpc::RegistRpc(RpcM2G_Holdweddingcheckm2g::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ArtifactDeityStoveOp::CreateRpc());
	CRpc::RegistRpc(RpcM2G_WeddingOperM2G::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ThemeActivityHint::CreateRpc());
	CRpc::RegistRpc(RpcC2G_BackFlowBuyOp::CreateRpc());
	CRpc::RegistRpc(RpcC2G_TransformOp::CreateRpc());
	CRpc::RegistRpc(RpcC2G_UpgradeEquip::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetFashionSynthesisInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2G_FashionIBShopBuy::CreateRpc());
	CRpc::RegistRpc(RpcM2G_CreateDragonGuildCost::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GetRolePower::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GetDragonGuildRecordM2G::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GetDragonGuildInfoG2M::CreateRpc());
	CRpc::RegistRpc(RpcC2G_TakeDragonGuildChest::CreateRpc());
	CRpc::RegistRpc(RpcM2G_ModifyDragonGuildNameCost::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetDragonGuildTaskChest::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GetDragonGuildTaskChestG2M::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GetDragonGuildRecieveInfo::CreateRpc());
	CRpc::RegistRpc(RpcM2G_BuyDragonGuildItemM2G::CreateRpc());
	CRpc::RegistRpc(RpcG2M_CheckDragonGuildRedPointG2M::CreateRpc());
	CRpc::RegistRpc(RpcC2G_BattleFieldAwardNumReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_BattleFieldRankReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetWeeklyTaskInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetWeeklyTaskReward::CreateRpc());
	CRpc::RegistRpc(RpcC2G_RefreshWeeklyTask::CreateRpc());
	CRpc::RegistRpc(RpcC2G_BattleFieldRoleAgainstReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetCompeteDragonInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2G_SetPreShow::CreateRpc());
	CRpc::RegistRpc(RpcC2G_FuseEquip::CreateRpc());
	CRpc::RegistRpc(RpcC2G_TurnOverWeeklyTaskItem::CreateRpc());
	CRpc::RegistRpc(RpcG2M_TaskDonateG2M::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetLuckyActivityInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2G_BuyDraw::CreateRpc());
	CRpc::RegistRpc(RpcC2G_LotteryDraw::CreateRpc());
	CRpc::RegistRpc(RpcM2G_NpcFlReqM2G::CreateRpc());
	CRpc::RegistRpc(RpcG2K_BMRankListReq::CreateRpc());
	CRpc::RegistRpc(RpcM2G_DailyTaskRefreshM2G::CreateRpc());
	CRpc::RegistRpc(RpcG2M_DailyTaskRefreshG2M::CreateRpc());
	CRpc::RegistRpc(RpcC2G_SurviveReqC2G::CreateRpc());
	CRpc::RegistRpc(RpcG2M_SurviveSynPointG2M::CreateRpc());
	CRpc::RegistRpc(RpcC2G_DoodadItemAllSkillReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_PeerBox::CreateRpc());
	CRpc::RegistRpc(RpcC2G_QueryBoxs::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ActivatePreShow::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GuildPartySummonSpirit::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GetDailyTaskG2M::CreateRpc());
	CRpc::RegistRpc(RpcC2G_ChooseSpecialEffects::CreateRpc());
	CRpc::RegistRpc(RpcG2K_TeamGoToNextSceneReq::CreateRpc());
	CRpc::RegistRpc(RpcC2G_EnterLeisureScene::CreateRpc());
	CRpc::RegistRpc(RpcC2G_BackFlowActivityOperation::CreateRpc());
	CRpc::RegistRpc(RpcC2G_GetNewZoneBenefit::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GetNewZoneBenefitG2M::CreateRpc());
	CRpc::RegistRpc(RpcC2G_SelectChargeBackRole::CreateRpc());


	CProtocol::RegistProtoName(PTCT2G_REGISTSERVERNTF_TYPE, "PTCT2G_REGISTSERVERNTF");
	CProtocol::RegistProtoName(PTCC2G_ENTERSCENEREQ_TYPE, "PTCC2G_ENTERSCENEREQ");
	CProtocol::RegistProtoName(PTCC2G_LEAVESCENEREQ_TYPE, "PTCC2G_LEAVESCENEREQ");
	CProtocol::RegistProtoName(PTCC2G_MOVEOPERATIONREQ_TYPE, "PTCC2G_MOVEOPERATIONREQ");
	CProtocol::RegistProtoName(PTCC2G_CASTSKILL_TYPE, "PTCC2G_CASTSKILL");
	CProtocol::RegistProtoName(PTCC2G_UPDATETUTORIAL_TYPE, "PTCC2G_UPDATETUTORIAL");
	CProtocol::RegistProtoName(PTCC2G_ROLEDEATHINSOLOSCENE_TYPE, "PTCC2G_ROLEDEATHINSOLOSCENE");
	CProtocol::RegistProtoName(PTCC2G_ENTERSCENECOOLDOWNQUERY_TYPE, "PTCC2G_ENTERSCENECOOLDOWNQUERY");
	CProtocol::RegistProtoName(PTCC2G_OPERATERECORDNTF_TYPE, "PTCC2G_OPERATERECORDNTF");
	CProtocol::RegistProtoName(PTCC2G_ONLINEREWARDREPORT_TYPE, "PTCC2G_ONLINEREWARDREPORT");
	CProtocol::RegistProtoName(PTCC2G_NEXTDAYREWARDREPORT_TYPE, "PTCC2G_NEXTDAYREWARDREPORT");
	CProtocol::RegistProtoName(PTCC2G_DELAYNOTIFY_TYPE, "PTCC2G_DELAYNOTIFY");
	CProtocol::RegistProtoName(PTCC2G_SKILLRESULTREQ_TYPE, "PTCC2G_SKILLRESULTREQ");
	CProtocol::RegistProtoName(PTCC2G_SKILLBULLETFIREREPORT_TYPE, "PTCC2G_SKILLBULLETFIREREPORT");
	CProtocol::RegistProtoName(PTCC2G_SKILLBULLETRESULTREQ_TYPE, "PTCC2G_SKILLBULLETRESULTREQ");
	CProtocol::RegistProtoName(PTCC2G_ADDLEVELSCRIPTEXTSTRING_TYPE, "PTCC2G_ADDLEVELSCRIPTEXTSTRING");
	CProtocol::RegistProtoName(PTCC2G_SCENEDAMAGERANKREPORT_TYPE, "PTCC2G_SCENEDAMAGERANKREPORT");
	CProtocol::RegistProtoName(PTCT2G_RECONNECTNTF_TYPE, "PTCT2G_RECONNECTNTF");
	CProtocol::RegistProtoName(PTCC2G_CHANGESUPPLEMENTREPORT_TYPE, "PTCC2G_CHANGESUPPLEMENTREPORT");
	CProtocol::RegistProtoName(PTCC2G_QTEOPERATION_TYPE, "PTCC2G_QTEOPERATION");
	CProtocol::RegistProtoName(PTCC2G_AUTOFIGHTNTF_TYPE, "PTCC2G_AUTOFIGHTNTF");
	CProtocol::RegistProtoName(PTCC2G_SYNCSCENEFINISH_TYPE, "PTCC2G_SYNCSCENEFINISH");
	CProtocol::RegistProtoName(PTCC2G_TSSSDKSENDANTI2SERVER_TYPE, "PTCC2G_TSSSDKSENDANTI2SERVER");
	CProtocol::RegistProtoName(PTCC2G_FMBREFUSE_TYPE, "PTCC2G_FMBREFUSE");
	CProtocol::RegistProtoName(PTCN2G_NEWGATECONNECTED_TYPE, "PTCN2G_NEWGATECONNECTED");
	CProtocol::RegistProtoName(PTCM2G_UPDATEGUILDQAROOM_TYPE, "PTCM2G_UPDATEGUILDQAROOM");
	CProtocol::RegistProtoName(PTCD2G_D2GDESIGNATIONNTF_TYPE, "PTCD2G_D2GDESIGNATIONNTF");
	CProtocol::RegistProtoName(PTCM2G_MSLEVELSEALNTF_TYPE, "PTCM2G_MSLEVELSEALNTF");
	CProtocol::RegistProtoName(PTCM2G_NOTICEGUILDBOSSENDTOGSN_TYPE, "PTCM2G_NOTICEGUILDBOSSENDTOGSN");
	CProtocol::RegistProtoName(PTCM2G_UPDATEFRIENDONGS_TYPE, "PTCM2G_UPDATEFRIENDONGS");
	CProtocol::RegistProtoName(PTCM2G_NOTIFYWATCHICONNUM_TYPE, "PTCM2G_NOTIFYWATCHICONNUM");
	CProtocol::RegistProtoName(PTCM2G_SYNSCENEREWARDM2G_TYPE, "PTCM2G_SYNSCENEREWARDM2G");
	CProtocol::RegistProtoName(PTCM2G_CHANGEGUILDBOSSTIME_TYPE, "PTCM2G_CHANGEGUILDBOSSTIME");
	CProtocol::RegistProtoName(PTCM2G_WORLDBOSSSYNCGS_TYPE, "PTCM2G_WORLDBOSSSYNCGS");
	CProtocol::RegistProtoName(PTCC2G_JOINROOM_TYPE, "PTCC2G_JOINROOM");
	CProtocol::RegistProtoName(PTCC2G_QUITROOM_TYPE, "PTCC2G_QUITROOM");
	CProtocol::RegistProtoName(PTCM2G_FIRSTPASSNEWCOMMENDREWARDNTF_TYPE, "PTCM2G_FIRSTPASSNEWCOMMENDREWARDNTF");
	CProtocol::RegistProtoName(PTCM2G_SYNROLEINGUILD_TYPE, "PTCM2G_SYNROLEINGUILD");
	CProtocol::RegistProtoName(PTCM2G_SYNGUILDINFO_TYPE, "PTCM2G_SYNGUILDINFO");
	CProtocol::RegistProtoName(PTCC2G_PLAYDICEOVER_TYPE, "PTCC2G_PLAYDICEOVER");
	CProtocol::RegistProtoName(PTCC2G_REMOVEIBSHOPICON_TYPE, "PTCC2G_REMOVEIBSHOPICON");
	CProtocol::RegistProtoName(PTCC2G_SETVOIPMEMBERSTATE_TYPE, "PTCC2G_SETVOIPMEMBERSTATE");
	CProtocol::RegistProtoName(PTCM2G_SYNCARENAGS_TYPE, "PTCM2G_SYNCARENAGS");
	CProtocol::RegistProtoName(PTCM2G_SYNGUILDSIMPLEDATA_TYPE, "PTCM2G_SYNGUILDSIMPLEDATA");
	CProtocol::RegistProtoName(PTCC2G_OPENSPRITEEGGNTF_TYPE, "PTCC2G_OPENSPRITEEGGNTF");
	CProtocol::RegistProtoName(PTCM2G_MNEWGUILDBONUSNTF_TYPE, "PTCM2G_MNEWGUILDBONUSNTF");
	CProtocol::RegistProtoName(PTCM2G_NOTIFYGUILDCHECKINPROCESS_TYPE, "PTCM2G_NOTIFYGUILDCHECKINPROCESS");
	CProtocol::RegistProtoName(PTCM2G_SYNFATIGUERESULT_TYPE, "PTCM2G_SYNFATIGUERESULT");
	CProtocol::RegistProtoName(PTCM2G_SYNGUILDCHECKINHINT_TYPE, "PTCM2G_SYNGUILDCHECKINHINT");
	CProtocol::RegistProtoName(PTCM2G_GIVEANSERQAREWARDNTF_TYPE, "PTCM2G_GIVEANSERQAREWARDNTF");
	CProtocol::RegistProtoName(PTCM2G_QAOVERTOGS_TYPE, "PTCM2G_QAOVERTOGS");
	CProtocol::RegistProtoName(PTCM2G_GARDENEVENTNTF_TYPE, "PTCM2G_GARDENEVENTNTF");
	CProtocol::RegistProtoName(PTCM2G_ROLEEVENTSYNCGS_TYPE, "PTCM2G_ROLEEVENTSYNCGS");
	CProtocol::RegistProtoName(PTCK2G_NEWROUTERNTF_TYPE, "PTCK2G_NEWROUTERNTF");
	CProtocol::RegistProtoName(PTCC2G_BATTLELOGREPORT_TYPE, "PTCC2G_BATTLELOGREPORT");
	CProtocol::RegistProtoName(PTCC2G_CLICKGUILDCAMP_TYPE, "PTCC2G_CLICKGUILDCAMP");
	CProtocol::RegistProtoName(PTCM2G_SYNCSERVERTIME_TYPE, "PTCM2G_SYNCSERVERTIME");
	CProtocol::RegistProtoName(PTCM2G_WORLDLEVELNTF_TYPE, "PTCM2G_WORLDLEVELNTF");
	CProtocol::RegistProtoName(PTCR2G_NOTIFYSERVERCLOSE_TYPE, "PTCR2G_NOTIFYSERVERCLOSE");
	CProtocol::RegistProtoName(PTCC2G_SYNDOINGGUILDINHERIT_TYPE, "PTCC2G_SYNDOINGGUILDINHERIT");
	CProtocol::RegistProtoName(PTCM2G_NOTICEGUILDINHERITNUMINFO_TYPE, "PTCM2G_NOTICEGUILDINHERITNUMINFO");
	CProtocol::RegistProtoName(PTCN2G_UPDATEFUNCTIONOPEN2GS_TYPE, "PTCN2G_UPDATEFUNCTIONOPEN2GS");
	CProtocol::RegistProtoName(PTCR2G_MSCRASHRECOVERR2G_TYPE, "PTCR2G_MSCRASHRECOVERR2G");
	CProtocol::RegistProtoName(PTCM2G_MSDESTORYSCENE_TYPE, "PTCM2G_MSDESTORYSCENE");
	CProtocol::RegistProtoName(PTCC2G_PICURLNTF_TYPE, "PTCC2G_PICURLNTF");
	CProtocol::RegistProtoName(PTCM2G_UPDATEPARTNERTOGS_TYPE, "PTCM2G_UPDATEPARTNERTOGS");
	CProtocol::RegistProtoName(PTCM2G_GCFSYNM2G_TYPE, "PTCM2G_GCFSYNM2G");
	CProtocol::RegistProtoName(PTCN2G_UPDATESTARTUPTYPEN2GNTF_TYPE, "PTCN2G_UPDATESTARTUPTYPEN2GNTF");
	CProtocol::RegistProtoName(PTCM2G_TRYTEACHERINHERIT_TYPE, "PTCM2G_TRYTEACHERINHERIT");
	CProtocol::RegistProtoName(PTCM2G_BALANCEINFONTF_TYPE, "PTCM2G_BALANCEINFONTF");
	CProtocol::RegistProtoName(PTCR2G_NOTIFYSERVERCONNECT_TYPE, "PTCR2G_NOTIFYSERVERCONNECT");
	CProtocol::RegistProtoName(PTCC2G_QUERYROLESTATEREQ_TYPE, "PTCC2G_QUERYROLESTATEREQ");
	CProtocol::RegistProtoName(PTCC2G_CANCELREDDOTREQ_TYPE, "PTCC2G_CANCELREDDOTREQ");
	CProtocol::RegistProtoName(PTCM2G_SYNGUILDBOSSHP_TYPE, "PTCM2G_SYNGUILDBOSSHP");
	CProtocol::RegistProtoName(PTCM2G_SYNGUILDBOSSDIE_TYPE, "PTCM2G_SYNGUILDBOSSDIE");
	CProtocol::RegistProtoName(PTCM2G_SYNGUILDBOSSDOEND_TYPE, "PTCM2G_SYNGUILDBOSSDOEND");
	CProtocol::RegistProtoName(PTCM2G_NOTICEGUILDBOSSATTR_TYPE, "PTCM2G_NOTICEGUILDBOSSATTR");
	CProtocol::RegistProtoName(PTCM2G_CONTROLFUNCTIONNTF_TYPE, "PTCM2G_CONTROLFUNCTIONNTF");
	CProtocol::RegistProtoName(PTCM2G_CLICKNOTICENTF_TYPE, "PTCM2G_CLICKNOTICENTF");
	CProtocol::RegistProtoName(PTCK2G_SYNCSERVERTIME2CROSSGS_TYPE, "PTCK2G_SYNCSERVERTIME2CROSSGS");
	CProtocol::RegistProtoName(PTCC2G_CLOSEHINTNTF_TYPE, "PTCC2G_CLOSEHINTNTF");
	CProtocol::RegistProtoName(PTCK2G_BROADCASTWORLD2GS_TYPE, "PTCK2G_BROADCASTWORLD2GS");
	CProtocol::RegistProtoName(PTCM2G_MILITARYRANKCHANGE_TYPE, "PTCM2G_MILITARYRANKCHANGE");
	CProtocol::RegistProtoName(PTCM2G_BUYPAYAILEENNTF_TYPE, "PTCM2G_BUYPAYAILEENNTF");
	CProtocol::RegistProtoName(PTCM2G_WORLDBOSSGUILDADDATTRSYNCGSNTF_TYPE, "PTCM2G_WORLDBOSSGUILDADDATTRSYNCGSNTF");
	CProtocol::RegistProtoName(PTCM2G_ENTERSCENEFROMMS_TYPE, "PTCM2G_ENTERSCENEFROMMS");
	CProtocol::RegistProtoName(PTCM2G_BROADCASTGLOBALVALUE_TYPE, "PTCM2G_BROADCASTGLOBALVALUE");
	CProtocol::RegistProtoName(PTCC2G_LOGINACTIVITYAWARDREQ_TYPE, "PTCC2G_LOGINACTIVITYAWARDREQ");
	CProtocol::RegistProtoName(PTCM2G_PKUPDATESYNNTF_TYPE, "PTCM2G_PKUPDATESYNNTF");
	CProtocol::RegistProtoName(PTCC2G_NOTIFYPLATSHARERESULT_TYPE, "PTCC2G_NOTIFYPLATSHARERESULT");
	CProtocol::RegistProtoName(PTCM2G_ADDMILITARYRANK_TYPE, "PTCM2G_ADDMILITARYRANK");
	CProtocol::RegistProtoName(PTCK2G_SERVERCROSSDATAW2CROSSG_TYPE, "PTCK2G_SERVERCROSSDATAW2CROSSG");
	CProtocol::RegistProtoName(PTCM2G_TEAMTRANMS2GS_TYPE, "PTCM2G_TEAMTRANMS2GS");
	CProtocol::RegistProtoName(PTCC2G_BATTLESTATISTICSREPORT_TYPE, "PTCC2G_BATTLESTATISTICSREPORT");
	CProtocol::RegistProtoName(PTCC2G_SHAREREPORT_TYPE, "PTCC2G_SHAREREPORT");
	CProtocol::RegistProtoName(PTCM2G_WEDDINGCARENDM2G_TYPE, "PTCM2G_WEDDINGCARENDM2G");
	CProtocol::RegistProtoName(PTCK2G_BIGMELEEENDNTF_TYPE, "PTCK2G_BIGMELEEENDNTF");
	CProtocol::RegistProtoName(PTCC2G_INVFIGHTAGAINREQC2G_TYPE, "PTCC2G_INVFIGHTAGAINREQC2G");
	CProtocol::RegistProtoName(PTCM2G_SYNDRAGONGUILDSIMPLEDATA_TYPE, "PTCM2G_SYNDRAGONGUILDSIMPLEDATA");
	CProtocol::RegistProtoName(PTCM2G_NOTIFYDRAGONGUILDREDPOINT_TYPE, "PTCM2G_NOTIFYDRAGONGUILDREDPOINT");
	CProtocol::RegistProtoName(PTCM2G_GOALAWARDSSERVERAWARDS_TYPE, "PTCM2G_GOALAWARDSSERVERAWARDS");
	CProtocol::RegistProtoName(PTCM2G_BATTLEFIEDLREADYSCENEGSNTF_TYPE, "PTCM2G_BATTLEFIEDLREADYSCENEGSNTF");
	CProtocol::RegistProtoName(PTCM2G_NPCFLNOTIFYM2G_TYPE, "PTCM2G_NPCFLNOTIFYM2G");
	CProtocol::RegistProtoName(PTCM2G_BATTLEFAILINFOGSNTF_TYPE, "PTCM2G_BATTLEFAILINFOGSNTF");
	CProtocol::RegistProtoName(PTCC2G_UPDATEFREEFLOWHINTINFO_TYPE, "PTCC2G_UPDATEFREEFLOWHINTINFO");
	CProtocol::RegistProtoName(PTCM2G_CONSUMEREBATENTF_TYPE, "PTCM2G_CONSUMEREBATENTF");
	CProtocol::RegistProtoName(PTCM2G_PAYTOTALNTF_TYPE, "PTCM2G_PAYTOTALNTF");
	CProtocol::RegistProtoName(PTCG2C_NOTIFYACCOUNTDATA_TYPE, "PTCG2C_NOTIFYACCOUNTDATA");
	CProtocol::RegistProtoName(PTCG2D_REGISTGAMESERVER_TYPE, "PTCG2D_REGISTGAMESERVER");
	CProtocol::RegistProtoName(PTCG2C_ENTERSCENENTF_TYPE, "PTCG2C_ENTERSCENENTF");
	CProtocol::RegistProtoName(PTCG2C_LEAVESCENENTF_TYPE, "PTCG2C_LEAVESCENENTF");
	CProtocol::RegistProtoName(PTCG2C_UNITAPPEAR_TYPE, "PTCG2C_UNITAPPEAR");
	CProtocol::RegistProtoName(PTCG2C_UNITDISAPPEAR_TYPE, "PTCG2C_UNITDISAPPEAR");
	CProtocol::RegistProtoName(PTCG2D_UPDATEFIELD_TYPE, "PTCG2D_UPDATEFIELD");
	CProtocol::RegistProtoName(PTCG2C_SKILLRESULT_TYPE, "PTCG2C_SKILLRESULT");
	CProtocol::RegistProtoName(PTCG2C_DEATHNOTIFY_TYPE, "PTCG2C_DEATHNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_ATTRIBUTECHANGENOTIFY_TYPE, "PTCG2C_ATTRIBUTECHANGENOTIFY");
	CProtocol::RegistProtoName(PTCG2C_LEVELCHANGENTF_TYPE, "PTCG2C_LEVELCHANGENTF");
	CProtocol::RegistProtoName(PTCG2C_ITEMCHANGEDNTF_TYPE, "PTCG2C_ITEMCHANGEDNTF");
	CProtocol::RegistProtoName(PTCG2C_OPENSYSTEMNTF_TYPE, "PTCG2C_OPENSYSTEMNTF");
	CProtocol::RegistProtoName(PTCG2C_BATTLERESULTNTF_TYPE, "PTCG2C_BATTLERESULTNTF");
	CProtocol::RegistProtoName(PTCG2C_COMPLETEACHIVEMENT_TYPE, "PTCG2C_COMPLETEACHIVEMENT");
	CProtocol::RegistProtoName(PTCG2C_SKILLCHANGEDNTF_TYPE, "PTCG2C_SKILLCHANGEDNTF");
	CProtocol::RegistProtoName(PTCG2C_GSERRORNOTIFY_TYPE, "PTCG2C_GSERRORNOTIFY");
	CProtocol::RegistProtoName(PTCG2D_QUERYGLOBALCONFIG_TYPE, "PTCG2D_QUERYGLOBALCONFIG");
	CProtocol::RegistProtoName(PTCG2C_CHANGEFASHIONNOTIFY_TYPE, "PTCG2C_CHANGEFASHIONNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_CHECKININFONOTIFY_TYPE, "PTCG2C_CHECKININFONOTIFY");
	CProtocol::RegistProtoName(PTCG2C_ACTIVITYROLENOTIFY_TYPE, "PTCG2C_ACTIVITYROLENOTIFY");
	CProtocol::RegistProtoName(PTCG2T_OPENGATE_TYPE, "PTCG2T_OPENGATE");
	CProtocol::RegistProtoName(PTCG2C_UPDATEPAYINFO_TYPE, "PTCG2C_UPDATEPAYINFO");
	CProtocol::RegistProtoName(PTCG2C_REWARDCHANGEDNTF_TYPE, "PTCG2C_REWARDCHANGEDNTF");
	CProtocol::RegistProtoName(PTCG2C_LEAVETEAM_TYPE, "PTCG2C_LEAVETEAM");
	CProtocol::RegistProtoName(PTCG2C_REVIVENOTIFY_TYPE, "PTCG2C_REVIVENOTIFY");
	CProtocol::RegistProtoName(PTCG2C_UPDATEBUYGOLDANDFATIGUEINFO_TYPE, "PTCG2C_UPDATEBUYGOLDANDFATIGUEINFO");
	CProtocol::RegistProtoName(PTCG2C_REVIVECOUNTDOWN_TYPE, "PTCG2C_REVIVECOUNTDOWN");
	CProtocol::RegistProtoName(PTCG2C_FATIGUERECOVERTIMENOTIFY_TYPE, "PTCG2C_FATIGUERECOVERTIMENOTIFY");
	CProtocol::RegistProtoName(PTCG2C_ENTERSCENECOOLDOWNNOTIFY_TYPE, "PTCG2C_ENTERSCENECOOLDOWNNOTIFY");
	CProtocol::RegistProtoName(PTCG2D_QUERYREWARD_TYPE, "PTCG2D_QUERYREWARD");
	CProtocol::RegistProtoName(PTCG2D_REMOVEREWARD_TYPE, "PTCG2D_REMOVEREWARD");
	CProtocol::RegistProtoName(PTCG2D_ADDREWARD_TYPE, "PTCG2D_ADDREWARD");
	CProtocol::RegistProtoName(PTCG2L_GSLOGNOTIFY_TYPE, "PTCG2L_GSLOGNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_FASHOINCHANGEDNTF_TYPE, "PTCG2C_FASHOINCHANGEDNTF");
	CProtocol::RegistProtoName(PTCG2C_GUILDNOTIFYMEMBERCHANGED_TYPE, "PTCG2C_GUILDNOTIFYMEMBERCHANGED");
	CProtocol::RegistProtoName(PTCG2C_GUILDSETTINGCHANGEDNOTIFY_TYPE, "PTCG2C_GUILDSETTINGCHANGEDNOTIFY");
	CProtocol::RegistProtoName(PTCG2D_LOADGUILDDATAREQ_TYPE, "PTCG2D_LOADGUILDDATAREQ");
	CProtocol::RegistProtoName(PTCG2D_UPDATEGUILD_TYPE, "PTCG2D_UPDATEGUILD");
	CProtocol::RegistProtoName(PTCG2C_GUILDCHECKINBOXNTF_TYPE, "PTCG2C_GUILDCHECKINBOXNTF");
	CProtocol::RegistProtoName(PTCG2C_NEWGUILDBONUSNTF_TYPE, "PTCG2C_NEWGUILDBONUSNTF");
	CProtocol::RegistProtoName(PTCG2C_GUILDBESTCARDSNTF_TYPE, "PTCG2C_GUILDBESTCARDSNTF");
	CProtocol::RegistProtoName(PTCG2C_EXECUTELEVELSCRIPTNTF_TYPE, "PTCG2C_EXECUTELEVELSCRIPTNTF");
	CProtocol::RegistProtoName(PTCG2C_ONLINEREWARDNTF_TYPE, "PTCG2C_ONLINEREWARDNTF");
	CProtocol::RegistProtoName(PTCG2C_NEXTDAYREWARDNTF_TYPE, "PTCG2C_NEXTDAYREWARDNTF");
	CProtocol::RegistProtoName(PTCG2C_HINTNOTIFY_TYPE, "PTCG2C_HINTNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_POSITIONDEBUG_TYPE, "PTCG2C_POSITIONDEBUG");
	CProtocol::RegistProtoName(PTCG2C_ENEMYDORPDOODADNTF_TYPE, "PTCG2C_ENEMYDORPDOODADNTF");
	CProtocol::RegistProtoName(PTCG2C_TAKERANDOMTASK_TYPE, "PTCG2C_TAKERANDOMTASK");
	CProtocol::RegistProtoName(PTCG2C_BUFFNOTIFY_TYPE, "PTCG2C_BUFFNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_TEAMSELECTNOTIFY_TYPE, "PTCG2C_TEAMSELECTNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_TEAMINVITENOTIFY_TYPE, "PTCG2C_TEAMINVITENOTIFY");
	CProtocol::RegistProtoName(PTCG2C_FETCHENEMYDOODADNTF_TYPE, "PTCG2C_FETCHENEMYDOODADNTF");
	CProtocol::RegistProtoName(PTCG2C_PKPREPARENTF_TYPE, "PTCG2C_PKPREPARENTF");
	CProtocol::RegistProtoName(PTCG2C_PKTIMEOUTNTF_TYPE, "PTCG2C_PKTIMEOUTNTF");
	CProtocol::RegistProtoName(PTCG2C_GUILDGOBLINKILLNTF_TYPE, "PTCG2C_GUILDGOBLINKILLNTF");
	CProtocol::RegistProtoName(PTCG2C_SCENEFINISHSTATECHANGED_TYPE, "PTCG2C_SCENEFINISHSTATECHANGED");
	CProtocol::RegistProtoName(PTCG2C_PKROLEINFONTF_TYPE, "PTCG2C_PKROLEINFONTF");
	CProtocol::RegistProtoName(PTCG2C_ITEMCIRCLEDRAWRESULT_TYPE, "PTCG2C_ITEMCIRCLEDRAWRESULT");
	CProtocol::RegistProtoName(PTCG2C_SCENEDAMAGERANKNTF_TYPE, "PTCG2C_SCENEDAMAGERANKNTF");
	CProtocol::RegistProtoName(PTCG2C_SCENELEFTDOODAD_TYPE, "PTCG2C_SCENELEFTDOODAD");
	CProtocol::RegistProtoName(PTCG2C_CHANGESUPPLEMENTNTF_TYPE, "PTCG2C_CHANGESUPPLEMENTNTF");
	CProtocol::RegistProtoName(PTCG2W_REGISTGAMESERVERTOWEBSERVER_TYPE, "PTCG2W_REGISTGAMESERVERTOWEBSERVER");
	CProtocol::RegistProtoName(PTCG2C_CORRECTPOSITION_TYPE, "PTCG2C_CORRECTPOSITION");
	CProtocol::RegistProtoName(PTCG2C_TELEPORTNOTICE_TYPE, "PTCG2C_TELEPORTNOTICE");
	CProtocol::RegistProtoName(PTCG2C_ALLYMATCHROLEIDNOTIFY_TYPE, "PTCG2C_ALLYMATCHROLEIDNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_TEAMFULLDATANTF_TYPE, "PTCG2C_TEAMFULLDATANTF");
	CProtocol::RegistProtoName(PTCG2C_PETCHANGENOTFIY_TYPE, "PTCG2C_PETCHANGENOTFIY");
	CProtocol::RegistProtoName(PTCG2C_EXPFINDBACKNTF_TYPE, "PTCG2C_EXPFINDBACKNTF");
	CProtocol::RegistProtoName(PTCG2C_TOWERSCENEINFONTF_TYPE, "PTCG2C_TOWERSCENEINFONTF");
	CProtocol::RegistProtoName(PTCG2C_COVERDESIGNATIONNTF_TYPE, "PTCG2C_COVERDESIGNATIONNTF");
	CProtocol::RegistProtoName(PTCG2C_REACHACHIEVENTF_TYPE, "PTCG2C_REACHACHIEVENTF");
	CProtocol::RegistProtoName(PTCG2C_REACHDESIGNATIONNTF_TYPE, "PTCG2C_REACHDESIGNATIONNTF");
	CProtocol::RegistProtoName(PTCG2C_LEVELSEALNTF_TYPE, "PTCG2C_LEVELSEALNTF");
	CProtocol::RegistProtoName(PTCG2C_RECONNECTSYNCNOTIFY_TYPE, "PTCG2C_RECONNECTSYNCNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_OUTLOOKCHANGENTF_TYPE, "PTCG2C_OUTLOOKCHANGENTF");
	CProtocol::RegistProtoName(PTCG2C_FIVEDAYREWARDNTF_TYPE, "PTCG2C_FIVEDAYREWARDNTF");
	CProtocol::RegistProtoName(PTCG2C_SYNCSTEPNOTIFY_TYPE, "PTCG2C_SYNCSTEPNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_TSSSDKANTIDATANTF_TYPE, "PTCG2C_TSSSDKANTIDATANTF");
	CProtocol::RegistProtoName(PTCG2M_GMCLOSEQAROOM_TYPE, "PTCG2M_GMCLOSEQAROOM");
	CProtocol::RegistProtoName(PTCG2C_QANOTIFY_TYPE, "PTCG2C_QANOTIFY");
	CProtocol::RegistProtoName(PTCG2C_PVPBATTLEBEGINNTF_TYPE, "PTCG2C_PVPBATTLEBEGINNTF");
	CProtocol::RegistProtoName(PTCG2C_PVPBATTLEENDNTF_TYPE, "PTCG2C_PVPBATTLEENDNTF");
	CProtocol::RegistProtoName(PTCG2C_PVPBATTLEKILL_TYPE, "PTCG2C_PVPBATTLEKILL");
	CProtocol::RegistProtoName(PTCG2C_FLOWERRANKREWARDNTF_TYPE, "PTCG2C_FLOWERRANKREWARDNTF");
	CProtocol::RegistProtoName(PTCG2C_FASTMBCONFIRMNTF_TYPE, "PTCG2C_FASTMBCONFIRMNTF");
	CProtocol::RegistProtoName(PTCG2C_FASTMBDISMISSNTF_TYPE, "PTCG2C_FASTMBDISMISSNTF");
	CProtocol::RegistProtoName(PTCG2C_SELECTROLENTF_TYPE, "PTCG2C_SELECTROLENTF");
	CProtocol::RegistProtoName(PTCG2T_CHANGEGSLINE_TYPE, "PTCG2T_CHANGEGSLINE");
	CProtocol::RegistProtoName(PTCG2C_RECEIVEFLOWERNTF_TYPE, "PTCG2C_RECEIVEFLOWERNTF");
	CProtocol::RegistProtoName(PTCG2C_FLOWERRAINNTF_TYPE, "PTCG2C_FLOWERRAINNTF");
	CProtocol::RegistProtoName(PTCG2C_STARTBATTLEFAILEDNTF_TYPE, "PTCG2C_STARTBATTLEFAILEDNTF");
	CProtocol::RegistProtoName(PTCG2C_WATCHBATTLEINFONTF_TYPE, "PTCG2C_WATCHBATTLEINFONTF");
	CProtocol::RegistProtoName(PTCG2C_GMFJOINBATTLEREQ_TYPE, "PTCG2C_GMFJOINBATTLEREQ");
	CProtocol::RegistProtoName(PTCG2C_GMFONEBATTLEENDNTF_TYPE, "PTCG2C_GMFONEBATTLEENDNTF");
	CProtocol::RegistProtoName(PTCG2C_GMFBASEDATANTF_TYPE, "PTCG2C_GMFBASEDATANTF");
	CProtocol::RegistProtoName(PTCG2C_WORLDCHANNELLEFTTIMESNTF_TYPE, "PTCG2C_WORLDCHANNELLEFTTIMESNTF");
	CProtocol::RegistProtoName(PTCG2C_NOTICEGUILDBOSSEND_TYPE, "PTCG2C_NOTICEGUILDBOSSEND");
	CProtocol::RegistProtoName(PTCG2C_UPDATEGUILDARENASTATE_TYPE, "PTCG2C_UPDATEGUILDARENASTATE");
	CProtocol::RegistProtoName(PTCG2C_GUILDBOSSTIMEOUT_TYPE, "PTCG2C_GUILDBOSSTIMEOUT");
	CProtocol::RegistProtoName(PTCG2M_UPDATEROLESUMMARY_TYPE, "PTCG2M_UPDATEROLESUMMARY");
	CProtocol::RegistProtoName(PTCG2C_SYNGUILDARENABATTLEINFO_TYPE, "PTCG2C_SYNGUILDARENABATTLEINFO");
	CProtocol::RegistProtoName(PTCG2C_SYNGUILDARENAFIGHTUNIT_TYPE, "PTCG2C_SYNGUILDARENAFIGHTUNIT");
	CProtocol::RegistProtoName(PTCG2C_MULACTIVITYSTATECHANGE_TYPE, "PTCG2C_MULACTIVITYSTATECHANGE");
	CProtocol::RegistProtoName(PTCG2C_GMFALLFIGHTENDNTF_TYPE, "PTCG2C_GMFALLFIGHTENDNTF");
	CProtocol::RegistProtoName(PTCG2C_WORLDBOSSSTATENTF_TYPE, "PTCG2C_WORLDBOSSSTATENTF");
	CProtocol::RegistProtoName(PTCG2C_NOTIFYROLEEMPTY2WATCHER_TYPE, "PTCG2C_NOTIFYROLEEMPTY2WATCHER");
	CProtocol::RegistProtoName(PTCG2C_BATTLEWATCHERNTF_TYPE, "PTCG2C_BATTLEWATCHERNTF");
	CProtocol::RegistProtoName(PTCG2C_NOTICEGUILDARENABEGIN_TYPE, "PTCG2C_NOTICEGUILDARENABEGIN");
	CProtocol::RegistProtoName(PTCG2C_NOTIFYGUILDBOSSADDATTR_TYPE, "PTCG2C_NOTIFYGUILDBOSSADDATTR");
	CProtocol::RegistProtoName(PTCG2C_NOTIFYWATCHDATA_TYPE, "PTCG2C_NOTIFYWATCHDATA");
	CProtocol::RegistProtoName(PTCG2M_SCENEDESTROYED_TYPE, "PTCG2M_SCENEDESTROYED");
	CProtocol::RegistProtoName(PTCG2C_NOTIFYCLIENTENTERFIGHT_TYPE, "PTCG2C_NOTIFYCLIENTENTERFIGHT");
	CProtocol::RegistProtoName(PTCG2M_NOTICEFORWARDNTF_TYPE, "PTCG2M_NOTICEFORWARDNTF");
	CProtocol::RegistProtoName(PTCG2M_SYNCLIVEINFOTOMS_TYPE, "PTCG2M_SYNCLIVEINFOTOMS");
	CProtocol::RegistProtoName(PTCG2C_SYNGUILDARENAROLEONLINE_TYPE, "PTCG2C_SYNGUILDARENAROLEONLINE");
	CProtocol::RegistProtoName(PTCG2C_CLIENTONLYBUFFNOTIFY_TYPE, "PTCG2C_CLIENTONLYBUFFNOTIFY");
	CProtocol::RegistProtoName(PTCG2M_ROLESFINISHSCENETOMASTER_TYPE, "PTCG2M_ROLESFINISHSCENETOMASTER");
	CProtocol::RegistProtoName(PTCG2M_SCENEENDTOMASTER_TYPE, "PTCG2M_SCENEENDTOMASTER");
	CProtocol::RegistProtoName(PTCG2M_UPDATEGUILDCAMPRESULT_TYPE, "PTCG2M_UPDATEGUILDCAMPRESULT");
	CProtocol::RegistProtoName(PTCG2C_SYNGUILDARENADISPLACE_TYPE, "PTCG2C_SYNGUILDARENADISPLACE");
	CProtocol::RegistProtoName(PTCG2M_REPORTWATCHNTF_TYPE, "PTCG2M_REPORTWATCHNTF");
	CProtocol::RegistProtoName(PTCG2C_ONTRANSFERWALL_TYPE, "PTCG2C_ONTRANSFERWALL");
	CProtocol::RegistProtoName(PTCG2M_GMLEVELSEALINFO_TYPE, "PTCG2M_GMLEVELSEALINFO");
	CProtocol::RegistProtoName(PTCG2M_ADDMAILGS_TYPE, "PTCG2M_ADDMAILGS");
	CProtocol::RegistProtoName(PTCG2C_NOTIFYWATCHICONNUM2CLIENT_TYPE, "PTCG2C_NOTIFYWATCHICONNUM2CLIENT");
	CProtocol::RegistProtoName(PTCG2C_GMFWAITOTHERLOAD_TYPE, "PTCG2C_GMFWAITOTHERLOAD");
	CProtocol::RegistProtoName(PTCG2C_GMFWAITFIGHTBEGIN_TYPE, "PTCG2C_GMFWAITFIGHTBEGIN");
	CProtocol::RegistProtoName(PTCG2M_GMSENDCHAT_TYPE, "PTCG2M_GMSENDCHAT");
	CProtocol::RegistProtoName(PTCG2M_ADDGUILDBOSSATTR_TYPE, "PTCG2M_ADDGUILDBOSSATTR");
	CProtocol::RegistProtoName(PTCG2M_FIRSTENTERSCENENTF_TYPE, "PTCG2M_FIRSTENTERSCENENTF");
	CProtocol::RegistProtoName(PTCG2M_GMSETGUILDBOSSTIME_TYPE, "PTCG2M_GMSETGUILDBOSSTIME");
	CProtocol::RegistProtoName(PTCG2M_SYNSCENEREWARDG2M_TYPE, "PTCG2M_SYNSCENEREWARDG2M");
	CProtocol::RegistProtoName(PTCG2M_NOTICEGUILDBOSSDIE_TYPE, "PTCG2M_NOTICEGUILDBOSSDIE");
	CProtocol::RegistProtoName(PTCG2M_CREATEGUILDBOSSSINGLE_TYPE, "PTCG2M_CREATEGUILDBOSSSINGLE");
	CProtocol::RegistProtoName(PTCG2M_WORLDBOSSSYNCMS_TYPE, "PTCG2M_WORLDBOSSSYNCMS");
	CProtocol::RegistProtoName(PTCG2C_WORLDBOSSATTRNTF_TYPE, "PTCG2C_WORLDBOSSATTRNTF");
	CProtocol::RegistProtoName(PTCG2C_JOINROOMREPLY_TYPE, "PTCG2C_JOINROOMREPLY");
	CProtocol::RegistProtoName(PTCG2C_UPDATEVOIPROOMMEMBERNTF_TYPE, "PTCG2C_UPDATEVOIPROOMMEMBERNTF");
	CProtocol::RegistProtoName(PTCG2C_PAYALLINFONTF_TYPE, "PTCG2C_PAYALLINFONTF");
	CProtocol::RegistProtoName(PTCG2C_LOGINREWARD2CNTF_TYPE, "PTCG2C_LOGINREWARD2CNTF");
	CProtocol::RegistProtoName(PTCG2M_RELOADVERSIONNTF_TYPE, "PTCG2M_RELOADVERSIONNTF");
	CProtocol::RegistProtoName(PTCG2C_AIDEBUGINFO_TYPE, "PTCG2C_AIDEBUGINFO");
	CProtocol::RegistProtoName(PTCG2C_STARTROLLNTF_TYPE, "PTCG2C_STARTROLLNTF");
	CProtocol::RegistProtoName(PTCG2C_NOTICEHUNGRYDOWN_TYPE, "PTCG2C_NOTICEHUNGRYDOWN");
	CProtocol::RegistProtoName(PTCG2C_FIRSTPASSREWARDNTF_TYPE, "PTCG2C_FIRSTPASSREWARDNTF");
	CProtocol::RegistProtoName(PTCG2M_TEAMEVENTNOTIFY_TYPE, "PTCG2M_TEAMEVENTNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_TITLECHANGENOTIFY_TYPE, "PTCG2C_TITLECHANGENOTIFY");
	CProtocol::RegistProtoName(PTCG2C_ITEMFINDBACKNTF_TYPE, "PTCG2C_ITEMFINDBACKNTF");
	CProtocol::RegistProtoName(PTCG2C_SPRITECHANGEDNTF_TYPE, "PTCG2C_SPRITECHANGEDNTF");
	CProtocol::RegistProtoName(PTCG2C_FIGHTGROUPCHANGENTF_TYPE, "PTCG2C_FIGHTGROUPCHANGENTF");
	CProtocol::RegistProtoName(PTCG2C_BOSSRUSHONEFINISHNTF_TYPE, "PTCG2C_BOSSRUSHONEFINISHNTF");
	CProtocol::RegistProtoName(PTCG2C_SYNATLASATTR_TYPE, "PTCG2C_SYNATLASATTR");
	CProtocol::RegistProtoName(PTCG2C_PLAYDICENTF_TYPE, "PTCG2C_PLAYDICENTF");
	CProtocol::RegistProtoName(PTCG2M_DEPROGRESSSYN_TYPE, "PTCG2M_DEPROGRESSSYN");
	CProtocol::RegistProtoName(PTCG2C_IBSHOPICON_TYPE, "PTCG2C_IBSHOPICON");
	CProtocol::RegistProtoName(PTCG2C_NOTIFYENHANCESUIT_TYPE, "PTCG2C_NOTIFYENHANCESUIT");
	CProtocol::RegistProtoName(PTCG2C_RISKBUYNTF_TYPE, "PTCG2C_RISKBUYNTF");
	CProtocol::RegistProtoName(PTCG2C_RISKRECOVERDICENTF_TYPE, "PTCG2C_RISKRECOVERDICENTF");
	CProtocol::RegistProtoName(PTCG2M_GETMAXTITLENTF_TYPE, "PTCG2M_GETMAXTITLENTF");
	CProtocol::RegistProtoName(PTCG2M_REQGUILDSIMPLEDATA_TYPE, "PTCG2M_REQGUILDSIMPLEDATA");
	CProtocol::RegistProtoName(PTCG2M_SYNGUILDBOSSHURT_TYPE, "PTCG2M_SYNGUILDBOSSHURT");
	CProtocol::RegistProtoName(PTCG2M_SYNADDGUILDACTIVITY_TYPE, "PTCG2M_SYNADDGUILDACTIVITY");
	CProtocol::RegistProtoName(PTCG2C_TOWERFIRSTPASSREWARDNTF_TYPE, "PTCG2C_TOWERFIRSTPASSREWARDNTF");
	CProtocol::RegistProtoName(PTCG2M_GMFUPDATEDATAG2M_TYPE, "PTCG2M_GMFUPDATEDATAG2M");
	CProtocol::RegistProtoName(PTCG2M_SYNGUILDCONTRIBUTE_TYPE, "PTCG2M_SYNGUILDCONTRIBUTE");
	CProtocol::RegistProtoName(PTCG2C_HALLICONSNTF_TYPE, "PTCG2C_HALLICONSNTF");
	CProtocol::RegistProtoName(PTCG2C_UPDATETASKSTATUS_TYPE, "PTCG2C_UPDATETASKSTATUS");
	CProtocol::RegistProtoName(PTCG2C_SKYCITYESTIMATERES_TYPE, "PTCG2C_SKYCITYESTIMATERES");
	CProtocol::RegistProtoName(PTCG2C_SKYCITYTIMERES_TYPE, "PTCG2C_SKYCITYTIMERES");
	CProtocol::RegistProtoName(PTCG2C_NOTIFYIDIPMESSAGEGS_TYPE, "PTCG2C_NOTIFYIDIPMESSAGEGS");
	CProtocol::RegistProtoName(PTCG2C_IDIPPUNISHINFONTF_TYPE, "PTCG2C_IDIPPUNISHINFONTF");
	CProtocol::RegistProtoName(PTCG2C_SKYCITYTEAMRES_TYPE, "PTCG2C_SKYCITYTEAMRES");
	CProtocol::RegistProtoName(PTCG2M_NOTIFYSCENEPKWIN_TYPE, "PTCG2M_NOTIFYSCENEPKWIN");
	CProtocol::RegistProtoName(PTCG2C_GMFKICKNTY_TYPE, "PTCG2C_GMFKICKNTY");
	CProtocol::RegistProtoName(PTCG2M_SKYCITYTEAMRESULTNTF_TYPE, "PTCG2M_SKYCITYTEAMRESULTNTF");
	CProtocol::RegistProtoName(PTCG2C_SKYCITYBATTLEDATANTF_TYPE, "PTCG2C_SKYCITYBATTLEDATANTF");
	CProtocol::RegistProtoName(PTCG2C_GMFBATTLESTATENTF_TYPE, "PTCG2C_GMFBATTLESTATENTF");
	CProtocol::RegistProtoName(PTCG2M_BROADCAST_TYPE, "PTCG2M_BROADCAST");
	CProtocol::RegistProtoName(PTCG2C_SPACTIVITYCHANGENTF_TYPE, "PTCG2C_SPACTIVITYCHANGENTF");
	CProtocol::RegistProtoName(PTCG2C_SPACTIVITYOFFSETDAYNTF_TYPE, "PTCG2C_SPACTIVITYOFFSETDAYNTF");
	CProtocol::RegistProtoName(PTCG2C_DPSNOTIFY_TYPE, "PTCG2C_DPSNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_SKILLCOOLNTF_TYPE, "PTCG2C_SKILLCOOLNTF");
	CProtocol::RegistProtoName(PTCG2C_GARDENPLANTEVENTNOTICE_TYPE, "PTCG2C_GARDENPLANTEVENTNOTICE");
	CProtocol::RegistProtoName(PTCG2C_GARDENBANQUETNOTICE_TYPE, "PTCG2C_GARDENBANQUETNOTICE");
	CProtocol::RegistProtoName(PTCG2C_GMFGUILDCOMBATNTF_TYPE, "PTCG2C_GMFGUILDCOMBATNTF");
	CProtocol::RegistProtoName(PTCG2C_GPRONEBATTLEENDNTF_TYPE, "PTCG2C_GPRONEBATTLEENDNTF");
	CProtocol::RegistProtoName(PTCG2C_GPRALLFIGHTENDNTF_TYPE, "PTCG2C_GPRALLFIGHTENDNTF");
	CProtocol::RegistProtoName(PTCG2M_SCENECREATED_TYPE, "PTCG2M_SCENECREATED");
	CProtocol::RegistProtoName(PTCG2M_ROLELOGIN_TYPE, "PTCG2M_ROLELOGIN");
	CProtocol::RegistProtoName(PTCG2M_ROLELOGOUT_TYPE, "PTCG2M_ROLELOGOUT");
	CProtocol::RegistProtoName(PTCG2M_ROLECHANGESCENENTF_TYPE, "PTCG2M_ROLECHANGESCENENTF");
	CProtocol::RegistProtoName(PTCG2M_ENTERSCENEREQTOMS_TYPE, "PTCG2M_ENTERSCENEREQTOMS");
	CProtocol::RegistProtoName(PTCG2M_RESWARTEAMRESULTNTF_TYPE, "PTCG2M_RESWARTEAMRESULTNTF");
	CProtocol::RegistProtoName(PTCG2C_RESWARBATTLEDATANTF_TYPE, "PTCG2C_RESWARBATTLEDATANTF");
	CProtocol::RegistProtoName(PTCG2K_ROLEENTERCROSSSCENENTF_TYPE, "PTCG2K_ROLEENTERCROSSSCENENTF");
	CProtocol::RegistProtoName(PTCG2K_CROSSSCENEDESTORYED_TYPE, "PTCG2K_CROSSSCENEDESTORYED");
	CProtocol::RegistProtoName(PTCG2C_RESWARTEAMRESONE_TYPE, "PTCG2C_RESWARTEAMRESONE");
	CProtocol::RegistProtoName(PTCG2C_PAYMEMBERPRIVILEGENTF_TYPE, "PTCG2C_PAYMEMBERPRIVILEGENTF");
	CProtocol::RegistProtoName(PTCG2C_KILLENEMYSCORENTF_TYPE, "PTCG2C_KILLENEMYSCORENTF");
	CProtocol::RegistProtoName(PTCG2C_SERVEROPENDAYNTF_TYPE, "PTCG2C_SERVEROPENDAYNTF");
	CProtocol::RegistProtoName(PTCG2N_SCENEDESTROYEDTONS_TYPE, "PTCG2N_SCENEDESTROYEDTONS");
	CProtocol::RegistProtoName(PTCG2C_SYNGUILDINHERITEXP_TYPE, "PTCG2C_SYNGUILDINHERITEXP");
	CProtocol::RegistProtoName(PTCG2C_NOTICEDOINGGUILDINHERIT_TYPE, "PTCG2C_NOTICEDOINGGUILDINHERIT");
	CProtocol::RegistProtoName(PTCG2C_SYNGUILDINHERITNUMINFO_TYPE, "PTCG2C_SYNGUILDINHERITNUMINFO");
	CProtocol::RegistProtoName(PTCG2C_CDCALL_TYPE, "PTCG2C_CDCALL");
	CProtocol::RegistProtoName(PTCG2M_NOTICEGUILDINHERITEND_TYPE, "PTCG2M_NOTICEGUILDINHERITEND");
	CProtocol::RegistProtoName(PTCG2M_MSCRASHRECOVERG2M_TYPE, "PTCG2M_MSCRASHRECOVERG2M");
	CProtocol::RegistProtoName(PTCG2M_SYNCALLLIVEINFOTOMS_TYPE, "PTCG2M_SYNCALLLIVEINFOTOMS");
	CProtocol::RegistProtoName(PTCG2M_ENTERHALLNOTICE_TYPE, "PTCG2M_ENTERHALLNOTICE");
	CProtocol::RegistProtoName(PTCG2C_SYNCMOVENOTIFY_TYPE, "PTCG2C_SYNCMOVENOTIFY");
	CProtocol::RegistProtoName(PTCG2C_CHANGENAMECOUNTNTF_TYPE, "PTCG2C_CHANGENAMECOUNTNTF");
	CProtocol::RegistProtoName(PTCG2M_SCENEEMPTYNOTIFY_TYPE, "PTCG2M_SCENEEMPTYNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_ANTIADDICTIONREMINDNTF_TYPE, "PTCG2C_ANTIADDICTIONREMINDNTF");
	CProtocol::RegistProtoName(PTCG2C_HORSEAWARDALLNTF_TYPE, "PTCG2C_HORSEAWARDALLNTF");
	CProtocol::RegistProtoName(PTCG2C_HORSERANKNTF_TYPE, "PTCG2C_HORSERANKNTF");
	CProtocol::RegistProtoName(PTCG2C_HORSEWAITTIMENTF_TYPE, "PTCG2C_HORSEWAITTIMENTF");
	CProtocol::RegistProtoName(PTCG2C_HORSEANIMATIONNTF_TYPE, "PTCG2C_HORSEANIMATIONNTF");
	CProtocol::RegistProtoName(PTCG2M_REMOVEASKITEMNTF_TYPE, "PTCG2M_REMOVEASKITEMNTF");
	CProtocol::RegistProtoName(PTCG2C_HORSEFINALNTF_TYPE, "PTCG2C_HORSEFINALNTF");
	CProtocol::RegistProtoName(PTCG2M_ADDPARTNERLIVENESSG2M_TYPE, "PTCG2M_ADDPARTNERLIVENESSG2M");
	CProtocol::RegistProtoName(PTCG2C_HORSECOUNTDOWNTIMENTF_TYPE, "PTCG2C_HORSECOUNTDOWNTIMENTF");
	CProtocol::RegistProtoName(PTCG2C_DOODADITEMADDNTF_TYPE, "PTCG2C_DOODADITEMADDNTF");
	CProtocol::RegistProtoName(PTCG2C_GCFZHANLINGNOTIFY_TYPE, "PTCG2C_GCFZHANLINGNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_INVFIGHTBEFENTERSCENENTF_TYPE, "PTCG2C_INVFIGHTBEFENTERSCENENTF");
	CProtocol::RegistProtoName(PTCG2C_HEROBATTLEDATANTF_TYPE, "PTCG2C_HEROBATTLEDATANTF");
	CProtocol::RegistProtoName(PTCG2C_SCENEPREPAREINFONTF_TYPE, "PTCG2C_SCENEPREPAREINFONTF");
	CProtocol::RegistProtoName(PTCG2C_HEROBATTLETEAMMSGNTF_TYPE, "PTCG2C_HEROBATTLETEAMMSGNTF");
	CProtocol::RegistProtoName(PTCG2C_HEROBATTLESYNCNTF_TYPE, "PTCG2C_HEROBATTLESYNCNTF");
	CProtocol::RegistProtoName(PTCG2C_HEROBATTLEINCIRCLENTF_TYPE, "PTCG2C_HEROBATTLEINCIRCLENTF");
	CProtocol::RegistProtoName(PTCG2M_GCFSYNG2M_TYPE, "PTCG2M_GCFSYNG2M");
	CProtocol::RegistProtoName(PTCG2C_HEROBATTLEOVERTIME_TYPE, "PTCG2C_HEROBATTLEOVERTIME");
	CProtocol::RegistProtoName(PTCG2C_HEROBATTLETEAMROLENTF_TYPE, "PTCG2C_HEROBATTLETEAMROLENTF");
	CProtocol::RegistProtoName(PTCG2C_GCFSYNG2CNTF_TYPE, "PTCG2C_GCFSYNG2CNTF");
	CProtocol::RegistProtoName(PTCG2M_PAYADDDIAMONDNTF_TYPE, "PTCG2M_PAYADDDIAMONDNTF");
	CProtocol::RegistProtoName(PTCG2M_PAYCONSUMENTF_TYPE, "PTCG2M_PAYCONSUMENTF");
	CProtocol::RegistProtoName(PTCG2C_DOODADITEMUSENTF_TYPE, "PTCG2C_DOODADITEMUSENTF");
	CProtocol::RegistProtoName(PTCG2C_SPECIALSTATENTF_TYPE, "PTCG2C_SPECIALSTATENTF");
	CProtocol::RegistProtoName(PTCG2C_CLOSECHOOSEHERONTF_TYPE, "PTCG2C_CLOSECHOOSEHERONTF");
	CProtocol::RegistProtoName(PTCG2R_BROADCASTG2R_TYPE, "PTCG2R_BROADCASTG2R");
	CProtocol::RegistProtoName(PTCG2C_LEVELSCRIPTSTATENTF_TYPE, "PTCG2C_LEVELSCRIPTSTATENTF");
	CProtocol::RegistProtoName(PTCG2C_SCENESTATENTF_TYPE, "PTCG2C_SCENESTATENTF");
	CProtocol::RegistProtoName(PTCG2C_QUERYROLESTATEACK_TYPE, "PTCG2C_QUERYROLESTATEACK");
	CProtocol::RegistProtoName(PTCG2C_IBSHOPHASBUYNTF_TYPE, "PTCG2C_IBSHOPHASBUYNTF");
	CProtocol::RegistProtoName(PTCG2C_SKILLINITCOOLNTF_TYPE, "PTCG2C_SKILLINITCOOLNTF");
	CProtocol::RegistProtoName(PTCG2C_HEROBATTLECANUSEHERO_TYPE, "PTCG2C_HEROBATTLECANUSEHERO");
	CProtocol::RegistProtoName(PTCG2C_LEAGUEBATTLELOADINFONTF_TYPE, "PTCG2C_LEAGUEBATTLELOADINFONTF");
	CProtocol::RegistProtoName(PTCG2C_LEAGUEBATTLEBASEDATANTF_TYPE, "PTCG2C_LEAGUEBATTLEBASEDATANTF");
	CProtocol::RegistProtoName(PTCG2C_LEAGUEBATTLEONERESULTNFT_TYPE, "PTCG2C_LEAGUEBATTLEONERESULTNFT");
	CProtocol::RegistProtoName(PTCG2C_LEAGUEBATTLERESULTNTF_TYPE, "PTCG2C_LEAGUEBATTLERESULTNTF");
	CProtocol::RegistProtoName(PTCG2C_PUSHPRAISENTF_TYPE, "PTCG2C_PUSHPRAISENTF");
	CProtocol::RegistProtoName(PTCG2C_HEROBATTLETIPSNTF_TYPE, "PTCG2C_HEROBATTLETIPSNTF");
	CProtocol::RegistProtoName(PTCG2C_LEAGUEBATTLESTATENTF_TYPE, "PTCG2C_LEAGUEBATTLESTATENTF");
	CProtocol::RegistProtoName(PTCG2M_LEAGUEBATTLEUPDATEDATAG2M_TYPE, "PTCG2M_LEAGUEBATTLEUPDATEDATAG2M");
	CProtocol::RegistProtoName(PTCG2M_LEAGUEELIMINATIONUPDATEG2M_TYPE, "PTCG2M_LEAGUEELIMINATIONUPDATEG2M");
	CProtocol::RegistProtoName(PTCG2M_SYNGUILDBOSSATTR_TYPE, "PTCG2M_SYNGUILDBOSSATTR");
	CProtocol::RegistProtoName(PTCG2M_LEVELCHANGENTFMS_TYPE, "PTCG2M_LEVELCHANGENTFMS");
	CProtocol::RegistProtoName(PTCG2K_LEAGUEELIMINATIONUPDATEG2K_TYPE, "PTCG2K_LEAGUEELIMINATIONUPDATEG2K");
	CProtocol::RegistProtoName(PTCG2M_OPENSYSTEMNTFMS_TYPE, "PTCG2M_OPENSYSTEMNTFMS");
	CProtocol::RegistProtoName(PTCG2T_BROADCASTG2T_TYPE, "PTCG2T_BROADCASTG2T");
	CProtocol::RegistProtoName(PTCG2C_COUNTDOWNNTF_TYPE, "PTCG2C_COUNTDOWNNTF");
	CProtocol::RegistProtoName(PTCG2C_ALLGIFTIBITEMNTF_TYPE, "PTCG2C_ALLGIFTIBITEMNTF");
	CProtocol::RegistProtoName(PTCG2C_GIFTIBBACKINFONTF_TYPE, "PTCG2C_GIFTIBBACKINFONTF");
	CProtocol::RegistProtoName(PTCG2C_SHARERANDOMGIFTNTF_TYPE, "PTCG2C_SHARERANDOMGIFTNTF");
	CProtocol::RegistProtoName(PTCG2C_WORLDBOSSGUILDADDATTRSYNCCLIENTNTF_TYPE, "PTCG2C_WORLDBOSSGUILDADDATTRSYNCCLIENTNTF");
	CProtocol::RegistProtoName(PTCG2M_UPDATEGLOBALVALUE_TYPE, "PTCG2M_UPDATEGLOBALVALUE");
	CProtocol::RegistProtoName(PTCG2C_MILITARYRANKNTF_TYPE, "PTCG2C_MILITARYRANKNTF");
	CProtocol::RegistProtoName(PTCG2C_LOGINACTIVITYSTATUSNTF_TYPE, "PTCG2C_LOGINACTIVITYSTATUSNTF");
	CProtocol::RegistProtoName(PTCG2C_DISPLAYADDITEM_TYPE, "PTCG2C_DISPLAYADDITEM");
	CProtocol::RegistProtoName(PTCG2C_UPDATEDISPLAYITEMS_TYPE, "PTCG2C_UPDATEDISPLAYITEMS");
	CProtocol::RegistProtoName(PTCG2C_GUILDCAMPPARTYNOTIFY_TYPE, "PTCG2C_GUILDCAMPPARTYNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_ABSPARTYNTF_TYPE, "PTCG2C_ABSPARTYNTF");
	CProtocol::RegistProtoName(PTCG2C_GUILDCAMPPARTYTRADENOTIFY_TYPE, "PTCG2C_GUILDCAMPPARTYTRADENOTIFY");
	CProtocol::RegistProtoName(PTCG2M_REPORTCUSTOMBATTLERESULT_TYPE, "PTCG2M_REPORTCUSTOMBATTLERESULT");
	CProtocol::RegistProtoName(PTCG2M_REPORTBADPLAYER2MS_TYPE, "PTCG2M_REPORTBADPLAYER2MS");
	CProtocol::RegistProtoName(PTCG2C_CUSTOMBATTLELOADINGNTF_TYPE, "PTCG2C_CUSTOMBATTLELOADINGNTF");
	CProtocol::RegistProtoName(PTCG2M_TARJAINFONTF_TYPE, "PTCG2M_TARJAINFONTF");
	CProtocol::RegistProtoName(PTCG2M_SCENETEAMRESULTNTF_TYPE, "PTCG2M_SCENETEAMRESULTNTF");
	CProtocol::RegistProtoName(PTCG2M_TEAMCOSTLEADERRESETNTF_TYPE, "PTCG2M_TEAMCOSTLEADERRESETNTF");
	CProtocol::RegistProtoName(PTCG2C_PLATFORMSHAREAWARDNTF_TYPE, "PTCG2C_PLATFORMSHAREAWARDNTF");
	CProtocol::RegistProtoName(PTCG2C_WEEKEND4V4ROLEDATANTF_TYPE, "PTCG2C_WEEKEND4V4ROLEDATANTF");
	CProtocol::RegistProtoName(PTCG2C_HEROKILLNOTIFY_TYPE, "PTCG2C_HEROKILLNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_MOBABATTLETEAMROLENTF_TYPE, "PTCG2C_MOBABATTLETEAMROLENTF");
	CProtocol::RegistProtoName(PTCG2C_MOBAROLECHANGENTF_TYPE, "PTCG2C_MOBAROLECHANGENTF");
	CProtocol::RegistProtoName(PTCG2C_MOBABATTLETEAMMSGNTF_TYPE, "PTCG2C_MOBABATTLETEAMMSGNTF");
	CProtocol::RegistProtoName(PTCG2C_TRANSSKILLNOTFIY_TYPE, "PTCG2C_TRANSSKILLNOTFIY");
	CProtocol::RegistProtoName(PTCG2C_ENTITYTARGETCHANGENTF_TYPE, "PTCG2C_ENTITYTARGETCHANGENTF");
	CProtocol::RegistProtoName(PTCG2C_MOBASIGNALBROADCAST_TYPE, "PTCG2C_MOBASIGNALBROADCAST");
	CProtocol::RegistProtoName(PTCG2C_PETINVITENTF_TYPE, "PTCG2C_PETINVITENTF");
	CProtocol::RegistProtoName(PTCG2M_ADDMOBABATTLERECORD_TYPE, "PTCG2M_ADDMOBABATTLERECORD");
	CProtocol::RegistProtoName(PTCG2C_CHATNOTIFYG2C_TYPE, "PTCG2C_CHATNOTIFYG2C");
	CProtocol::RegistProtoName(PTCG2C_MOBAMINIMAPNTF_TYPE, "PTCG2C_MOBAMINIMAPNTF");
	CProtocol::RegistProtoName(PTCG2M_CLIENTSESSIONCHANGEG2M_TYPE, "PTCG2M_CLIENTSESSIONCHANGEG2M");
	CProtocol::RegistProtoName(PTCG2M_ROLELOGINRECONNECTNTF_TYPE, "PTCG2M_ROLELOGINRECONNECTNTF");
	CProtocol::RegistProtoName(PTCG2C_MOBAADDEXPNTF_TYPE, "PTCG2C_MOBAADDEXPNTF");
	CProtocol::RegistProtoName(PTCG2C_BMREADYTIMENTF_TYPE, "PTCG2C_BMREADYTIMENTF");
	CProtocol::RegistProtoName(PTCG2K_BMSCENEFIGHTINFONTF_TYPE, "PTCG2K_BMSCENEFIGHTINFONTF");
	CProtocol::RegistProtoName(PTCG2C_BMFIGHTTIMENTF_TYPE, "PTCG2C_BMFIGHTTIMENTF");
	CProtocol::RegistProtoName(PTCG2C_BMROLESCENESYNCNTF_TYPE, "PTCG2C_BMROLESCENESYNCNTF");
	CProtocol::RegistProtoName(PTCG2C_BIGMELEERELIVENTF_TYPE, "PTCG2C_BIGMELEERELIVENTF");
	CProtocol::RegistProtoName(PTCG2K_BMLEAVEFIGHTSCENENTF_TYPE, "PTCG2K_BMLEAVEFIGHTSCENENTF");
	CProtocol::RegistProtoName(PTCG2C_MOBAHINTNTF_TYPE, "PTCG2C_MOBAHINTNTF");
	CProtocol::RegistProtoName(PTCG2M_TEAMTRANGS2MS_TYPE, "PTCG2M_TEAMTRANGS2MS");
	CProtocol::RegistProtoName(PTCG2C_TAJIEHELPNOTIFY_TYPE, "PTCG2C_TAJIEHELPNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_NOTIFYSTARTUPTYPETOCLIENT_TYPE, "PTCG2C_NOTIFYSTARTUPTYPETOCLIENT");
	CProtocol::RegistProtoName(PTCG2C_HEROBATTLEANCIENTPOWER_TYPE, "PTCG2C_HEROBATTLEANCIENTPOWER");
	CProtocol::RegistProtoName(PTCG2C_BATTLESTATISTICSNTF_TYPE, "PTCG2C_BATTLESTATISTICSNTF");
	CProtocol::RegistProtoName(PTCG2C_WEDDINGLOADINFONTF_TYPE, "PTCG2C_WEDDINGLOADINFONTF");
	CProtocol::RegistProtoName(PTCG2C_WEDDINGSTATENTF_TYPE, "PTCG2C_WEDDINGSTATENTF");
	CProtocol::RegistProtoName(PTCG2C_WEDDINGEVENTNTF_TYPE, "PTCG2C_WEDDINGEVENTNTF");
	CProtocol::RegistProtoName(PTCG2C_HORSEFAILTIPSNTF_TYPE, "PTCG2C_HORSEFAILTIPSNTF");
	CProtocol::RegistProtoName(PTCG2C_WARNINGRANDOMNTF_TYPE, "PTCG2C_WARNINGRANDOMNTF");
	CProtocol::RegistProtoName(PTCG2M_UPDATEWEDDINGSTATEG2M_TYPE, "PTCG2M_UPDATEWEDDINGSTATEG2M");
	CProtocol::RegistProtoName(PTCG2C_THEMEACTIVITYCHANGENTF_TYPE, "PTCG2C_THEMEACTIVITYCHANGENTF");
	CProtocol::RegistProtoName(PTCG2K_ROLELEAVECROSSSCENENTF_TYPE, "PTCG2K_ROLELEAVECROSSSCENENTF");
	CProtocol::RegistProtoName(PTCG2C_WEDDINGCARNTF_TYPE, "PTCG2C_WEDDINGCARNTF");
	CProtocol::RegistProtoName(PTCG2C_BACKFLOWOPENNTF_TYPE, "PTCG2C_BACKFLOWOPENNTF");
	CProtocol::RegistProtoName(PTCG2M_PRIVATECHAT_TYPE, "PTCG2M_PRIVATECHAT");
	CProtocol::RegistProtoName(PTCG2C_TRANSNOTIFY_TYPE, "PTCG2C_TRANSNOTIFY");
	CProtocol::RegistProtoName(PTCG2M_INVFIGHTAGAINREQG2M_TYPE, "PTCG2M_INVFIGHTAGAINREQG2M");
	CProtocol::RegistProtoName(PTCG2K_INVFIGHTAGAINREQG2W_TYPE, "PTCG2K_INVFIGHTAGAINREQG2W");
	CProtocol::RegistProtoName(PTCG2M_REQDRAGONGUILDSIMPLEDATA_TYPE, "PTCG2M_REQDRAGONGUILDSIMPLEDATA");
	CProtocol::RegistProtoName(PTCG2M_COSTDRAGONGUILDRECEIVECOUNT_TYPE, "PTCG2M_COSTDRAGONGUILDRECEIVECOUNT");
	CProtocol::RegistProtoName(PTCG2M_CHECKDRAGONGUILDTASK_TYPE, "PTCG2M_CHECKDRAGONGUILDTASK");
	CProtocol::RegistProtoName(PTCG2C_BATTLEFIELDREADYINFONTF_TYPE, "PTCG2C_BATTLEFIELDREADYINFONTF");
	CProtocol::RegistProtoName(PTCG2C_BATTLEFIELDRELIVENTF_TYPE, "PTCG2C_BATTLEFIELDRELIVENTF");
	CProtocol::RegistProtoName(PTCG2C_BATTLEFIELDROLEAGAINSTNTF_TYPE, "PTCG2C_BATTLEFIELDROLEAGAINSTNTF");
	CProtocol::RegistProtoName(PTCG2M_GOALAWARDSSERVEREVENT_TYPE, "PTCG2M_GOALAWARDSSERVEREVENT");
	CProtocol::RegistProtoName(PTCG2C_BFFIGHTTIMENTF_TYPE, "PTCG2C_BFFIGHTTIMENTF");
	CProtocol::RegistProtoName(PTCG2C_PAYSCORENTF_TYPE, "PTCG2C_PAYSCORENTF");
	CProtocol::RegistProtoName(PTCG2M_GOALAWARDSSERVERTEAM_TYPE, "PTCG2M_GOALAWARDSSERVERTEAM");
	CProtocol::RegistProtoName(PTCG2C_NPCFLNTF_TYPE, "PTCG2C_NPCFLNTF");
	CProtocol::RegistProtoName(PTCG2C_DOODADITEMSKILLSNTF_TYPE, "PTCG2C_DOODADITEMSKILLSNTF");
	CProtocol::RegistProtoName(PTCG2C_WORDNOTIFY_TYPE, "PTCG2C_WORDNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_BIGMELEEPOINTNTF_TYPE, "PTCG2C_BIGMELEEPOINTNTF");
	CProtocol::RegistProtoName(PTCG2C_BIGMELEEPOINTOUTLOOKNTF_TYPE, "PTCG2C_BIGMELEEPOINTOUTLOOKNTF");
	CProtocol::RegistProtoName(PTCG2C_KICKACCOUNTJKYDNTF_TYPE, "PTCG2C_KICKACCOUNTJKYDNTF");
	CProtocol::RegistProtoName(PTCG2M_KICKACCOUNTJKYDG2M_TYPE, "PTCG2M_KICKACCOUNTJKYDG2M");
	CProtocol::RegistProtoName(RPCC2G_SYNCTIME_TYPE, "RPCC2G_SYNCTIME");
	CProtocol::RegistProtoName(RPCC2G_SKILLLEVELUP_TYPE, "RPCC2G_SKILLLEVELUP");
	CProtocol::RegistProtoName(RPCC2G_FETCHACHIVEMENTREWARD_TYPE, "RPCC2G_FETCHACHIVEMENTREWARD");
	CProtocol::RegistProtoName(RPCC2G_SELECTCHESTREWARD_TYPE, "RPCC2G_SELECTCHESTREWARD");
	CProtocol::RegistProtoName(RPCC2G_RESETSKILL_TYPE, "RPCC2G_RESETSKILL");
	CProtocol::RegistProtoName(RPCC2G_CHOOSEPROFESSION_TYPE, "RPCC2G_CHOOSEPROFESSION");
	CProtocol::RegistProtoName(RPCC2G_BINDSKILL_TYPE, "RPCC2G_BINDSKILL");
	CProtocol::RegistProtoName(RPCC2G_TASKOPERATE_TYPE, "RPCC2G_TASKOPERATE");
	CProtocol::RegistProtoName(RPCC2G_QUERYLOTTERYCD_TYPE, "RPCC2G_QUERYLOTTERYCD");
	CProtocol::RegistProtoName(RPCC2G_DRAWLOTTERY_TYPE, "RPCC2G_DRAWLOTTERY");
	CProtocol::RegistProtoName(RPCC2G_ENHANCEITEM_TYPE, "RPCC2G_ENHANCEITEM");
	CProtocol::RegistProtoName(RPCC2G_QUERYSCENEDAYCOUNT_TYPE, "RPCC2G_QUERYSCENEDAYCOUNT");
	CProtocol::RegistProtoName(RPCC2G_GMCOMMAND_TYPE, "RPCC2G_GMCOMMAND");
	CProtocol::RegistProtoName(RPCC2G_DECOMPOSEEQUIPMENT_TYPE, "RPCC2G_DECOMPOSEEQUIPMENT");
	CProtocol::RegistProtoName(RPCC2G_EMBLEMLEVELUP_TYPE, "RPCC2G_EMBLEMLEVELUP");
	CProtocol::RegistProtoName(RPCC2G_USEITEM_TYPE, "RPCC2G_USEITEM");
	CProtocol::RegistProtoName(RPCC2G_JADEOPERATION_TYPE, "RPCC2G_JADEOPERATION");
	CProtocol::RegistProtoName(RPCC2G_JADECOMPOSE_TYPE, "RPCC2G_JADECOMPOSE");
	CProtocol::RegistProtoName(RPCC2G_CHECKIN_TYPE, "RPCC2G_CHECKIN");
	CProtocol::RegistProtoName(RPCC2G_GETACTIVITYCHEST_TYPE, "RPCC2G_GETACTIVITYCHEST");
	CProtocol::RegistProtoName(RPCC2G_FASHIONCOMPOSE_TYPE, "RPCC2G_FASHIONCOMPOSE");
	CProtocol::RegistProtoName(RPCC2G_BUYJADESLOT_TYPE, "RPCC2G_BUYJADESLOT");
	CProtocol::RegistProtoName(RPCC2G_GETSYSTEMREWARD_TYPE, "RPCC2G_GETSYSTEMREWARD");
	CProtocol::RegistProtoName(RPCC2G_SWEEP_TYPE, "RPCC2G_SWEEP");
	CProtocol::RegistProtoName(RPCC2G_REVIVE_TYPE, "RPCC2G_REVIVE");
	CProtocol::RegistProtoName(RPCC2G_QUERYSHOPITEM_TYPE, "RPCC2G_QUERYSHOPITEM");
	CProtocol::RegistProtoName(RPCC2G_BUYSHOPITEM_TYPE, "RPCC2G_BUYSHOPITEM");
	CProtocol::RegistProtoName(RPCC2G_BUYGOLDANDFATIGUE_TYPE, "RPCC2G_BUYGOLDANDFATIGUE");
	CProtocol::RegistProtoName(RPCC2G_ADDTEMPATTR_TYPE, "RPCC2G_ADDTEMPATTR");
	CProtocol::RegistProtoName(RPCC2G_QUERYOPENGAMEACTIVITYTIME_TYPE, "RPCC2G_QUERYOPENGAMEACTIVITYTIME");
	CProtocol::RegistProtoName(RPCC2G_SHOWFLOWERPAGE_TYPE, "RPCC2G_SHOWFLOWERPAGE");
	CProtocol::RegistProtoName(RPCC2G_SENDFLOWER_TYPE, "RPCC2G_SENDFLOWER");
	CProtocol::RegistProtoName(RPCC2G_FETCHCHAPTERCHEST_TYPE, "RPCC2G_FETCHCHAPTERCHEST");
	CProtocol::RegistProtoName(RPCC2G_STARTGUILDCARD_TYPE, "RPCC2G_STARTGUILDCARD");
	CProtocol::RegistProtoName(RPCC2G_QUERYGUILDCARD_TYPE, "RPCC2G_QUERYGUILDCARD");
	CProtocol::RegistProtoName(RPCC2G_CHANGEGUILDCARD_TYPE, "RPCC2G_CHANGEGUILDCARD");
	CProtocol::RegistProtoName(RPCC2G_ENDGUILDCARD_TYPE, "RPCC2G_ENDGUILDCARD");
	CProtocol::RegistProtoName(RPCC2G_GETGUILDCHECKINBOX_TYPE, "RPCC2G_GETGUILDCHECKINBOX");
	CProtocol::RegistProtoName(RPCC2G_STAGECOUNTRESET_TYPE, "RPCC2G_STAGECOUNTRESET");
	CProtocol::RegistProtoName(RPCC2G_GETGUILDBONUSLIST_TYPE, "RPCC2G_GETGUILDBONUSLIST");
	CProtocol::RegistProtoName(RPCC2G_GETGUILDBONUSDETAILINFO_TYPE, "RPCC2G_GETGUILDBONUSDETAILINFO");
	CProtocol::RegistProtoName(RPCC2G_GETGUILDBONUSREWARD_TYPE, "RPCC2G_GETGUILDBONUSREWARD");
	CProtocol::RegistProtoName(RPCC2G_OPENSCENECHEST_TYPE, "RPCC2G_OPENSCENECHEST");
	CProtocol::RegistProtoName(RPCC2G_REPORTBATTLE_TYPE, "RPCC2G_REPORTBATTLE");
	CProtocol::RegistProtoName(RPCC2G_GETONLINEREWARD_TYPE, "RPCC2G_GETONLINEREWARD");
	CProtocol::RegistProtoName(RPCC2G_GETNEXTDAYREWARD_TYPE, "RPCC2G_GETNEXTDAYREWARD");
	CProtocol::RegistProtoName(RPCC2G_FETCHENEMYDOODADREQ_TYPE, "RPCC2G_FETCHENEMYDOODADREQ");
	CProtocol::RegistProtoName(RPCC2G_GUILDGOBLININFO_TYPE, "RPCC2G_GUILDGOBLININFO");
	CProtocol::RegistProtoName(RPCC2G_USESUPPLEMENT_TYPE, "RPCC2G_USESUPPLEMENT");
	CProtocol::RegistProtoName(RPCC2G_SETROLECONFIG_TYPE, "RPCC2G_SETROLECONFIG");
	CProtocol::RegistProtoName(RPCC2G_LEVELUPSLOTATTR_TYPE, "RPCC2G_LEVELUPSLOTATTR");
	CProtocol::RegistProtoName(RPCC2G_PETOPERATION_TYPE, "RPCC2G_PETOPERATION");
	CProtocol::RegistProtoName(RPCC2G_BUYTEAMSCENECOUNT_TYPE, "RPCC2G_BUYTEAMSCENECOUNT");
	CProtocol::RegistProtoName(RPCC2G_EXPFINDBACK_TYPE, "RPCC2G_EXPFINDBACK");
	CProtocol::RegistProtoName(RPCC2G_GETTOWERACTIVITYTOP_TYPE, "RPCC2G_GETTOWERACTIVITYTOP");
	CProtocol::RegistProtoName(RPCC2G_GETDESIGNATIONREQ_TYPE, "RPCC2G_GETDESIGNATIONREQ");
	CProtocol::RegistProtoName(RPCC2G_SETDESIGNATIONREQ_TYPE, "RPCC2G_SETDESIGNATIONREQ");
	CProtocol::RegistProtoName(RPCC2G_GETCLASSIFYDESIGNATIONREQ_TYPE, "RPCC2G_GETCLASSIFYDESIGNATIONREQ");
	CProtocol::RegistProtoName(RPCC2G_GETACHIEVEBRIFINFOREQ_TYPE, "RPCC2G_GETACHIEVEBRIFINFOREQ");
	CProtocol::RegistProtoName(RPCC2G_GETACHIEVECLASSIFYINFOREQ_TYPE, "RPCC2G_GETACHIEVECLASSIFYINFOREQ");
	CProtocol::RegistProtoName(RPCC2G_REQPLAYERAUTOPLAY_TYPE, "RPCC2G_REQPLAYERAUTOPLAY");
	CProtocol::RegistProtoName(RPCC2G_GETACHIEVEREWARDREQ_TYPE, "RPCC2G_GETACHIEVEREWARDREQ");
	CProtocol::RegistProtoName(RPCC2G_GETLEVELSEALINFO_TYPE, "RPCC2G_GETLEVELSEALINFO");
	CProtocol::RegistProtoName(RPCC2G_LEVELSEALBUTTONSTATUS_TYPE, "RPCC2G_LEVELSEALBUTTONSTATUS");
	CProtocol::RegistProtoName(RPCC2G_GETACHIEVEPOINTREWARDREQ_TYPE, "RPCC2G_GETACHIEVEPOINTREWARDREQ");
	CProtocol::RegistProtoName(RPCC2G_REQGETLOGINREWARD_TYPE, "RPCC2G_REQGETLOGINREWARD");
	CProtocol::RegistProtoName(RPCC2G_FIVEDAYREWARDREQ_TYPE, "RPCC2G_FIVEDAYREWARDREQ");
	CProtocol::RegistProtoName(RPCC2G_GUILDCHECKINBONUSINFO_TYPE, "RPCC2G_GUILDCHECKINBONUSINFO");
	CProtocol::RegistProtoName(RPCC2G_SENDGUILDBONUS_TYPE, "RPCC2G_SENDGUILDBONUS");
	CProtocol::RegistProtoName(RPCC2G_ASKFORCHECKINBONUS_TYPE, "RPCC2G_ASKFORCHECKINBONUS");
	CProtocol::RegistProtoName(RPCC2G_THANKSFORBONUS_TYPE, "RPCC2G_THANKSFORBONUS");
	CProtocol::RegistProtoName(RPCC2G_PVPALLREQ_TYPE, "RPCC2G_PVPALLREQ");
	CProtocol::RegistProtoName(RPCC2G_AGREEQAREQ_TYPE, "RPCC2G_AGREEQAREQ");
	CProtocol::RegistProtoName(RPCC2G_GETFLOWERLEFTTIME_TYPE, "RPCC2G_GETFLOWERLEFTTIME");
	CProtocol::RegistProtoName(RPCC2G_GETFLOWER_TYPE, "RPCC2G_GETFLOWER");
	CProtocol::RegistProtoName(RPCC2G_GETFLOWERREWARD_TYPE, "RPCC2G_GETFLOWERREWARD");
	CProtocol::RegistProtoName(RPCC2G_GETFLOWERREWARDLIST_TYPE, "RPCC2G_GETFLOWERREWARDLIST");
	CProtocol::RegistProtoName(RPCC2G_ENHANCETRANSTER_TYPE, "RPCC2G_ENHANCETRANSTER");
	CProtocol::RegistProtoName(RPCC2G_PVPNOWALLDATA_TYPE, "RPCC2G_PVPNOWALLDATA");
	CProtocol::RegistProtoName(RPCC2G_GETDRAGONTOPINFO_TYPE, "RPCC2G_GETDRAGONTOPINFO");
	CProtocol::RegistProtoName(RPCG2M_REGISTERGS2MS_TYPE, "RPCG2M_REGISTERGS2MS");
	CProtocol::RegistProtoName(RPCC2G_TEAMINVITELISTREQ_TYPE, "RPCC2G_TEAMINVITELISTREQ");
	CProtocol::RegistProtoName(RPCG2T_REGISTERGS2GT_TYPE, "RPCG2T_REGISTERGS2GT");
	CProtocol::RegistProtoName(RPCT2G_REGISTERGT2GS_TYPE, "RPCT2G_REGISTERGT2GS");
	CProtocol::RegistProtoName(RPCN2G_ENTERSCENE_TYPE, "RPCN2G_ENTERSCENE");
	CProtocol::RegistProtoName(RPCN2G_LEAVESCENE_TYPE, "RPCN2G_LEAVESCENE");
	CProtocol::RegistProtoName(RPCG2D_SAVEROLEDATA_TYPE, "RPCG2D_SAVEROLEDATA");
	CProtocol::RegistProtoName(RPCC2G_IDENTIFYEMBLEM_TYPE, "RPCC2G_IDENTIFYEMBLEM");
	CProtocol::RegistProtoName(RPCC2G_OPENGUILDQAREQ_TYPE, "RPCC2G_OPENGUILDQAREQ");
	CProtocol::RegistProtoName(RPCC2G_GETGUILDQADATAREQ_TYPE, "RPCC2G_GETGUILDQADATAREQ");
	CProtocol::RegistProtoName(RPCC2G_MULACTIVITYREQ_TYPE, "RPCC2G_MULACTIVITYREQ");
	CProtocol::RegistProtoName(RPCC2G_GETWATCHINFOBYID_TYPE, "RPCC2G_GETWATCHINFOBYID");
	CProtocol::RegistProtoName(RPCC2G_GETMYWATCHRECORD_TYPE, "RPCC2G_GETMYWATCHRECORD");
	CProtocol::RegistProtoName(RPCC2G_ENTERWATCHBATTLE_TYPE, "RPCC2G_ENTERWATCHBATTLE");
	CProtocol::RegistProtoName(RPCC2G_CHANGEWATCHROLE_TYPE, "RPCC2G_CHANGEWATCHROLE");
	CProtocol::RegistProtoName(RPCC2G_COMMENDWATCHBATTLE_TYPE, "RPCC2G_COMMENDWATCHBATTLE");
	CProtocol::RegistProtoName(RPCC2G_BOSSRUSHREQ_TYPE, "RPCC2G_BOSSRUSHREQ");
	CProtocol::RegistProtoName(RPCC2G_DOENTERSCENE_TYPE, "RPCC2G_DOENTERSCENE");
	CProtocol::RegistProtoName(RPCG2N_CHANGESCENEVERIFY_TYPE, "RPCG2N_CHANGESCENEVERIFY");
	CProtocol::RegistProtoName(RPCC2G_TAKEOFFALLJADE_TYPE, "RPCC2G_TAKEOFFALLJADE");
	CProtocol::RegistProtoName(RPCC2G_SWEEPTOWER_TYPE, "RPCC2G_SWEEPTOWER");
	CProtocol::RegistProtoName(RPCC2G_RESETTOWER_TYPE, "RPCC2G_RESETTOWER");
	CProtocol::RegistProtoName(RPCM2G_GSHTTPCOMMANDNEW_TYPE, "RPCM2G_GSHTTPCOMMANDNEW");
	CProtocol::RegistProtoName(RPCC2G_GETGODDESSTRIALREWARDS_TYPE, "RPCC2G_GETGODDESSTRIALREWARDS");
	CProtocol::RegistProtoName(RPCN2G_CREATEBATTLE_TYPE, "RPCN2G_CREATEBATTLE");
	CProtocol::RegistProtoName(RPCM2G_CHATCANSEND_TYPE, "RPCM2G_CHATCANSEND");
	CProtocol::RegistProtoName(RPCG2M_GETWATCHINFOBYTYPE_TYPE, "RPCG2M_GETWATCHINFOBYTYPE");
	CProtocol::RegistProtoName(RPCG2M_ENTERWATCHBATTLEMS_TYPE, "RPCG2M_ENTERWATCHBATTLEMS");
	CProtocol::RegistProtoName(RPCG2M_INCLEVELSEALBOSSCOUNT_TYPE, "RPCG2M_INCLEVELSEALBOSSCOUNT");
	CProtocol::RegistProtoName(RPCM2G_NOTICEGUILDBOSSDOENDTOGSN_TYPE, "RPCM2G_NOTICEGUILDBOSSDOENDTOGSN");
	CProtocol::RegistProtoName(RPCC2G_QUERYPOWERPOINT_TYPE, "RPCC2G_QUERYPOWERPOINT");
	CProtocol::RegistProtoName(RPCC2G_GETPAYALLINFO_TYPE, "RPCC2G_GETPAYALLINFO");
	CProtocol::RegistProtoName(RPCC2G_PAYCARDAWARD_TYPE, "RPCC2G_PAYCARDAWARD");
	CProtocol::RegistProtoName(RPCC2G_PAYCLICK_TYPE, "RPCC2G_PAYCLICK");
	CProtocol::RegistProtoName(RPCC2G_GETACTIVITYINFO_TYPE, "RPCC2G_GETACTIVITYINFO");
	CProtocol::RegistProtoName(RPCG2M_MSGETFLWOERRANKREWARD_TYPE, "RPCG2M_MSGETFLWOERRANKREWARD");
	CProtocol::RegistProtoName(RPCG2M_MSSENDFLOWER_TYPE, "RPCG2M_MSSENDFLOWER");
	CProtocol::RegistProtoName(RPCM2G_GETFLOWERREWARDSTATE_TYPE, "RPCM2G_GETFLOWERREWARDSTATE");
	CProtocol::RegistProtoName(RPCM2G_TEAMSYNM2C_TYPE, "RPCM2G_TEAMSYNM2C");
	CProtocol::RegistProtoName(RPCM2G_TEAMSTARTBATTLEM2G_TYPE, "RPCM2G_TEAMSTARTBATTLEM2G");
	CProtocol::RegistProtoName(RPCM2G_TEAMREQUESTM2G_TYPE, "RPCM2G_TEAMREQUESTM2G");
	CProtocol::RegistProtoName(RPCC2G_SYNPETINFO_TYPE, "RPCC2G_SYNPETINFO");
	CProtocol::RegistProtoName(RPCC2G_CHOOSEROLLREQ_TYPE, "RPCC2G_CHOOSEROLLREQ");
	CProtocol::RegistProtoName(RPCC2G_PAYFIRSTAWARD_TYPE, "RPCC2G_PAYFIRSTAWARD");
	CProtocol::RegistProtoName(RPCC2G_GROWTHFUNDAWARD_TYPE, "RPCC2G_GROWTHFUNDAWARD");
	CProtocol::RegistProtoName(RPCC2G_BUYVIPLEVELGIFT_TYPE, "RPCC2G_BUYVIPLEVELGIFT");
	CProtocol::RegistProtoName(RPCC2G_TITLELEVELUP_TYPE, "RPCC2G_TITLELEVELUP");
	CProtocol::RegistProtoName(RPCC2G_FIRSTPASSINFOREQ_TYPE, "RPCC2G_FIRSTPASSINFOREQ");
	CProtocol::RegistProtoName(RPCC2G_GETFIRSTPASSREWARD_TYPE, "RPCC2G_GETFIRSTPASSREWARD");
	CProtocol::RegistProtoName(RPCC2G_COMMENDFIRSTPASS_TYPE, "RPCC2G_COMMENDFIRSTPASS");
	CProtocol::RegistProtoName(RPCG2M_MSCOMMENDFIRSTPASS_TYPE, "RPCG2M_MSCOMMENDFIRSTPASS");
	CProtocol::RegistProtoName(RPCC2G_FIRSTPASSGETTOPROLEINFO_TYPE, "RPCC2G_FIRSTPASSGETTOPROLEINFO");
	CProtocol::RegistProtoName(RPCG2M_GETRANKINFO_TYPE, "RPCG2M_GETRANKINFO");
	CProtocol::RegistProtoName(RPCG2M_PVPALLG2MREQ_TYPE, "RPCG2M_PVPALLG2MREQ");
	CProtocol::RegistProtoName(RPCG2M_GMCOMMANDG2M_TYPE, "RPCG2M_GMCOMMANDG2M");
	CProtocol::RegistProtoName(RPCM2G_MSUSESUMMARYREQ_TYPE, "RPCM2G_MSUSESUMMARYREQ");
	CProtocol::RegistProtoName(RPCM2G_TEAMCHANGEEXPCHECK_TYPE, "RPCM2G_TEAMCHANGEEXPCHECK");
	CProtocol::RegistProtoName(RPCG2M_UPDATEMSRANKLIST_TYPE, "RPCG2M_UPDATEMSRANKLIST");
	CProtocol::RegistProtoName(RPCG2M_PKRANKREQ_TYPE, "RPCG2M_PKRANKREQ");
	CProtocol::RegistProtoName(RPCM2G_PKREQM2G_TYPE, "RPCM2G_PKREQM2G");
	CProtocol::RegistProtoName(RPCM2G_CREATEGUILDCOST_TYPE, "RPCM2G_CREATEGUILDCOST");
	CProtocol::RegistProtoName(RPCM2G_AUCTIONALLM2GREQ_TYPE, "RPCM2G_AUCTIONALLM2GREQ");
	CProtocol::RegistProtoName(RPCM2G_ENTERGUILDPOWER_TYPE, "RPCM2G_ENTERGUILDPOWER");
	CProtocol::RegistProtoName(RPCC2G_CHANGEOUTLOOKOP_TYPE, "RPCC2G_CHANGEOUTLOOKOP");
	CProtocol::RegistProtoName(RPCC2G_ITEMFINDBACKINFO_TYPE, "RPCC2G_ITEMFINDBACKINFO");
	CProtocol::RegistProtoName(RPCC2G_ITEMFINDBACK_TYPE, "RPCC2G_ITEMFINDBACK");
	CProtocol::RegistProtoName(RPCC2G_ACTIVATATLAS_TYPE, "RPCC2G_ACTIVATATLAS");
	CProtocol::RegistProtoName(RPCC2G_AUTOBREAKATLAS_TYPE, "RPCC2G_AUTOBREAKATLAS");
	CProtocol::RegistProtoName(RPCC2G_BREAKATLAS_TYPE, "RPCC2G_BREAKATLAS");
	CProtocol::RegistProtoName(RPCC2G_BUYIBITEM_TYPE, "RPCC2G_BUYIBITEM");
	CProtocol::RegistProtoName(RPCC2G_QUERYIBITEM_TYPE, "RPCC2G_QUERYIBITEM");
	CProtocol::RegistProtoName(RPCC2G_SPRITEOPERATION_TYPE, "RPCC2G_SPRITEOPERATION");
	CProtocol::RegistProtoName(RPCC2G_DEPROGRESSREQ_TYPE, "RPCC2G_DEPROGRESSREQ");
	CProtocol::RegistProtoName(RPCC2G_GETRISKMAPINFOS_TYPE, "RPCC2G_GETRISKMAPINFOS");
	CProtocol::RegistProtoName(RPCC2G_PLAYDICEREQUEST_TYPE, "RPCC2G_PLAYDICEREQUEST");
	CProtocol::RegistProtoName(RPCC2G_CHANGERISKBOXSTATE_TYPE, "RPCC2G_CHANGERISKBOXSTATE");
	CProtocol::RegistProtoName(RPCC2G_REENTERRISKBATTLE_TYPE, "RPCC2G_REENTERRISKBATTLE");
	CProtocol::RegistProtoName(RPCC2G_LEVELSEALEXCHANGE_TYPE, "RPCC2G_LEVELSEALEXCHANGE");
	CProtocol::RegistProtoName(RPCC2G_PLAYDICEOVER_TYPE, "RPCC2G_PLAYDICEOVER");
	CProtocol::RegistProtoName(RPCG2M_OPGUILDREQDATA_TYPE, "RPCG2M_OPGUILDREQDATA");
	CProtocol::RegistProtoName(RPCC2G_RISKBUYREQUEST_TYPE, "RPCC2G_RISKBUYREQUEST");
	CProtocol::RegistProtoName(RPCC2G_SMELTITEM_TYPE, "RPCC2G_SMELTITEM");
	CProtocol::RegistProtoName(RPCM2G_STARTARENAGS_TYPE, "RPCM2G_STARTARENAGS");
	CProtocol::RegistProtoName(RPCC2G_REFRESHSWEEPREWARD_TYPE, "RPCC2G_REFRESHSWEEPREWARD");
	CProtocol::RegistProtoName(RPCC2G_GETSWEEPTOWERREWARD_TYPE, "RPCC2G_GETSWEEPTOWERREWARD");
	CProtocol::RegistProtoName(RPCM2G_REQGUILDDAREREWARDREC_TYPE, "RPCM2G_REQGUILDDAREREWARDREC");
	CProtocol::RegistProtoName(RPCG2M_CHECKGUILDDAREREWARD_TYPE, "RPCG2M_CHECKGUILDDAREREWARD");
	CProtocol::RegistProtoName(RPCC2G_LEARNGUILDSKILL_TYPE, "RPCC2G_LEARNGUILDSKILL");
	CProtocol::RegistProtoName(RPCG2M_ENDAREANMS_TYPE, "RPCG2M_ENDAREANMS");
	CProtocol::RegistProtoName(RPCM2G_REQROLEGUILDSKILL_TYPE, "RPCM2G_REQROLEGUILDSKILL");
	CProtocol::RegistProtoName(RPCG2M_MGUILDBONUSOPERATEREQ_TYPE, "RPCG2M_MGUILDBONUSOPERATEREQ");
	CProtocol::RegistProtoName(RPCC2G_QUERYSCENETIME_TYPE, "RPCC2G_QUERYSCENETIME");
	CProtocol::RegistProtoName(RPCM2G_REQGUILDACTIVITY_TYPE, "RPCM2G_REQGUILDACTIVITY");
	CProtocol::RegistProtoName(RPCM2G_REQGUILDCHECKIN_TYPE, "RPCM2G_REQGUILDCHECKIN");
	CProtocol::RegistProtoName(RPCM2G_NOTICEGUILDCHECKIN_TYPE, "RPCM2G_NOTICEGUILDCHECKIN");
	CProtocol::RegistProtoName(RPCG2M_NOTIFYCHECKINBOX_TYPE, "RPCG2M_NOTIFYCHECKINBOX");
	CProtocol::RegistProtoName(RPCG2M_GUILDCARDSYNCMS_TYPE, "RPCG2M_GUILDCARDSYNCMS");
	CProtocol::RegistProtoName(RPCC2G_GETTOWERFIRSTPASSREWARD_TYPE, "RPCC2G_GETTOWERFIRSTPASSREWARD");
	CProtocol::RegistProtoName(RPCM2G_REQRECVFATIGUE_TYPE, "RPCM2G_REQRECVFATIGUE");
	CProtocol::RegistProtoName(RPCM2G_DESIGNATIONEVENTREQ_TYPE, "RPCM2G_DESIGNATIONEVENTREQ");
	CProtocol::RegistProtoName(RPCM2G_GSRELOAD_TYPE, "RPCM2G_GSRELOAD");
	CProtocol::RegistProtoName(RPCG2M_REQGUILDLADDERREWARD_TYPE, "RPCG2M_REQGUILDLADDERREWARD");
	CProtocol::RegistProtoName(RPCM2G_MSCONSUMEITEMS_TYPE, "RPCM2G_MSCONSUMEITEMS");
	CProtocol::RegistProtoName(RPCC2G_GMFREADYREQ_TYPE, "RPCC2G_GMFREADYREQ");
	CProtocol::RegistProtoName(RPCC2G_INSPIREREQ_TYPE, "RPCC2G_INSPIREREQ");
	CProtocol::RegistProtoName(RPCC2G_REQGUILDBOSSTIMELEFT_TYPE, "RPCC2G_REQGUILDBOSSTIMELEFT");
	CProtocol::RegistProtoName(RPCC2G_SKYCITYALLINFOREQ_TYPE, "RPCC2G_SKYCITYALLINFOREQ");
	CProtocol::RegistProtoName(RPCG2M_ENTERQAROOM_TYPE, "RPCG2M_ENTERQAROOM");
	CProtocol::RegistProtoName(RPCG2M_CREATEGUILDQAROOM_TYPE, "RPCG2M_CREATEGUILDQAROOM");
	CProtocol::RegistProtoName(RPCC2G_GETSPACTIVITYREWARD_TYPE, "RPCC2G_GETSPACTIVITYREWARD");
	CProtocol::RegistProtoName(RPCC2G_GETSPACTIVITYBIGPRIZE_TYPE, "RPCC2G_GETSPACTIVITYBIGPRIZE");
	CProtocol::RegistProtoName(RPCM2G_GARDENPLAYEVENT_TYPE, "RPCM2G_GARDENPLAYEVENT");
	CProtocol::RegistProtoName(RPCM2G_GARDENPLANTCHANGESTAGE_TYPE, "RPCM2G_GARDENPLANTCHANGESTAGE");
	CProtocol::RegistProtoName(RPCM2G_GETGUILDLADDERREWARDTIME_TYPE, "RPCM2G_GETGUILDLADDERREWARDTIME");
	CProtocol::RegistProtoName(RPCT2G_RECONNECTGS_TYPE, "RPCT2G_RECONNECTGS");
	CProtocol::RegistProtoName(RPCC2G_GETLEVELSEALSELFGIFT_TYPE, "RPCC2G_GETLEVELSEALSELFGIFT");
	CProtocol::RegistProtoName(RPCM2G_QUERYROLECARDMATCH_TYPE, "RPCM2G_QUERYROLECARDMATCH");
	CProtocol::RegistProtoName(RPCG2R_REGISTERGS2ROUTER_TYPE, "RPCG2R_REGISTERGS2ROUTER");
	CProtocol::RegistProtoName(RPCG2K_REGISTERGS2WORLD_TYPE, "RPCG2K_REGISTERGS2WORLD");
	CProtocol::RegistProtoName(RPCK2G_CREATEBATTLECROSS_TYPE, "RPCK2G_CREATEBATTLECROSS");
	CProtocol::RegistProtoName(RPCC2G_GOLDCLICK_TYPE, "RPCC2G_GOLDCLICK");
	CProtocol::RegistProtoName(RPCG2N_REGISTERGS2NS_TYPE, "RPCG2N_REGISTERGS2NS");
	CProtocol::RegistProtoName(RPCM2G_GIVEMAILPRIZE_TYPE, "RPCM2G_GIVEMAILPRIZE");
	CProtocol::RegistProtoName(RPCM2G_GUILDAUCTREQM2C_TYPE, "RPCM2G_GUILDAUCTREQM2C");
	CProtocol::RegistProtoName(RPCC2G_RESWARALLINFOREQONE_TYPE, "RPCC2G_RESWARALLINFOREQONE");
	CProtocol::RegistProtoName(RPCM2G_GUILDAUCTFAILBACK_TYPE, "RPCM2G_GUILDAUCTFAILBACK");
	CProtocol::RegistProtoName(RPCG2M_GSDESIGNATIONEVENTREQ_TYPE, "RPCG2M_GSDESIGNATIONEVENTREQ");
	CProtocol::RegistProtoName(RPCC2G_ITEMBUFFOP_TYPE, "RPCC2G_ITEMBUFFOP");
	CProtocol::RegistProtoName(RPCC2G_JUSTDANCE_TYPE, "RPCC2G_JUSTDANCE");
	CProtocol::RegistProtoName(RPCC2G_GETDAILYTASKINFO_TYPE, "RPCC2G_GETDAILYTASKINFO");
	CProtocol::RegistProtoName(RPCC2G_GETDAILYTASKREWARD_TYPE, "RPCC2G_GETDAILYTASKREWARD");
	CProtocol::RegistProtoName(RPCC2G_DAILYTASKASKHELP_TYPE, "RPCC2G_DAILYTASKASKHELP");
	CProtocol::RegistProtoName(RPCG2M_ASKGUILDHELP_TYPE, "RPCG2M_ASKGUILDHELP");
	CProtocol::RegistProtoName(RPCM2G_COSTDONATEITEM_TYPE, "RPCM2G_COSTDONATEITEM");
	CProtocol::RegistProtoName(RPCM2G_QUERYDONATEINFO_TYPE, "RPCM2G_QUERYDONATEINFO");
	CProtocol::RegistProtoName(RPCM2G_TRYADDGUILDINHERIT_TYPE, "RPCM2G_TRYADDGUILDINHERIT");
	CProtocol::RegistProtoName(RPCC2G_QUERYQQVIPINFO_TYPE, "RPCC2G_QUERYQQVIPINFO");
	CProtocol::RegistProtoName(RPCC2G_DAILYTASKGIVEUP_TYPE, "RPCC2G_DAILYTASKGIVEUP");
	CProtocol::RegistProtoName(RPCM2G_CRASHRECOVERM2G_TYPE, "RPCM2G_CRASHRECOVERM2G");
	CProtocol::RegistProtoName(RPCM2G_FETCHROLEINFOONMS_TYPE, "RPCM2G_FETCHROLEINFOONMS");
	CProtocol::RegistProtoName(RPCC2G_RESWARBUFF_TYPE, "RPCC2G_RESWARBUFF");
	CProtocol::RegistProtoName(RPCG2M_TRYTEAMCOSTREWARD_TYPE, "RPCG2M_TRYTEAMCOSTREWARD");
	CProtocol::RegistProtoName(RPCM2G_CHANGENAMEGS_TYPE, "RPCM2G_CHANGENAMEGS");
	CProtocol::RegistProtoName(RPCC2G_QUERYQQFRIENDSVIPINFO_TYPE, "RPCC2G_QUERYQQFRIENDSVIPINFO");
	CProtocol::RegistProtoName(RPCM2G_REQGUILDINHERITTIMES_TYPE, "RPCM2G_REQGUILDINHERITTIMES");
	CProtocol::RegistProtoName(RPCC2G_ENCHANTEQUIP_TYPE, "RPCC2G_ENCHANTEQUIP");
	CProtocol::RegistProtoName(RPCM2G_MAKEPARTNERM2G_TYPE, "RPCM2G_MAKEPARTNERM2G");
	CProtocol::RegistProtoName(RPCC2G_TAKEPARTNERCHEST_TYPE, "RPCC2G_TAKEPARTNERCHEST");
	CProtocol::RegistProtoName(RPCG2M_GETPARTNERINFOG2M_TYPE, "RPCG2M_GETPARTNERINFOG2M");
	CProtocol::RegistProtoName(RPCM2G_GETPARTNERRECORDM2G_TYPE, "RPCM2G_GETPARTNERRECORDM2G");
	CProtocol::RegistProtoName(RPCC2G_GETHEROBATTLEINFO_TYPE, "RPCC2G_GETHEROBATTLEINFO");
	CProtocol::RegistProtoName(RPCC2G_HORSERECONNECT_TYPE, "RPCC2G_HORSERECONNECT");
	CProtocol::RegistProtoName(RPCC2G_PHOTOGRAPHEFFECT_TYPE, "RPCC2G_PHOTOGRAPHEFFECT");
	CProtocol::RegistProtoName(RPCG2M_GETCHARM_TYPE, "RPCG2M_GETCHARM");
	CProtocol::RegistProtoName(RPCM2G_INVFIGHTM2GREQ_TYPE, "RPCM2G_INVFIGHTM2GREQ");
	CProtocol::RegistProtoName(RPCM2G_BUYPARTNERITEMM2G_TYPE, "RPCM2G_BUYPARTNERITEMM2G");
	CProtocol::RegistProtoName(RPCM2G_POKERTOURNAMENTSIGNUPCOST_TYPE, "RPCM2G_POKERTOURNAMENTSIGNUPCOST");
	CProtocol::RegistProtoName(RPCC2G_PANDORALOTTERY_TYPE, "RPCC2G_PANDORALOTTERY");
	CProtocol::RegistProtoName(RPCC2G_QUERYCLIENTIP_TYPE, "RPCC2G_QUERYCLIENTIP");
	CProtocol::RegistProtoName(RPCC2G_SETHEROINHEROBATTLE_TYPE, "RPCC2G_SETHEROINHEROBATTLE");
	CProtocol::RegistProtoName(RPCM2G_PAYGETBALANCE_TYPE, "RPCM2G_PAYGETBALANCE");
	CProtocol::RegistProtoName(RPCM2G_PAYCONSUMEADDGOODS_TYPE, "RPCM2G_PAYCONSUMEADDGOODS");
	CProtocol::RegistProtoName(RPCC2G_GETGUILDBONUSLEFT_TYPE, "RPCC2G_GETGUILDBONUSLEFT");
	CProtocol::RegistProtoName(RPCC2G_GETHEROBATTLEGAMERECORD_TYPE, "RPCC2G_GETHEROBATTLEGAMERECORD");
	CProtocol::RegistProtoName(RPCC2G_BUYHEROINHEROBATTLE_TYPE, "RPCC2G_BUYHEROINHEROBATTLE");
	CProtocol::RegistProtoName(RPCC2G_GETHEROBATTLEWEEKREWARD_TYPE, "RPCC2G_GETHEROBATTLEWEEKREWARD");
	CProtocol::RegistProtoName(RPCM2G_GCFCOMMONREQM2G_TYPE, "RPCM2G_GCFCOMMONREQM2G");
	CProtocol::RegistProtoName(RPCG2M_GETGARDENINFO_TYPE, "RPCG2M_GETGARDENINFO");
	CProtocol::RegistProtoName(RPCC2G_ATLASUPSTAR_TYPE, "RPCC2G_ATLASUPSTAR");
	CProtocol::RegistProtoName(RPCC2G_LEAGUEBATTLEREADYREQ_TYPE, "RPCC2G_LEAGUEBATTLEREADYREQ");
	CProtocol::RegistProtoName(RPCC2G_FORGEEQUIP_TYPE, "RPCC2G_FORGEEQUIP");
	CProtocol::RegistProtoName(RPCC2G_CHANGEPROFESSION_TYPE, "RPCC2G_CHANGEPROFESSION");
	CProtocol::RegistProtoName(RPCG2M_LEAGUERANKUPDATEG2M_TYPE, "RPCG2M_LEAGUERANKUPDATEG2M");
	CProtocol::RegistProtoName(RPCG2M_CHANGEPROFESSIONMS_TYPE, "RPCG2M_CHANGEPROFESSIONMS");
	CProtocol::RegistProtoName(RPCG2D_READHIDDENROLES_TYPE, "RPCG2D_READHIDDENROLES");
	CProtocol::RegistProtoName(RPCC2G_GETHOLIDAYSTAGEINFO_TYPE, "RPCC2G_GETHOLIDAYSTAGEINFO");
	CProtocol::RegistProtoName(RPCC2G_ITEMSELL_TYPE, "RPCC2G_ITEMSELL");
	CProtocol::RegistProtoName(RPCC2G_ENCHANTTRANSFER_TYPE, "RPCC2G_ENCHANTTRANSFER");
	CProtocol::RegistProtoName(RPCC2G_PERSONALCAREER_TYPE, "RPCC2G_PERSONALCAREER");
	CProtocol::RegistProtoName(RPCG2M_PERSONALCAREERTOTALINFO_TYPE, "RPCG2M_PERSONALCAREERTOTALINFO");
	CProtocol::RegistProtoName(RPCC2G_CHANGEDECLARATION_TYPE, "RPCC2G_CHANGEDECLARATION");
	CProtocol::RegistProtoName(RPCC2G_CHANGESKILLSET_TYPE, "RPCC2G_CHANGESKILLSET");
	CProtocol::RegistProtoName(RPCG2K_LEAGUERANKUPDATEG2K_TYPE, "RPCG2K_LEAGUERANKUPDATEG2K");
	CProtocol::RegistProtoName(RPCM2G_CHECKNOTICE_TYPE, "RPCM2G_CHECKNOTICE");
	CProtocol::RegistProtoName(RPCC2G_GETPAYREWARD_TYPE, "RPCC2G_GETPAYREWARD");
	CProtocol::RegistProtoName(RPCG2M_GETMULACTOPENINFO_TYPE, "RPCG2M_GETMULACTOPENINFO");
	CProtocol::RegistProtoName(RPCK2G_CROSSGSRELOAD_TYPE, "RPCK2G_CROSSGSRELOAD");
	CProtocol::RegistProtoName(RPCG2M_GETMILITARYRANK_TYPE, "RPCG2M_GETMILITARYRANK");
	CProtocol::RegistProtoName(RPCC2G_PAYFRIENDITEM_TYPE, "RPCC2G_PAYFRIENDITEM");
	CProtocol::RegistProtoName(RPCG2M_UPPAYFRIENDITEM_TYPE, "RPCG2M_UPPAYFRIENDITEM");
	CProtocol::RegistProtoName(RPCG2M_SYNCACTIVITYEND2MS_TYPE, "RPCG2M_SYNCACTIVITYEND2MS");
	CProtocol::RegistProtoName(RPCC2G_ITEMCOMPOSE_TYPE, "RPCC2G_ITEMCOMPOSE");
	CProtocol::RegistProtoName(RPCC2G_GETBACKFLOWSPACTTIME_TYPE, "RPCC2G_GETBACKFLOWSPACTTIME");
	CProtocol::RegistProtoName(RPCM2G_WORLDBOSSGUILDADDATTRGS_TYPE, "RPCM2G_WORLDBOSSGUILDADDATTRGS");
	CProtocol::RegistProtoName(RPCC2G_ARGENTAACTIVITY_TYPE, "RPCC2G_ARGENTAACTIVITY");
	CProtocol::RegistProtoName(RPCM2G_MODIFYGSGUILDNAME_TYPE, "RPCM2G_MODIFYGSGUILDNAME");
	CProtocol::RegistProtoName(RPCC2G_GETPLATFORMSHARECHEST_TYPE, "RPCC2G_GETPLATFORMSHARECHEST");
	CProtocol::RegistProtoName(RPCC2G_LOGINACTIVITY_TYPE, "RPCC2G_LOGINACTIVITY");
	CProtocol::RegistProtoName(RPCC2G_GETGROWTHFUNDTIME_TYPE, "RPCC2G_GETGROWTHFUNDTIME");
	CProtocol::RegistProtoName(RPCM2G_GIFTIBITEMREQ_TYPE, "RPCM2G_GIFTIBITEMREQ");
	CProtocol::RegistProtoName(RPCC2G_ACTIVATEFASHIONCHARM_TYPE, "RPCC2G_ACTIVATEFASHIONCHARM");
	CProtocol::RegistProtoName(RPCG2M_PAYDEGREEREQ_TYPE, "RPCG2M_PAYDEGREEREQ");
	CProtocol::RegistProtoName(RPCC2G_GETGUILDCAMPARTYRANDITEM_TYPE, "RPCC2G_GETGUILDCAMPARTYRANDITEM");
	CProtocol::RegistProtoName(RPCC2G_TALKGUILDCAMPSPRITE_TYPE, "RPCC2G_TALKGUILDCAMPSPRITE");
	CProtocol::RegistProtoName(RPCC2G_GUILDCAMPPARTYREQEXCHANGE_TYPE, "RPCC2G_GUILDCAMPPARTYREQEXCHANGE");
	CProtocol::RegistProtoName(RPCC2G_GETGUILDCAMPPARTYEXCHANGEINFO_TYPE, "RPCC2G_GETGUILDCAMPPARTYEXCHANGEINFO");
	CProtocol::RegistProtoName(RPCM2G_CUSTOMBATTLEGSOP_TYPE, "RPCM2G_CUSTOMBATTLEGSOP");
	CProtocol::RegistProtoName(RPCC2G_GUILDCAMPEXCHANGEOPERATE_TYPE, "RPCC2G_GUILDCAMPEXCHANGEOPERATE");
	CProtocol::RegistProtoName(RPCC2G_GETGUILDCAMPPARTYREWARD_TYPE, "RPCC2G_GETGUILDCAMPPARTYREWARD");
	CProtocol::RegistProtoName(RPCM2G_GIFTIBREQGSGOODS_TYPE, "RPCM2G_GIFTIBREQGSGOODS");
	CProtocol::RegistProtoName(RPCC2G_ARTIFACTCOMPOSE_TYPE, "RPCC2G_ARTIFACTCOMPOSE");
	CProtocol::RegistProtoName(RPCC2G_REPLYPARTYEXCHANGEITEMOPT_TYPE, "RPCC2G_REPLYPARTYEXCHANGEITEMOPT");
	CProtocol::RegistProtoName(RPCC2G_ABSENTERSCENE_TYPE, "RPCC2G_ABSENTERSCENE");
	CProtocol::RegistProtoName(RPCC2G_ENCHANTACTIVEATTRIBUTE_TYPE, "RPCC2G_ENCHANTACTIVEATTRIBUTE");
	CProtocol::RegistProtoName(RPCC2G_BUYEXTRASKILLEMBLEMSLOT_TYPE, "RPCC2G_BUYEXTRASKILLEMBLEMSLOT");
	CProtocol::RegistProtoName(RPCC2G_CHANGELIVEVISIBLE_TYPE, "RPCC2G_CHANGELIVEVISIBLE");
	CProtocol::RegistProtoName(RPCG2M_CUSTOMBATTLERESULTMS_TYPE, "RPCG2M_CUSTOMBATTLERESULTMS");
	CProtocol::RegistProtoName(RPCG2K_CUSTOMBATTLERESULTWORLD_TYPE, "RPCG2K_CUSTOMBATTLERESULTWORLD");
	CProtocol::RegistProtoName(RPCC2G_REPORTBADPLAYER_TYPE, "RPCC2G_REPORTBADPLAYER");
	CProtocol::RegistProtoName(RPCM2G_TEAMTRANMS2GS_TYPE, "RPCM2G_TEAMTRANMS2GS");
	CProtocol::RegistProtoName(RPCC2G_GETGUILDPARTYRECEIVEINFO_TYPE, "RPCC2G_GETGUILDPARTYRECEIVEINFO");
	CProtocol::RegistProtoName(RPCC2G_RETURNSMELTSTONE_TYPE, "RPCC2G_RETURNSMELTSTONE");
	CProtocol::RegistProtoName(RPCM2G_KMATCHCOMMONREQM2C_TYPE, "RPCM2G_KMATCHCOMMONREQM2C");
	CProtocol::RegistProtoName(RPCC2G_GETPLATSHAREAWARD_TYPE, "RPCC2G_GETPLATSHAREAWARD");
	CProtocol::RegistProtoName(RPCC2G_WEEKEND4V4GETINFO_TYPE, "RPCC2G_WEEKEND4V4GETINFO");
	CProtocol::RegistProtoName(RPCC2G_SCENEMOBAOP_TYPE, "RPCC2G_SCENEMOBAOP");
	CProtocol::RegistProtoName(RPCC2G_MOBASIGNALING_TYPE, "RPCC2G_MOBASIGNALING");
	CProtocol::RegistProtoName(RPCC2G_ACTIVATEHAIRCOLOR_TYPE, "RPCC2G_ACTIVATEHAIRCOLOR");
	CProtocol::RegistProtoName(RPCM2G_TRYTEACHERINHERITNEW_TYPE, "RPCM2G_TRYTEACHERINHERITNEW");
	CProtocol::RegistProtoName(RPCC2G_DHRREQC2G_TYPE, "RPCC2G_DHRREQC2G");
	CProtocol::RegistProtoName(RPCC2G_PETOPERATIONOTHER_TYPE, "RPCC2G_PETOPERATIONOTHER");
	CProtocol::RegistProtoName(RPCN2G_NOTIFYROLELOGINRECONNECT2GS_TYPE, "RPCN2G_NOTIFYROLELOGINRECONNECT2GS");
	CProtocol::RegistProtoName(RPCG2K_REPORTBMROLEINFO_TYPE, "RPCG2K_REPORTBMROLEINFO");
	CProtocol::RegistProtoName(RPCM2G_MSGIVEITEM_TYPE, "RPCM2G_MSGIVEITEM");
	CProtocol::RegistProtoName(RPCM2G_TEAMSTARTBATTLEM2GCROSS_TYPE, "RPCM2G_TEAMSTARTBATTLEM2GCROSS");
	CProtocol::RegistProtoName(RPCC2G_BUYSPRITEEGG_TYPE, "RPCC2G_BUYSPRITEEGG");
	CProtocol::RegistProtoName(RPCC2G_GETENHANCEATTR_TYPE, "RPCC2G_GETENHANCEATTR");
	CProtocol::RegistProtoName(RPCC2G_GETANCIENTTIMESAWARD_TYPE, "RPCC2G_GETANCIENTTIMESAWARD");
	CProtocol::RegistProtoName(RPCC2G_SELECTHEROANCIENTPOWER_TYPE, "RPCC2G_SELECTHEROANCIENTPOWER");
	CProtocol::RegistProtoName(RPCM2G_CHECKMARRIAGECOUPLE_TYPE, "RPCM2G_CHECKMARRIAGECOUPLE");
	CProtocol::RegistProtoName(RPCC2G_WEDDINGOPERATOR_TYPE, "RPCC2G_WEDDINGOPERATOR");
	CProtocol::RegistProtoName(RPCC2G_TACTENTERSCENE_TYPE, "RPCC2G_TACTENTERSCENE");
	CProtocol::RegistProtoName(RPCC2G_DRAGONGROUPROLELIST_TYPE, "RPCC2G_DRAGONGROUPROLELIST");
	CProtocol::RegistProtoName(RPCC2G_DRAGONGROUPRECORD_TYPE, "RPCC2G_DRAGONGROUPRECORD");
	CProtocol::RegistProtoName(RPCM2G_GIVEMARRIAGELVREREWARDM2G_TYPE, "RPCM2G_GIVEMARRIAGELVREREWARDM2G");
	CProtocol::RegistProtoName(RPCM2G_HOLDWEDDINGCHECKM2G_TYPE, "RPCM2G_HOLDWEDDINGCHECKM2G");
	CProtocol::RegistProtoName(RPCC2G_ARTIFACTDEITYSTOVEOP_TYPE, "RPCC2G_ARTIFACTDEITYSTOVEOP");
	CProtocol::RegistProtoName(RPCM2G_WEDDINGOPERM2G_TYPE, "RPCM2G_WEDDINGOPERM2G");
	CProtocol::RegistProtoName(RPCC2G_THEMEACTIVITYHINT_TYPE, "RPCC2G_THEMEACTIVITYHINT");
	CProtocol::RegistProtoName(RPCC2G_BACKFLOWBUYOP_TYPE, "RPCC2G_BACKFLOWBUYOP");
	CProtocol::RegistProtoName(RPCC2G_TRANSFORMOP_TYPE, "RPCC2G_TRANSFORMOP");
	CProtocol::RegistProtoName(RPCC2G_UPGRADEEQUIP_TYPE, "RPCC2G_UPGRADEEQUIP");
	CProtocol::RegistProtoName(RPCC2G_GETFASHIONSYNTHESISINFO_TYPE, "RPCC2G_GETFASHIONSYNTHESISINFO");
	CProtocol::RegistProtoName(RPCC2G_FASHIONIBSHOPBUY_TYPE, "RPCC2G_FASHIONIBSHOPBUY");
	CProtocol::RegistProtoName(RPCM2G_CREATEDRAGONGUILDCOST_TYPE, "RPCM2G_CREATEDRAGONGUILDCOST");
	CProtocol::RegistProtoName(RPCM2G_GETROLEPOWER_TYPE, "RPCM2G_GETROLEPOWER");
	CProtocol::RegistProtoName(RPCM2G_GETDRAGONGUILDRECORDM2G_TYPE, "RPCM2G_GETDRAGONGUILDRECORDM2G");
	CProtocol::RegistProtoName(RPCG2M_GETDRAGONGUILDINFOG2M_TYPE, "RPCG2M_GETDRAGONGUILDINFOG2M");
	CProtocol::RegistProtoName(RPCC2G_TAKEDRAGONGUILDCHEST_TYPE, "RPCC2G_TAKEDRAGONGUILDCHEST");
	CProtocol::RegistProtoName(RPCM2G_MODIFYDRAGONGUILDNAMECOST_TYPE, "RPCM2G_MODIFYDRAGONGUILDNAMECOST");
	CProtocol::RegistProtoName(RPCC2G_GETDRAGONGUILDTASKCHEST_TYPE, "RPCC2G_GETDRAGONGUILDTASKCHEST");
	CProtocol::RegistProtoName(RPCG2M_GETDRAGONGUILDTASKCHESTG2M_TYPE, "RPCG2M_GETDRAGONGUILDTASKCHESTG2M");
	CProtocol::RegistProtoName(RPCM2G_GETDRAGONGUILDRECIEVEINFO_TYPE, "RPCM2G_GETDRAGONGUILDRECIEVEINFO");
	CProtocol::RegistProtoName(RPCM2G_BUYDRAGONGUILDITEMM2G_TYPE, "RPCM2G_BUYDRAGONGUILDITEMM2G");
	CProtocol::RegistProtoName(RPCG2M_CHECKDRAGONGUILDREDPOINTG2M_TYPE, "RPCG2M_CHECKDRAGONGUILDREDPOINTG2M");
	CProtocol::RegistProtoName(RPCC2G_BATTLEFIELDAWARDNUMREQ_TYPE, "RPCC2G_BATTLEFIELDAWARDNUMREQ");
	CProtocol::RegistProtoName(RPCC2G_BATTLEFIELDRANKREQ_TYPE, "RPCC2G_BATTLEFIELDRANKREQ");
	CProtocol::RegistProtoName(RPCC2G_GETWEEKLYTASKINFO_TYPE, "RPCC2G_GETWEEKLYTASKINFO");
	CProtocol::RegistProtoName(RPCC2G_GETWEEKLYTASKREWARD_TYPE, "RPCC2G_GETWEEKLYTASKREWARD");
	CProtocol::RegistProtoName(RPCC2G_REFRESHWEEKLYTASK_TYPE, "RPCC2G_REFRESHWEEKLYTASK");
	CProtocol::RegistProtoName(RPCC2G_BATTLEFIELDROLEAGAINSTREQ_TYPE, "RPCC2G_BATTLEFIELDROLEAGAINSTREQ");
	CProtocol::RegistProtoName(RPCC2G_GETCOMPETEDRAGONINFO_TYPE, "RPCC2G_GETCOMPETEDRAGONINFO");
	CProtocol::RegistProtoName(RPCC2G_SETPRESHOW_TYPE, "RPCC2G_SETPRESHOW");
	CProtocol::RegistProtoName(RPCC2G_FUSEEQUIP_TYPE, "RPCC2G_FUSEEQUIP");
	CProtocol::RegistProtoName(RPCC2G_TURNOVERWEEKLYTASKITEM_TYPE, "RPCC2G_TURNOVERWEEKLYTASKITEM");
	CProtocol::RegistProtoName(RPCG2M_TASKDONATEG2M_TYPE, "RPCG2M_TASKDONATEG2M");
	CProtocol::RegistProtoName(RPCC2G_GETLUCKYACTIVITYINFO_TYPE, "RPCC2G_GETLUCKYACTIVITYINFO");
	CProtocol::RegistProtoName(RPCC2G_BUYDRAW_TYPE, "RPCC2G_BUYDRAW");
	CProtocol::RegistProtoName(RPCC2G_LOTTERYDRAW_TYPE, "RPCC2G_LOTTERYDRAW");
	CProtocol::RegistProtoName(RPCM2G_NPCFLREQM2G_TYPE, "RPCM2G_NPCFLREQM2G");
	CProtocol::RegistProtoName(RPCG2K_BMRANKLISTREQ_TYPE, "RPCG2K_BMRANKLISTREQ");
	CProtocol::RegistProtoName(RPCM2G_DAILYTASKREFRESHM2G_TYPE, "RPCM2G_DAILYTASKREFRESHM2G");
	CProtocol::RegistProtoName(RPCG2M_DAILYTASKREFRESHG2M_TYPE, "RPCG2M_DAILYTASKREFRESHG2M");
	CProtocol::RegistProtoName(RPCC2G_SURVIVEREQC2G_TYPE, "RPCC2G_SURVIVEREQC2G");
	CProtocol::RegistProtoName(RPCG2M_SURVIVESYNPOINTG2M_TYPE, "RPCG2M_SURVIVESYNPOINTG2M");
	CProtocol::RegistProtoName(RPCC2G_DOODADITEMALLSKILLREQ_TYPE, "RPCC2G_DOODADITEMALLSKILLREQ");
	CProtocol::RegistProtoName(RPCC2G_PEERBOX_TYPE, "RPCC2G_PEERBOX");
	CProtocol::RegistProtoName(RPCC2G_QUERYBOXS_TYPE, "RPCC2G_QUERYBOXS");
	CProtocol::RegistProtoName(RPCC2G_ACTIVATEPRESHOW_TYPE, "RPCC2G_ACTIVATEPRESHOW");
	CProtocol::RegistProtoName(RPCC2G_GUILDPARTYSUMMONSPIRIT_TYPE, "RPCC2G_GUILDPARTYSUMMONSPIRIT");
	CProtocol::RegistProtoName(RPCG2M_GETDAILYTASKG2M_TYPE, "RPCG2M_GETDAILYTASKG2M");
	CProtocol::RegistProtoName(RPCC2G_CHOOSESPECIALEFFECTS_TYPE, "RPCC2G_CHOOSESPECIALEFFECTS");
	CProtocol::RegistProtoName(RPCG2K_TEAMGOTONEXTSCENEREQ_TYPE, "RPCG2K_TEAMGOTONEXTSCENEREQ");
	CProtocol::RegistProtoName(RPCC2G_ENTERLEISURESCENE_TYPE, "RPCC2G_ENTERLEISURESCENE");
	CProtocol::RegistProtoName(RPCC2G_BACKFLOWACTIVITYOPERATION_TYPE, "RPCC2G_BACKFLOWACTIVITYOPERATION");
	CProtocol::RegistProtoName(RPCC2G_GETNEWZONEBENEFIT_TYPE, "RPCC2G_GETNEWZONEBENEFIT");
	CProtocol::RegistProtoName(RPCG2M_GETNEWZONEBENEFITG2M_TYPE, "RPCG2M_GETNEWZONEBENEFITG2M");
	CProtocol::RegistProtoName(RPCC2G_SELECTCHARGEBACKROLE_TYPE, "RPCC2G_SELECTCHARGEBACKROLE");

}