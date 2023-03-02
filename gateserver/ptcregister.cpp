#include "pch.h"
#include "forward.h"
#include "config.h"
#include "ptcregister.h"
#include "crpc.h"
#include "protocol.h"

// generate by ProtoGen at date: 2017/11/8 14:10:06

#include "login/ptcc2t_keepalivepingack.h"
#include "common/ptcg2t_opengate.h"
#include "common/ptcg2t_changegsline.h"
#include "login/ptcn2t_kickaccount.h"
#include "chat/ptcm2t_chatforward.h"
#include "common/ptcm2t_versionnotify.h"
#include "broadcast/ptcm2t_broadcastm2t.h"
#include "login/ptcn2t_notifygtsessionlogin.h"
#include "broadcast/ptcg2t_broadcastg2t.h"
#include "common/ptcm2t_notifyrouterinfo2gate.h"
#include "session/ptcn2t_sessionchange2gt.h"
#include "common/ptct2g_registserverntf.h"
#include "login/ptct2c_loginchallenge.h"
#include "login/ptct2c_keepalivepingreq.h"
#include "common/ptct2c_errornotify.h"
#include "chat/ptct2c_chatnotify.h"
#include "login/ptct2g_reconnectntf.h"
#include "login/ptct2n_logoutntf.h"
#include "login/ptcg2c_notifyaccountdata.h"
#include "scene/ptcc2g_enterscenereq.h"
#include "scene/ptcg2c_enterscenentf.h"
#include "scene/ptcc2g_leavescenereq.h"
#include "scene/ptcg2c_leavescenentf.h"
#include "action/ptcc2g_moveoperationreq.h"
#include "scene/ptcg2c_unitappear.h"
#include "scene/ptcg2c_unitdisappear.h"
#include "skill/ptcc2g_castskill.h"
#include "skill/ptcg2c_skillresult.h"
#include "scene/ptcg2c_deathnotify.h"
#include "scene/ptcg2c_attributechangenotify.h"
#include "role/ptcg2c_levelchangentf.h"
#include "common/ptcg2c_itemchangedntf.h"
#include "common/ptcc2g_updatetutorial.h"
#include "role/ptcg2c_opensystemntf.h"
#include "battle/ptcg2c_battleresultntf.h"
#include "common/ptcg2c_completeachivement.h"
#include "skill/ptcg2c_skillchangedntf.h"
#include "common/ptcg2c_gserrornotify.h"
#include "scene/ptcg2c_changefashionnotify.h"
#include "checkin/ptcg2c_checkininfonotify.h"
#include "activity/ptcg2c_activityrolenotify.h"
#include "pay/ptcg2c_updatepayinfo.h"
#include "reward/ptcg2c_rewardchangedntf.h"
#include "team/ptcg2c_leaveteam.h"
#include "battle/ptcg2c_revivenotify.h"
#include "pay/ptcg2c_updatebuygoldandfatigueinfo.h"
#include "battle/ptcc2g_roledeathinsoloscene.h"
#include "battle/ptcg2c_revivecountdown.h"
#include "role/ptcg2c_fatiguerecovertimenotify.h"
#include "scene/ptcc2g_enterscenecooldownquery.h"
#include "scene/ptcg2c_enterscenecooldownnotify.h"
#include "fashoin/ptcg2c_fashoinchangedntf.h"
#include "common/ptcc2g_operaterecordntf.h"
#include "guild/ptcg2c_guildnotifymemberchanged.h"
#include "guild/ptcg2c_guildsettingchangednotify.h"
#include "guild/ptcg2c_guildcheckinboxntf.h"
#include "guild/ptcg2c_newguildbonusntf.h"
#include "guild/ptcg2c_guildbestcardsntf.h"
#include "scene/ptcg2c_executelevelscriptntf.h"
#include "reward/ptcg2c_onlinerewardntf.h"
#include "reward/ptcg2c_nextdayrewardntf.h"
#include "reward/ptcc2g_onlinerewardreport.h"
#include "reward/ptcc2g_nextdayrewardreport.h"
#include "common/ptcg2c_hintnotify.h"
#include "scene/ptcg2c_positiondebug.h"
#include "battle/ptcg2c_enemydorpdoodadntf.h"
#include "task/ptcg2c_takerandomtask.h"
#include "battle/ptcg2c_buffnotify.h"
#include "battle/ptcg2c_teamselectnotify.h"
#include "team/ptcg2c_teaminvitenotify.h"
#include "battle/ptcg2c_fetchenemydoodadntf.h"
#include "common/ptcc2g_delaynotify.h"
#include "pk/ptcg2c_pkpreparentf.h"
#include "pk/ptcg2c_pktimeoutntf.h"
#include "battle/ptcg2c_guildgoblinkillntf.h"
#include "skill/ptcc2g_skillresultreq.h"
#include "skill/ptcc2g_skillbulletfirereport.h"
#include "skill/ptcc2g_skillbulletresultreq.h"
#include "scene/ptcg2c_scenefinishstatechanged.h"
#include "pk/ptcg2c_pkroleinfontf.h"
#include "common/ptcg2c_itemcircledrawresult.h"
#include "scene/ptcc2g_addlevelscriptextstring.h"
#include "scene/ptcc2g_scenedamagerankreport.h"
#include "scene/ptcg2c_scenedamagerankntf.h"
#include "battle/ptcg2c_sceneleftdoodad.h"
#include "item/ptcc2g_changesupplementreport.h"
#include "item/ptcg2c_changesupplementntf.h"
#include "common/ptcc2g_qteoperation.h"
#include "action/ptcg2c_correctposition.h"
#include "common/ptcg2c_teleportnotice.h"
#include "scene/ptcg2c_allymatchroleidnotify.h"
#include "team/ptcg2c_teamfulldatantf.h"
#include "pet/ptcg2c_petchangenotfiy.h"
#include "findback/ptcg2c_expfindbackntf.h"
#include "battle/ptcc2g_autofightntf.h"
#include "tower/ptcg2c_towersceneinfontf.h"
#include "designation/ptcg2c_coverdesignationntf.h"
#include "achievement/ptcg2c_reachachieventf.h"
#include "designation/ptcg2c_reachdesignationntf.h"
#include "scene/ptcc2g_syncscenefinish.h"
#include "levelseal/ptcg2c_levelsealntf.h"
#include "common/ptcg2c_reconnectsyncnotify.h"
#include "common/ptcg2c_outlookchangentf.h"
#include "reward/ptcg2c_fivedayrewardntf.h"
#include "action/ptcg2c_syncstepnotify.h"
#include "tsssdk/ptcc2g_tsssdksendanti2server.h"
#include "tsssdk/ptcg2c_tsssdkantidatantf.h"
#include "qa/ptcm2c_answerackntf.h"
#include "qa/ptcm2c_pushquestionntf.h"
#include "qa/ptcg2c_qanotify.h"
#include "pvp/ptcg2c_pvpbattlebeginntf.h"
#include "pvp/ptcg2c_pvpbattleendntf.h"
#include "pvp/ptcg2c_pvpbattlekill.h"
#include "social/ptcg2c_flowerrankrewardntf.h"
#include "qa/ptcm2c_qaoverntf.h"
#include "qa/ptcm2c_qaroomrankntf.h"
#include "team/ptcg2c_fastmbconfirmntf.h"
#include "team/ptcc2g_fmbrefuse.h"
#include "team/ptcg2c_fastmbdismissntf.h"
#include "role/ptcg2c_selectrolentf.h"
#include "social/ptcg2c_receiveflowerntf.h"
#include "social/ptcg2c_flowerrainntf.h"
#include "team/ptcg2c_startbattlefailedntf.h"
#include "qa/ptcm2c_qaenterroomntf.h"
#include "battlewatch/ptcg2c_watchbattleinfontf.h"
#include "guild/ptcg2c_gmfjoinbattlereq.h"
#include "guild/ptcg2c_gmfonebattleendntf.h"
#include "guild/ptcg2c_gmfbasedatantf.h"
#include "chat/ptcg2c_worldchannellefttimesntf.h"
#include "guild/ptcg2c_noticeguildbossend.h"
#include "guild/ptcg2c_updateguildarenastate.h"
#include "guild/ptcg2c_guildbosstimeout.h"
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
#include "battle/ptcg2c_notifycliententerfight.h"
#include "guild/ptcg2c_synguildarenaroleonline.h"
#include "battle/ptcg2c_clientonlybuffnotify.h"
#include "chat/ptcm2c_mcchatofflinenotify.h"
#include "friend/ptcm2c_friendopntfnew.h"
#include "chat/ptcc2m_loadofflinechatntf.h"
#include "friend/ptcc2m_friendqueryreportnew.h"
#include "friend/ptcm2c_frienddegreeupntfnew.h"
#include "friend/ptcc2m_blacklistreportnew.h"
#include "friend/ptcm2c_blacklistntfnew.h"
#include "guild/ptcg2c_synguildarenadisplace.h"
#include "battle/ptcg2c_ontransferwall.h"
#include "battlewatch/ptcg2c_notifywatchiconnum2client.h"
#include "guild/ptcg2c_gmfwaitotherload.h"
#include "guild/ptcg2c_gmfwaitfightbegin.h"
#include "common/ptcc2m_rolestatereportnew.h"
#include "common/ptcm2c_rolestatentfnew.h"
#include "common/ptcm2c_hintnotifyms.h"
#include "worldboss/ptcg2c_worldbossattrntf.h"
#include "chat/ptcc2g_joinroom.h"
#include "chat/ptcg2c_joinroomreply.h"
#include "chat/ptcc2g_quitroom.h"
#include "chat/ptcg2c_updatevoiproommemberntf.h"
#include "pay/ptcg2c_payallinfontf.h"
#include "reward/ptcg2c_loginreward2cntf.h"
#include "social/ptcm2c_msreceiveflowerntf.h"
#include "social/ptcm2c_msflowerrainntf.h"
#include "team/ptcm2c_teamchangem2cntf.h"
#include "team/ptcm2c_leaveteamm2cntf.h"
#include "team/ptcm2c_teamfulldatam2cntf.h"
#include "team/ptcm2c_teaminvitem2cnotify.h"
#include "common/ptcm2c_mserrornotify.h"
#include "team/ptcm2c_inviterefusem2cntf.h"
#include "team/ptcc2m_teaminviteackc2m.h"
#include "team/ptcm2c_rolematchstatem2cnotify.h"
#include "team/ptcm2c_fastmbconfirmm2cntf.h"
#include "team/ptcm2c_fastmbdismissm2cntf.h"
#include "team/ptcc2m_fmbrefusec2m.h"
#include "battle/ptcg2c_aidebuginfo.h"
#include "battle/ptcg2c_startrollntf.h"
#include "queuing/ptcc2n_checkqueuingreq.h"
#include "queuing/ptcn2c_checkqueuingntf.h"
#include "pet/ptcg2c_noticehungrydown.h"
#include "firstpass/ptcg2c_firstpassrewardntf.h"
#include "title/ptcg2c_titlechangenotify.h"
#include "qa/ptcm2c_qaidnamentf.h"
#include "pk/ptcm2c_pktimeoutm2cntf.h"
#include "guild/ptcm2c_loginguildinfo.h"
#include "guild/ptcm2c_sendguildskillinfo.h"
#include "common/ptcm2c_mseventnotify.h"
#include "findback/ptcg2c_itemfindbackntf.h"
#include "sprite/ptcg2c_spritechangedntf.h"
#include "battle/ptcg2c_fightgroupchangentf.h"
#include "stage/ptcg2c_bossrushonefinishntf.h"
#include "atlas/ptcg2c_synatlasattr.h"
#include "superrisk/ptcg2c_playdicentf.h"
#include "superrisk/ptcc2g_playdiceover.h"
#include "ibshop/ptcg2c_ibshopicon.h"
#include "ibshop/ptcc2g_removeibshopicon.h"
#include "enhance/ptcg2c_notifyenhancesuit.h"
#include "guild/ptcm2c_synguildarenafightunitnew.h"
#include "guild/ptcm2c_synguildarenabattleinfonew.h"
#include "guild/ptcm2c_noticeguildarenabeginnew.h"
#include "guild/ptcm2c_synguildarenaroleonlinenew.h"
#include "guild/ptcm2c_synguildarenadisplacenew.h"
#include "superrisk/ptcg2c_riskbuyntf.h"
#include "chat/ptcc2g_setvoipmemberstate.h"
#include "superrisk/ptcg2c_riskrecoverdicentf.h"
#include "guild/ptcm2c_notifyguildskilldata.h"
#include "sprite/ptcc2g_openspriteeggntf.h"
#include "guild/ptcm2c_guildsetingnotify.h"
#include "guild/ptcm2c_guildbestcardsntfms.h"
#include "tower/ptcg2c_towerfirstpassrewardntf.h"
#include "stage/ptcm2c_derankchangntf.h"
#include "guild/ptcm2c_gmfjoinbattlem2creq.h"
#include "guild/ptcc2m_gmfjoinbattlec2mres.h"
#include "activity/ptcg2c_halliconsntf.h"
#include "task/ptcg2c_updatetaskstatus.h"
#include "skycity/ptcm2c_skycityfinalres.h"
#include "skycity/ptcg2c_skycityestimateres.h"
#include "skycity/ptcg2c_skycitytimeres.h"
#include "common/ptcg2c_notifyidipmessagegs.h"
#include "common/ptcm2c_notifyidipmessagems.h"
#include "common/ptcg2c_idippunishinfontf.h"
#include "common/ptcm2c_idippunishinfomsntf.h"
#include "chat/ptcm2c_idipclearchatntf.h"
#include "skycity/ptcg2c_skycityteamres.h"
#include "guild/ptcc2m_guildcardrankreq.h"
#include "guild/ptcm2c_guildcardrankntf.h"
#include "guild/ptcm2c_noticeguildladderstart.h"
#include "scene/ptcg2c_gmfkicknty.h"
#include "guild/ptcc2m_guildcardmatchreq.h"
#include "guild/ptcm2c_guildcardmatchntf.h"
#include "chat/ptcc2m_openprivatechatntf.h"
#include "scene/ptcg2c_skycitybattledatantf.h"
#include "qa/ptcc2m_commitanswerntf.h"
#include "qa/ptcc2m_giveupqaquestionntf.h"
#include "scene/ptcg2c_gmfbattlestatentf.h"
#include "spactivity/ptcg2c_spactivitychangentf.h"
#include "spactivity/ptcg2c_spactivityoffsetdayntf.h"
#include "garden/ptcm2c_spritestatechangentf.h"
#include "team/ptcm2c_invunfstatem2cntf.h"
#include "battle/ptcg2c_dpsnotify.h"
#include "guild/ptcm2c_noticeguildarenanexttime.h"
#include "skill/ptcg2c_skillcoolntf.h"
#include "garden/ptcm2c_gardenbanquetntf.h"
#include "garden/ptcg2c_gardenplanteventnotice.h"
#include "garden/ptcg2c_gardenbanquetnotice.h"
#include "platfriend/ptcc2m_syncplatfriend2ms.h"
#include "guild/ptcm2c_noticeguildwagereward.h"
#include "guild/ptcg2c_gmfguildcombatntf.h"
#include "guild/ptcg2c_gpronebattleendntf.h"
#include "guild/ptcg2c_gprallfightendntf.h"
#include "reswar/ptcg2c_reswarbattledatantf.h"
#include "reswar/ptcg2c_reswarteamresone.h"
#include "guild/ptcm2c_sendguildbonusntf.h"
#include "battle/ptcc2g_battlelogreport.h"
#include "reswar/ptcm2c_reswarguildbriefntf.h"
#include "guildcamp/ptcc2g_clickguildcamp.h"
#include "pay/ptcg2c_paymemberprivilegentf.h"
#include "guild/ptcm2c_synguilintegralstate.h"
#include "reswar/ptcm2c_reswarstatentf.h"
#include "guild/ptcm2c_synguildintegralstate.h"
#include "battle/ptcg2c_killenemyscorentf.h"
#include "common/ptcg2c_serveropendayntf.h"
#include "reswar/ptcm2c_reswarranksimpleinfontf.h"
#include "reswar/ptcm2c_guildbuffsimpleinfontf.h"
#include "reswar/ptcc2m_queryreswarrequet.h"
#include "reswar/ptcm2c_guildbuffsimpleitemntf.h"
#include "garden/ptcc2m_gardenfishstop.h"
#include "reswar/ptcm2c_halliconmntf.h"
#include "guild/ptcg2c_synguildinheritexp.h"
#include "guild/ptcc2g_syndoingguildinherit.h"
#include "guild/ptcg2c_noticedoingguildinherit.h"
#include "guild/ptcg2c_synguildinheritnuminfo.h"
#include "reswar/ptcm2c_reswarminedatantf.h"
#include "skill/ptcg2c_cdcall.h"
#include "chat/ptcc2m_leavelargeroom.h"
#include "chat/ptcm2c_largeroomrolentf.h"
#include "chat/ptcm2c_largeroomloginparamntf.h"
#include "reswar/ptcm2c_reswartimentf.h"
#include "action/ptcg2c_syncmovenotify.h"
#include "chat/ptcc2m_handlemicphone.h"
#include "role/ptcg2c_changenamecountntf.h"
#include "reswar/ptcm2c_guildbuffcdparamntf.h"
#include "common/ptcg2c_antiaddictionremindntf.h"
#include "horserace/ptcg2c_horseawardallntf.h"
#include "horserank/ptcg2c_horserankntf.h"
#include "horserace/ptcg2c_horsewaittimentf.h"
#include "horserace/ptcg2c_horseanimationntf.h"
#include "horse/ptcg2c_horsefinalntf.h"
#include "role/ptcc2g_picurlntf.h"
#include "mentorship/ptcm2c_notifymentorapply.h"
#include "pokertournament/ptcc2m_pokertournamentreq.h"
#include "partner/ptcm2c_updatepartnertoclient.h"
#include "partner/ptcm2c_makepartnerresultntf.h"
#include "invfight/ptcm2c_invfightnotify.h"
#include "horse/ptcg2c_horsecountdowntimentf.h"
#include "battle/ptcg2c_doodaditemaddntf.h"
#include "guilfcastle/ptcg2c_gcfzhanlingnotify.h"
#include "invfight/ptcg2c_invfightbefenterscenentf.h"
#include "herobattle/ptcg2c_herobattledatantf.h"
#include "scene/ptcg2c_sceneprepareinfontf.h"
#include "herobattle/ptcg2c_herobattleteammsgntf.h"
#include "herobattle/ptcg2c_herobattlesyncntf.h"
#include "herobattle/ptcg2c_herobattleincirclentf.h"
#include "herobattle/ptcg2c_herobattleovertime.h"
#include "pay/ptcc2m_payparameterinfontf.h"
#include "pay/ptcm2c_payparameterinfoinvalidntf.h"
#include "herobattle/ptcg2c_herobattleteamrolentf.h"
#include "guildcastle/ptcg2c_gcfsyng2cntf.h"
#include "guild/ptcm2c_noticeguildterrwar.h"
#include "guild/ptcm2c_noticeguildterrall.h"
#include "common/ptcc2n_updatestartuptypentf.h"
#include "battle/ptcg2c_doodaditemusentf.h"
#include "guildbonus/ptcm2c_guildbonusgetall.h"
#include "battle/ptcg2c_specialstatentf.h"
#include "reswar/ptcm2c_reswarenemytimentf.h"
#include "herobattle/ptcg2c_closechooseherontf.h"
#include "battle/ptcg2c_levelscriptstatentf.h"
#include "scene/ptcg2c_scenestatentf.h"
#include "role/ptcc2g_queryrolestatereq.h"
#include "role/ptcg2c_queryrolestateack.h"
#include "ibshop/ptcg2c_ibshophasbuyntf.h"
#include "league/ptcm2c_updateleaguebattleseasoninfo.h"
#include "role/ptcg2c_skillinitcoolntf.h"
#include "league/ptcm2c_notifyleagueteamcreate.h"
#include "league/ptcm2c_notifyleagueteamdissolve.h"
#include "herobattle/ptcg2c_herobattlecanusehero.h"
#include "league/ptcm2c_updateleagueteamstate.h"
#include "league/ptcm2c_leaguebattlestartmatchntf.h"
#include "league/ptcm2c_leaguebattlestopmatchntf.h"
#include "leaguebattle/ptcg2c_leaguebattleloadinfontf.h"
#include "leaguebattle/ptcg2c_leaguebattlebasedatantf.h"
#include "leaguebattle/ptcg2c_leaguebattleoneresultnft.h"
#include "leaguebattle/ptcg2c_leaguebattleresultntf.h"
#include "pandora/ptcg2c_pushpraisentf.h"
#include "herobattle/ptcg2c_herobattletipsntf.h"
#include "leaguebattle/ptcg2c_leaguebattlestatentf.h"
#include "league/ptcm2c_leaguebattlematchtimeoutntf.h"
#include "reddot/ptcc2g_cancelreddotreq.h"
#include "pay/ptcm2c_midasexceptionntf.h"
#include "league/ptcm2c_updateleagueeleroomstatentf.h"
#include "league/ptcc2m_closeleagueelentf.h"
#include "chat/ptcm2c_kickfmauchor.h"
#include "auction/ptcm2c_guildauctitemtimefresh.h"
#include "pokertournament/ptcm2c_pokertournamentendrefund.h"
#include "guild/ptcm2c_noticeguildterrend.h"
#include "guild/ptcm2c_noticeguildterrbattlewin.h"
#include "guild/ptcm2c_noticeguildterrbigicon.h"
#include "common/ptcc2g_closehintntf.h"
#include "common/ptca2c_audioaidntf.h"
#include "common/ptcg2c_countdownntf.h"
#include "pay/ptcg2c_allgiftibitemntf.h"
#include "pay/ptcg2c_giftibbackinfontf.h"
#include "lottery/ptcg2c_sharerandomgiftntf.h"
#include "guild/ptcm2c_modifyguildnamentf.h"
#include "worldboss/ptcg2c_worldbossguildaddattrsyncclientntf.h"
#include "militaryrank/ptcg2c_militaryrankntf.h"
#include "loginactivity/ptcg2c_loginactivitystatusntf.h"
#include "loginactivity/ptcc2g_loginactivityawardreq.h"
#include "pay/ptcc2m_paybuygoodsfailntf.h"
#include "pay/ptcc2m_ibgifttipsreq.h"
#include "arenastar/ptcm2c_arenastardatantf.h"
#include "skyteam/ptcm2c_notifyskyteamcreate.h"
#include "skycraft/ptcm2c_skycraftmatchntf.h"
#include "skycraft/ptcm2c_skycrafteliroomntf.h"
#include "skycraft/ptcc2m_closeskycraftelintf.h"
#include "fashion/ptcg2c_displayadditem.h"
#include "fashion/ptcg2c_updatedisplayitems.h"
#include "guild/ptcg2c_guildcamppartynotify.h"
#include "pay/ptcm2c_ibgifticonntf.h"
#include "pay/ptcm2c_giftibsucessntf.h"
#include "absparty/ptcg2c_abspartyntf.h"
#include "guild/ptcg2c_guildcamppartytradenotify.h"
#include "team/ptcm2c_teamrequestplatfreind2client.h"
#include "custombattle/ptcg2c_custombattleloadingntf.h"
#include "custombattle/ptcm2c_custombattlegmnotify.h"
#include "team/ptcm2c_tarjabriefntf.h"
#include "platformshareresult/ptcg2c_platformshareawardntf.h"
#include "platformshareresult/ptcc2g_notifyplatshareresult.h"
#include "weekend4v4/ptcg2c_weekend4v4roledatantf.h"
#include "mobabattle/ptcg2c_herokillnotify.h"
#include "mobabattle/ptcg2c_mobabattleteamrolentf.h"
#include "mobabattle/ptcg2c_mobarolechangentf.h"
#include "mobabattle/ptcg2c_mobabattleteammsgntf.h"
#include "skill/ptcg2c_transskillnotfiy.h"
#include "battle/ptcg2c_entitytargetchangentf.h"
#include "mobabattle/ptcg2c_mobasignalbroadcast.h"
#include "pet/ptcg2c_petinvitentf.h"
#include "chat/ptcg2c_chatnotifyg2c.h"
#include "mobabattle/ptcg2c_mobaminimapntf.h"
#include "mobabattle/ptcg2c_mobaaddexpntf.h"
#include "bigmelee/ptcg2c_bmreadytimentf.h"
#include "bigmelee/ptcg2c_bmfighttimentf.h"
#include "bigmelee/ptcg2c_bmrolescenesyncntf.h"
#include "bigmelee/ptcg2c_bigmeleereliventf.h"
#include "scene/ptcg2c_mobahintntf.h"
#include "tajie/ptcg2c_tajiehelpnotify.h"
#include "common/ptcg2c_notifystartuptypetoclient.h"
#include "herobattle/ptcg2c_herobattleancientpower.h"
#include "groupchat/ptcm2c_groupchatmanager.h"
#include "groupchat/ptcm2c_groupchatquit.h"
#include "groupchat/ptcm2c_groupchatdismiss.h"
#include "wedding/ptcm2c_weddinginvitentf.h"
#include "marriage/ptcm2c_notifymarriageapply.h"
#include "battle/ptcc2g_battlestatisticsreport.h"
#include "battle/ptcg2c_battlestatisticsntf.h"
#include "wedding/ptcg2c_weddingloadinfontf.h"
#include "wedding/ptcg2c_weddingstatentf.h"
#include "wedding/ptcg2c_weddingeventntf.h"
#include "horse/ptcg2c_horsefailtipsntf.h"
#include "marriage/ptcm2c_notifymarriagedivorceapply.h"
#include "skill/ptcg2c_warningrandomntf.h"
#include "groupchat/ptcm2c_groupchatissuecount.h"
#include "groupchat/ptcm2c_groupchatapply.h"
#include "misc/ptcc2g_sharereport.h"
#include "spactivity/ptcg2c_themeactivitychangentf.h"
#include "wedding/ptcg2c_weddingcarntf.h"
#include "spactivity/ptcg2c_backflowopenntf.h"
#include "team/ptcm2c_startbattlefailedm2cntf.h"
#include "transform/ptcg2c_transnotify.h"
#include "invfight/ptcc2g_invfightagainreqc2g.h"
#include "dragonguild/ptcm2c_logindragonguildinfo.h"
#include "dragonguild/ptcm2c_dragonguildsettingchanged.h"
#include "dragonguild/ptcm2c_dragonguildnamentf.h"
#include "battlefield/ptcc2m_gobackreadyscenentf.h"
#include "goalawards/ptcm2c_goalawardsredpoint.h"
#include "battlefield/ptcg2c_battlefieldreadyinfontf.h"
#include "battlefield/ptcg2c_battlefieldreliventf.h"
#include "battlefield/ptcg2c_battlefieldroleagainstntf.h"
#include "battlefield/ptcg2c_bffighttimentf.h"
#include "pay/ptcg2c_payscorentf.h"
#include "npcfeeling/ptcg2c_npcflntf.h"
#include "task/ptcm2c_dailytaskeventntf.h"
#include "common/ptcc2g_updatefreeflowhintinfo.h"
#include "battle/ptcg2c_doodaditemskillsntf.h"
#include "skycity/ptcc2m_mulactivityiconsysreq.h"
#include "notice/ptcg2c_wordnotify.h"
#include "bigmelee/ptcg2c_bigmeleepointntf.h"
#include "bigmelee/ptcg2c_bigmeleepointoutlookntf.h"
#include "task/ptcm2c_taskrefreshntf.h"
#include "kickaccount/ptcg2c_kickaccountjkydntf.h"
#include "guildcastle/ptcm2c_guildcastfeatsntf.h"
#include "common/ptcg2c_worldlevelntf2client.h"
#include "stage/ptcg2c_updatestageinfontf.h"
#include "login/rpcc2t_reconnect.h"
#include "login/rpcc2t_clientloginrequest.h"
#include "common/rpct2m_registergt2ms.h"
#include "common/rpcg2t_registergs2gt.h"
#include "common/rpct2g_registergt2gs.h"
#include "login/rpct2n_loginrequestnew.h"
#include "login/rpct2g_reconnectgs.h"
#include "common/rpct2r_registergt2router.h"
#include "common/rpct2n_registergt2ns.h"
#include "chat/rpcc2t_uploadaudiotogate.h"
#include "chat/rpct2a_uploadaudiotoaudio.h"
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
#include "mail/rpcc2m_fetchmail.h"
#include "mail/rpcc2m_mailop.h"
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
#include "common/rpcc2a_getaudiolistreq.h"
#include "common/rpcc2a_uploadaudioreq.h"
#include "equip/rpcc2g_enhancetranster.h"
#include "pvp/rpcc2g_pvpnowalldata.h"
#include "qa/rpcc2m_getqadatareq.h"
#include "dragon/rpcc2g_getdragontopinfo.h"
#include "team/rpcc2g_teaminvitelistreq.h"
#include "role/rpcc2n_createrolenew.h"
#include "role/rpcc2n_selectrolenew.h"
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
#include "role/rpcc2n_returntoselectrole.h"
#include "scene/rpcc2g_doenterscene.h"
#include "jade/rpcc2g_takeoffalljade.h"
#include "activity/rpcc2g_sweeptower.h"
#include "activity/rpcc2g_resettower.h"
#include "goddesstrial/rpcc2g_getgoddesstrialrewards.h"
#include "chat/rpcc2m_chat.h"
#include "friend/rpcc2m_doaddfriendnew.h"
#include "friend/rpcc2m_removefriendnew.h"
#include "friend/rpcc2m_addblacklistnew.h"
#include "friend/rpcc2m_removeblacklistnew.h"
#include "friend/rpcc2m_friendgiftopnew.h"
#include "friend/rpcc2m_randomfriendwaitlistnew.h"
#include "common/rpcc2g_querypowerpoint.h"
#include "worldboss/rpcc2m_getworldbossstatenew.h"
#include "worldboss/rpcc2m_getworldbosstimeleft.h"
#include "pay/rpcc2g_getpayallinfo.h"
#include "pay/rpcc2g_paycardaward.h"
#include "pay/rpcc2g_payclick.h"
#include "activity/rpcc2g_getactivityinfo.h"
#include "social/rpcc2m_showflowerpagenew.h"
#include "social/rpcc2m_msgetflowerrewardlist.h"
#include "team/rpcc2m_fetchteamlistc2m.h"
#include "team/rpcc2m_invhistoryc2mreq.h"
#include "team/rpcc2m_teamrequestc2m.h"
#include "pet/rpcc2g_synpetinfo.h"
#include "battle/rpcc2g_chooserollreq.h"
#include "pay/rpcc2g_payfirstaward.h"
#include "pay/rpcc2g_growthfundaward.h"
#include "pay/rpcc2g_buyviplevelgift.h"
#include "title/rpcc2g_titlelevelup.h"
#include "firstpass/rpcc2g_firstpassinforeq.h"
#include "firstpass/rpcc2g_getfirstpassreward.h"
#include "firstpass/rpcc2g_commendfirstpass.h"
#include "firstpass/rpcc2g_firstpassgettoproleinfo.h"
#include "ranklist/rpcc2m_clientqueryranklistntf.h"
#include "friend/rpcc2m_addfriendnew.h"
#include "pk/rpcc2m_pkreqc2m.h"
#include "chat/rpcc2m_clearprivatechatlist.h"
#include "guild/rpcc2m_askguildbriefinfo.h"
#include "guild/rpcc2m_createorenterguild.h"
#include "guild/rpcc2m_leavefromguild.h"
#include "auction/rpcc2m_auctionallreq.h"
#include "role/rpcc2g_changeoutlookop.h"
#include "findback/rpcc2g_itemfindbackinfo.h"
#include "findback/rpcc2g_itemfindback.h"
#include "guild/rpcc2m_reqguildlist.h"
#include "atlas/rpcc2g_activatatlas.h"
#include "atlas/rpcc2g_autobreakatlas.h"
#include "atlas/rpcc2g_breakatlas.h"
#include "ibshop/rpcc2g_buyibitem.h"
#include "ibshop/rpcc2g_queryibitem.h"
#include "sprite/rpcc2g_spriteoperation.h"
#include "scene/rpcc2g_deprogressreq.h"
#include "scene/rpcc2m_derankreq.h"
#include "worldboss/rpcc2m_worldbossend.h"
#include "superrisk/rpcc2g_getriskmapinfos.h"
#include "superrisk/rpcc2g_playdicerequest.h"
#include "superrisk/rpcc2g_changeriskboxstate.h"
#include "superrisk/rpcc2g_reenterriskbattle.h"
#include "guild/rpcc2m_reqguildbossinfo.h"
#include "levelseal/rpcc2g_levelsealexchange.h"
#include "guild/rpcc2m_askguildmembers.h"
#include "superrisk/rpcc2g_playdiceover.h"
#include "guild/rpcc2m_askguildskillinfonew.h"
#include "guild/rpcc2m_studyguildskillnew.h"
#include "superrisk/rpcc2g_riskbuyrequest.h"
#include "smelt/rpcc2g_smeltitem.h"
#include "activity/rpcc2g_refreshsweepreward.h"
#include "activity/rpcc2g_getsweeptowerreward.h"
#include "guild/rpcc2g_learnguildskill.h"
#include "ranklist/rpcc2m_mtshowtoplist.h"
#include "guild/rpcc2m_getguildbosstimeleft.h"
#include "guild/rpcc2m_fetchguildapp.h"
#include "guild/rpcc2m_guildapprovalnew.h"
#include "guild/rpcc2m_changeguildsettingnew.h"
#include "scene/rpcc2g_queryscenetime.h"
#include "guild/rpcc2m_getguildcheckinrecordsnew.h"
#include "guild/rpcc2m_guildcheckinnew.h"
#include "guild/rpcc2m_queryguildcheckinnew.h"
#include "activity/rpcc2g_gettowerfirstpassreward.h"
#include "guild/rpcc2m_guildfatigueopnew.h"
#include "guild/rpcc2m_fetchguildhistorynew.h"
#include "guild/rpcc2m_changememberpositionnew.h"
#include "guild/rpcc2m_getotherguildbriefnew.h"
#include "guild/rpcc2m_askguildarenainfonew.h"
#include "guild/rpcc2m_askguildarenateaminfonew.h"
#include "guild/rpcc2m_adjustguildarenaroleposnew.h"
#include "guild/rpcc2m_guildarenasimpledeploynew.h"
#include "guild/rpcc2m_reqguildladderinfo.h"
#include "guild/rpcc2m_reqguildladderrnakinfo.h"
#include "garden/rpcc2m_startplant.h"
#include "garden/rpcc2m_fetchplantinfo.h"
#include "garden/rpcc2m_plantcultivation.h"
#include "garden/rpcc2m_plantharvest.h"
#include "garden/rpcc2m_tryfish.h"
#include "garden/rpcc2m_friendgardenplantlog.h"
#include "garden/rpcc2m_gardencookingfood.h"
#include "skycity/rpcc2m_skycityenter.h"
#include "scene/rpcc2g_gmfreadyreq.h"
#include "battle/rpcc2g_inspirereq.h"
#include "guild/rpcc2g_reqguildbosstimeleft.h"
#include "scene/rpcc2g_skycityallinforeq.h"
#include "guild/rpcc2m_reqguildrankinfo.h"
#include "guild/rpcc2m_getguildbindinfo.h"
#include "guild/rpcc2m_guildbindgroup.h"
#include "guild/rpcc2m_guildjoinbindgroup.h"
#include "guild/rpcc2m_guildunbindgroup.h"
#include "guild/rpcc2m_gmfjoinreq.h"
#include "spactivity/rpcc2g_getspactivityreward.h"
#include "spactivity/rpcc2g_getspactivitybigprize.h"
#include "garden/rpcc2m_gardenbanquet.h"
#include "garden/rpcc2m_gardenoverview.h"
#include "levelseal/rpcc2g_getlevelsealselfgift.h"
#include "garden/rpcc2m_gardenbanquetaward.h"
#include "garden/rpcc2m_activecookbook.h"
#include "garden/rpcc2m_gardenfishinfo.h"
#include "pay/rpcc2m_paynotify.h"
#include "garden/rpcc2m_gardensteal.h"
#include "garden/rpcc2m_gardenexpelsprite.h"
#include "platfriend/rpcc2m_reqplatfriendranklist.h"
#include "platfriend/rpcc2m_sendgift2platfriend.h"
#include "guild/rpcc2m_askguildwageinfo.h"
#include "guild/rpcc2m_getguildwagereward.h"
#include "garden/rpcc2m_opengardenfarmland.h"
#include "reswar/rpcc2m_reswarexplore.h"
#include "reswar/rpcc2m_queryreswar.h"
#include "lottery/rpcc2g_goldclick.h"
#include "auction/rpcc2m_guildauctreqall.h"
#include "guild/rpcc2m_getguildbonussendlist.h"
#include "guild/rpcc2m_sendguildbonusinsendlist.h"
#include "reswar/rpcc2m_useguildbuff.h"
#include "reswar/rpcc2g_reswarallinforeqone.h"
#include "reswar/rpcc2m_startreswarpve.h"
#include "guild/rpcc2m_applyguildarena.h"
#include "guild/rpcc2m_getapplyguildlist.h"
#include "guild/rpcc2m_getintegralbattleinfo.h"
#include "guild/rpcc2m_getguildintegralinfo.h"
#include "guildcamp/rpcc2m_guildcampinfo.h"
#include "reswar/rpcc2m_queryreswarrolerank.h"
#include "item/rpcc2g_itembuffop.h"
#include "guild/rpcc2m_reqguildarenahistory.h"
#include "dance/rpcc2g_justdance.h"
#include "task/rpcc2g_getdailytaskinfo.h"
#include "task/rpcc2g_getdailytaskreward.h"
#include "task/rpcc2g_dailytaskaskhelp.h"
#include "task/rpcc2m_getguilddonateinfo.h"
#include "task/rpcc2m_donatememberitem.h"
#include "guild/rpcc2m_addguildinherit.h"
#include "guild/rpcc2m_aceptguildinherit.h"
#include "guild/rpcc2m_reqguildinheritinfo.h"
#include "guild/rpcc2m_delguildinherit.h"
#include "qqvip/rpcc2g_queryqqvipinfo.h"
#include "task/rpcc2g_dailytaskgiveup.h"
#include "platnotice/rpcc2m_fetchplatnotice.h"
#include "chat/rpcc2m_joinfmroom.h"
#include "reswar/rpcc2g_reswarbuff.h"
#include "role/rpcc2m_changenamenew.h"
#include "qqvip/rpcc2g_queryqqfriendsvipinfo.h"
#include "equip/rpcc2g_enchantequip.h"
#include "partner/rpcc2m_getpartnerinfo.h"
#include "mentorship/rpcc2m_getothermentorstatus.h"
#include "mentorship/rpcc2m_getmymentorinfo.h"
#include "mentorship/rpcc2m_getmyapplystudentinfo.h"
#include "mentorship/rpcc2m_getmyapplymasterinfo.h"
#include "mentorship/rpcc2m_mentormybeappliedmsg.h"
#include "mentorship/rpcc2m_updatementorapplystudentinfo.h"
#include "mentorship/rpcc2m_mentorrelationop.h"
#include "partner/rpcc2g_takepartnerchest.h"
#include "partner/rpcc2m_getpartnerdetailinfo.h"
#include "partner/rpcc2m_getpartnerliveness.h"
#include "partner/rpcc2m_leavepartner.h"
#include "herobattle/rpcc2g_getherobattleinfo.h"
#include "partner/rpcc2m_cancelleavepartner.h"
#include "invfight/rpcc2m_invfightreqall.h"
#include "horse/rpcc2g_horsereconnect.h"
#include "photo/rpcc2g_photographeffect.h"
#include "guildcastle/rpcc2m_gcfcommonreq.h"
#include "guildcastle/rpcc2m_gcfreadysinforeq.h"
#include "partner/rpcc2m_getpartnershop.h"
#include "partner/rpcc2m_buypartnershopitem.h"
#include "guild/rpcc2m_reqguildterrcityinfo.h"
#include "guild/rpcc2m_reqguildterrchallinfo.h"
#include "guild/rpcc2m_reqguildterrallianceinfo.h"
#include "partner/rpcc2m_getpartnershoprecord.h"
#include "pandora/rpcc2g_pandoralottery.h"
#include "guild/rpcc2m_allianceguildterr.h"
#include "guildcastle/rpcc2m_gcffightinforeqc2m.h"
#include "guild/rpcc2m_recalliance.h"
#include "guild/rpcc2m_tryalliance.h"
#include "audiotext/rpcc2a_audioauthkey.h"
#include "audiotxt/rpcc2a_audiotext.h"
#include "guild/rpcc2m_clearguildterralliance.h"
#include "chat/rpcc2g_queryclientip.h"
#include "herobattle/rpcc2g_setheroinherobattle.h"
#include "guildbonus/rpcc2g_getguildbonusleft.h"
#include "role/rpcc2m_getunitappearancenew.h"
#include "herobattle/rpcc2g_getherobattlegamerecord.h"
#include "herobattle/rpcc2g_buyheroinherobattle.h"
#include "herobattle/rpcc2g_getherobattleweekreward.h"
#include "league/rpcc2m_getleaguebattleinfo.h"
#include "league/rpcc2m_getleagueteaminfo.h"
#include "league/rpcc2m_leaveleagueteam.h"
#include "league/rpcc2m_leaguebattlereq.h"
#include "atlas/rpcc2g_atlasupstar.h"
#include "leaguebattle/rpcc2g_leaguebattlereadyreq.h"
#include "equip/rpcc2g_forgeequip.h"
#include "subscribe/rpcc2m_getlistsubscribe.h"
#include "subscribe/rpcc2m_setsubscribe.h"
#include "role/rpcc2g_changeprofession.h"
#include "holidaystage/rpcc2g_getholidaystageinfo.h"
#include "league/rpcc2m_getleaguebattlerecord.h"
#include "item/rpcc2g_itemsell.h"
#include "equip/rpcc2g_enchanttransfer.h"
#include "league/rpcc2m_getleagueeleinfo.h"
#include "league/rpcc2m_joinleagueelebattle.h"
#include "role/rpcc2g_personalcareer.h"
#include "role/rpcc2g_changedeclaration.h"
#include "platnotice/rpcc2m_clicknewnotice.h"
#include "skill/rpcc2g_changeskillset.h"
#include "pay/rpcc2g_getpayreward.h"
#include "guild/rpcc2m_reqguildterrintellinfo.h"
#include "pay/rpcc2g_payfrienditem.h"
#include "item/rpcc2g_itemcompose.h"
#include "ranklist/rpcc2m_queryranklistlua.h"
#include "spactivity/rpcc2g_getbackflowspacttime.h"
#include "worldboss/rpcc2m_worldbossguildaddattr.h"
#include "spactivity/rpcc2g_argentaactivity.h"
#include "guild/rpcc2m_modifymsguildname.h"
#include "lottery/rpcc2g_getplatformsharechest.h"
#include "loginactivity/rpcc2g_loginactivity.h"
#include "pay/rpcc2g_getgrowthfundtime.h"
#include "arenastar/rpcc2m_arenastarrolereq.h"
#include "skyteam/rpcc2m_leaveskyteam.h"
#include "skycraft/rpcc2m_skycraftmatchreq.h"
#include "custombattle/rpcc2m_custombattleop.h"
#include "skycraft/rpcc2m_getskycraftinfo.h"
#include "skycraft/rpcc2m_getskycraftrecord.h"
#include "skycraft/rpcc2m_getskycrafteliinfo.h"
#include "skycraft/rpcc2m_getskycraftteaminfo.h"
#include "pay/rpcc2m_ibgifthistreq.h"
#include "fashion/rpcc2g_activatefashioncharm.h"
#include "guild/rpcc2g_getguildcampartyranditem.h"
#include "guild/rpcc2g_talkguildcampsprite.h"
#include "guild/rpcc2g_guildcamppartyreqexchange.h"
#include "guild/rpcc2g_getguildcamppartyexchangeinfo.h"
#include "guild/rpcc2g_guildcampexchangeoperate.h"
#include "guild/rpcc2g_getguildcamppartyreward.h"
#include "pay/rpcc2m_giftibreqgoods.h"
#include "artifact/rpcc2g_artifactcompose.h"
#include "guild/rpcc2g_replypartyexchangeitemopt.h"
#include "absparty/rpcc2g_absenterscene.h"
#include "enchant/rpcc2g_enchantactiveattribute.h"
#include "emblem/rpcc2g_buyextraskillemblemslot.h"
#include "battlewatch/rpcc2g_changelivevisible.h"
#include "battle/rpcc2g_reportbadplayer.h"
#include "match/rpcc2m_kmatchcommonreq.h"
#include "guild/rpcc2g_getguildpartyreceiveinfo.h"
#include "smelt/rpcc2g_returnsmeltstone.h"
#include "platformshareresult/rpcc2g_getplatshareaward.h"
#include "weekend4v4/rpcc2g_weekend4v4getinfo.h"
#include "spactivity/rpcc2m_getfloweractivityreward.h"
#include "scene/rpcc2g_scenemobaop.h"
#include "mobabattle/rpcc2g_mobasignaling.h"
#include "fashion/rpcc2g_activatehaircolor.h"
#include "common/rpcc2g_dhrreqc2g.h"
#include "mobabattle/rpcc2m_getmobabattlegamerecord.h"
#include "mobabattle/rpcc2m_getmobabattlebriefrecord.h"
#include "pet/rpcc2g_petoperationother.h"
#include "login/rpcc2n_loginreconnectreq.h"
#include "bigmelee/rpcc2m_enterbmreadyscene.h"
#include "bigmelee/rpcc2m_querybigmeleerank.h"
#include "mobabattle/rpcc2m_getmobabattleinfo.h"
#include "mobabattle/rpcc2m_getmobabattleweekreward.h"
#include "sprite/rpcc2g_buyspriteegg.h"
#include "enhance/rpcc2g_getenhanceattr.h"
#include "groupchat/rpcc2m_groupchatleaderreviewlist.h"
#include "groupchat/rpcc2m_groupchatfindroleinfolist.h"
#include "groupchat/rpcc2m_groupchatfindteaminfolist.h"
#include "groupchat/rpcc2m_groupchatplayerissueinfo.h"
#include "groupchat/rpcc2m_groupchatleaderissueinfo.h"
#include "groupchat/rpcc2m_groupchatleaderreview.h"
#include "groupchat/rpcc2m_groupchatplayerapply.h"
#include "groupchat/rpcc2m_groupchatgetgroupinfo.h"
#include "groupchat/rpcc2m_groupchatmanager.h"
#include "groupchat/rpcc2m_groupchatquit.h"
#include "groupchat/rpcc2m_groupchatcreate.h"
#include "spactivity/rpcc2g_getancienttimesaward.h"
#include "herobattle/rpcc2g_selectheroancientpower.h"
#include "groupchat/rpcc2m_groupchatclear.h"
#include "wedding/rpcc2m_holdwedding.h"
#include "wedding/rpcc2m_getallweddinginfo.h"
#include "wedding/rpcc2m_getweddinginviteinfo.h"
#include "wedding/rpcc2m_enterweddingscene.h"
#include "marriage/rpcc2m_getmarriagerelation.h"
#include "marriage/rpcc2m_marriagerelationop.h"
#include "wedding/rpcc2m_weddinginviteoperator.h"
#include "groupchat/rpcc2m_groupchatleaderaddrole.h"
#include "wedding/rpcc2g_weddingoperator.h"
#include "spactivity/rpcc2g_tactenterscene.h"
#include "groupchat/rpcc2m_groupchatchangename.h"
#include "dragongroup/rpcc2g_dragongrouprolelist.h"
#include "dragongroup/rpcc2g_dragongrouprecord.h"
#include "marriage/rpcc2m_getmarriageliveness.h"
#include "marriage/rpcc2m_takemarriagechest.h"
#include "pay/rpcc2m_checkrolebeforepay.h"
#include "artifact/rpcc2g_artifactdeitystoveop.h"
#include "wedding/rpcc2m_startweddingcar.h"
#include "spactivity/rpcc2g_themeactivityhint.h"
#include "pay/rpcc2g_backflowbuyop.h"
#include "transform/rpcc2g_transformop.h"
#include "equip/rpcc2g_upgradeequip.h"
#include "fashion/rpcc2g_getfashionsynthesisinfo.h"
#include "fashion/rpcc2g_fashionibshopbuy.h"
#include "battlefield/rpcc2m_enterbattlereadyscene.h"
#include "dragonguild/rpcc2m_getdragonguildliveness.h"
#include "dragonguild/rpcc2m_createorjoindragonguild.h"
#include "dragonguild/rpcc2g_takedragonguildchest.h"
#include "dragonguild/rpcc2m_fetchdgapps.h"
#include "dragonguild/rpcc2m_changedragonguildsetting.h"
#include "dragonguild/rpcc2m_dragonguildapproval.h"
#include "dragonguild/rpcc2m_leavefromdragonguild.h"
#include "dragonguild/rpcc2m_modifydragonguildname.h"
#include "dragonguild/rpcc2m_askdragonguildmembers.h"
#include "dragonguild/rpcc2m_fetchdragonguildlist.h"
#include "dragonguild/rpcc2m_getdragonguildtaskinfo.h"
#include "dragonguild/rpcc2g_getdragonguildtaskchest.h"
#include "dragonguild/rpcc2m_buydragonguildshopitem.h"
#include "dragonguild/rpcc2m_getdragonguildshop.h"
#include "dragonguild/rpcc2m_getdragonguildshoprecord.h"
#include "dragonguild/rpcc2m_changedragonguildposition.h"
#include "dragonguild/rpcc2m_getdragonguildbindinfo.h"
#include "dragonguild/rpcc2m_dragonguildbindgroupreq.h"
#include "dragonguild/rpcc2m_dragonguildjoinbindgroup.h"
#include "dragonguild/rpcc2m_dragonguildunbindgroup.h"
#include "goalawards/rpcc2m_goalawardsgetlist.h"
#include "goalawards/rpcc2m_goalawardsgetawards.h"
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
#include "spactivity/rpcc2g_getluckyactivityinfo.h"
#include "spactivity/rpcc2g_buydraw.h"
#include "spactivity/rpcc2g_lotterydraw.h"
#include "npcfeeling/rpcc2m_npcflreqc2m.h"
#include "task/rpcc2m_getdailytaskrefreshinfo.h"
#include "task/rpcc2m_getdailytaskaskhelp.h"
#include "task/rpcc2m_getdailytaskrefreshrecord.h"
#include "task/rpcc2m_dailytaskrefreshoper.h"
#include "survive/rpcc2g_survivereqc2g.h"
#include "battle/rpcc2g_doodaditemallskillreq.h"
#include "scene/rpcc2g_peerbox.h"
#include "scene/rpcc2g_queryboxs.h"
#include "pay/rpcc2g_activatepreshow.h"
#include "guild/rpcc2g_guildpartysummonspirit.h"
#include "fashion/rpcc2g_choosespecialeffects.h"
#include "scene/rpcc2g_enterleisurescene.h"
#include "spactivity/rpcc2g_backflowactivityoperation.h"
#include "activity/rpcc2g_getnewzonebenefit.h"
#include "activity/rpcc2g_selectchargebackrole.h"


static BaseLink *FindLink(const char *linkName)
{
	return Config::GetConfig()->FindLinkHandler(linkName);
}

void CProtocolRegister::Regiter() 
{
	CProtocol::Register(new PtcC2T_KeepAlivePingAck());
	CProtocol::Register(new PtcG2T_OpenGate());
	CProtocol::Register(new PtcG2T_ChangeGsLine());
	CProtocol::Register(new PtcN2T_KickAccount());
	CProtocol::Register(new PtcM2T_chatforward());
	CProtocol::Register(new PtcM2T_VersionNotify());
	CProtocol::Register(new PtcM2T_BroadCastM2T());
	CProtocol::Register(new PtcN2T_NotifyGtSessionLogin());
	CProtocol::Register(new PtcG2T_BroadCastG2T());
	CProtocol::Register(new PtcM2T_NotifyRouterInfo2Gate());
	CProtocol::Register(new PtcN2T_SessionChange2Gt());
	ProtocolForward::RegistForward(PTCG2C_NOTIFYACCOUNTDATA_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_ENTERSCENEREQ_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_ENTERSCENENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_LEAVESCENEREQ_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_LEAVESCENENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_MOVEOPERATIONREQ_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_UNITAPPEAR_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_UNITDISAPPEAR_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_CASTSKILL_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_SKILLRESULT_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_DEATHNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_ATTRIBUTECHANGENOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_LEVELCHANGENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_ITEMCHANGEDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_UPDATETUTORIAL_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_OPENSYSTEMNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_BATTLERESULTNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_COMPLETEACHIVEMENT_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SKILLCHANGEDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GSERRORNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_CHANGEFASHIONNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_CHECKININFONOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_ACTIVITYROLENOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_UPDATEPAYINFO_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_REWARDCHANGEDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_LEAVETEAM_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_REVIVENOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_UPDATEBUYGOLDANDFATIGUEINFO_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_ROLEDEATHINSOLOSCENE_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_REVIVECOUNTDOWN_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_FATIGUERECOVERTIMENOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_ENTERSCENECOOLDOWNQUERY_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_ENTERSCENECOOLDOWNNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_FASHOINCHANGEDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_OPERATERECORDNTF_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_GUILDNOTIFYMEMBERCHANGED_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GUILDSETTINGCHANGEDNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GUILDCHECKINBOXNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_NEWGUILDBONUSNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GUILDBESTCARDSNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_EXECUTELEVELSCRIPTNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_ONLINEREWARDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_NEXTDAYREWARDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_ONLINEREWARDREPORT_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCC2G_NEXTDAYREWARDREPORT_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_HINTNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_POSITIONDEBUG_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_ENEMYDORPDOODADNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_TAKERANDOMTASK_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_BUFFNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_TEAMSELECTNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_TEAMINVITENOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_FETCHENEMYDOODADNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_DELAYNOTIFY_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_PKPREPARENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_PKTIMEOUTNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GUILDGOBLINKILLNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_SKILLRESULTREQ_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCC2G_SKILLBULLETFIREREPORT_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCC2G_SKILLBULLETRESULTREQ_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_SCENEFINISHSTATECHANGED_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_PKROLEINFONTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_ITEMCIRCLEDRAWRESULT_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_ADDLEVELSCRIPTEXTSTRING_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCC2G_SCENEDAMAGERANKREPORT_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_SCENEDAMAGERANKNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SCENELEFTDOODAD_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_CHANGESUPPLEMENTREPORT_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_CHANGESUPPLEMENTNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_QTEOPERATION_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_CORRECTPOSITION_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_TELEPORTNOTICE_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_ALLYMATCHROLEIDNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_TEAMFULLDATANTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_PETCHANGENOTFIY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_EXPFINDBACKNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_AUTOFIGHTNTF_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_TOWERSCENEINFONTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_COVERDESIGNATIONNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_REACHACHIEVENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_REACHDESIGNATIONNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_SYNCSCENEFINISH_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_LEVELSEALNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_RECONNECTSYNCNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_OUTLOOKCHANGENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_FIVEDAYREWARDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SYNCSTEPNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_TSSSDKSENDANTI2SERVER_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_TSSSDKANTIDATANTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_ANSWERACKNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_PUSHQUESTIONNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_QANOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_PVPBATTLEBEGINNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_PVPBATTLEENDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_PVPBATTLEKILL_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_FLOWERRANKREWARDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_QAOVERNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_QAROOMRANKNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_FASTMBCONFIRMNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_FMBREFUSE_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_FASTMBDISMISSNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SELECTROLENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_RECEIVEFLOWERNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_FLOWERRAINNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_STARTBATTLEFAILEDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_QAENTERROOMNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_WATCHBATTLEINFONTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GMFJOINBATTLEREQ_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GMFONEBATTLEENDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GMFBASEDATANTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_WORLDCHANNELLEFTTIMESNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_NOTICEGUILDBOSSEND_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_UPDATEGUILDARENASTATE_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GUILDBOSSTIMEOUT_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SYNGUILDARENABATTLEINFO_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SYNGUILDARENAFIGHTUNIT_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_MULACTIVITYSTATECHANGE_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GMFALLFIGHTENDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_WORLDBOSSSTATENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_NOTIFYROLEEMPTY2WATCHER_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_BATTLEWATCHERNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_NOTICEGUILDARENABEGIN_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_NOTIFYGUILDBOSSADDATTR_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_NOTIFYWATCHDATA_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_NOTIFYCLIENTENTERFIGHT_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SYNGUILDARENAROLEONLINE_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_CLIENTONLYBUFFNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_MCCHATOFFLINENOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_FRIENDOPNTFNEW_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2M_LOADOFFLINECHATNTF_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCC2M_FRIENDQUERYREPORTNEW_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCM2C_FRIENDDEGREEUPNTFNEW_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2M_BLACKLISTREPORTNEW_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCM2C_BLACKLISTNTFNEW_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SYNGUILDARENADISPLACE_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_ONTRANSFERWALL_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_NOTIFYWATCHICONNUM2CLIENT_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GMFWAITOTHERLOAD_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GMFWAITFIGHTBEGIN_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2M_ROLESTATEREPORTNEW_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCM2C_ROLESTATENTFNEW_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_HINTNOTIFYMS_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_WORLDBOSSATTRNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_JOINROOM_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_JOINROOMREPLY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_QUITROOM_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_UPDATEVOIPROOMMEMBERNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_PAYALLINFONTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_LOGINREWARD2CNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_MSRECEIVEFLOWERNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_MSFLOWERRAINNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_TEAMCHANGEM2CNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_LEAVETEAMM2CNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_TEAMFULLDATAM2CNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_TEAMINVITEM2CNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_MSERRORNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_INVITEREFUSEM2CNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2M_TEAMINVITEACKC2M_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCM2C_ROLEMATCHSTATEM2CNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_FASTMBCONFIRMM2CNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_FASTMBDISMISSM2CNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2M_FMBREFUSEC2M_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCG2C_AIDEBUGINFO_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_STARTROLLNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2N_CHECKQUEUINGREQ_TYPE, FindLink("ctrllink"));
	ProtocolForward::RegistForward(PTCN2C_CHECKQUEUINGNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_NOTICEHUNGRYDOWN_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_FIRSTPASSREWARDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_TITLECHANGENOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_QAIDNAMENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_PKTIMEOUTM2CNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_LOGINGUILDINFO_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_SENDGUILDSKILLINFO_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_MSEVENTNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_ITEMFINDBACKNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SPRITECHANGEDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_FIGHTGROUPCHANGENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_BOSSRUSHONEFINISHNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SYNATLASATTR_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_PLAYDICENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_PLAYDICEOVER_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_IBSHOPICON_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_REMOVEIBSHOPICON_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_NOTIFYENHANCESUIT_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_SYNGUILDARENAFIGHTUNITNEW_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_SYNGUILDARENABATTLEINFONEW_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_NOTICEGUILDARENABEGINNEW_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_SYNGUILDARENAROLEONLINENEW_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_SYNGUILDARENADISPLACENEW_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_RISKBUYNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_SETVOIPMEMBERSTATE_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_RISKRECOVERDICENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_NOTIFYGUILDSKILLDATA_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_OPENSPRITEEGGNTF_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCM2C_GUILDSETINGNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_GUILDBESTCARDSNTFMS_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_TOWERFIRSTPASSREWARDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_DERANKCHANGNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_GMFJOINBATTLEM2CREQ_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2M_GMFJOINBATTLEC2MRES_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCG2C_HALLICONSNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_UPDATETASKSTATUS_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_SKYCITYFINALRES_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SKYCITYESTIMATERES_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SKYCITYTIMERES_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_NOTIFYIDIPMESSAGEGS_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_NOTIFYIDIPMESSAGEMS_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_IDIPPUNISHINFONTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_IDIPPUNISHINFOMSNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_IDIPCLEARCHATNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SKYCITYTEAMRES_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2M_GUILDCARDRANKREQ_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCM2C_GUILDCARDRANKNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_NOTICEGUILDLADDERSTART_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GMFKICKNTY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2M_GUILDCARDMATCHREQ_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCM2C_GUILDCARDMATCHNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2M_OPENPRIVATECHATNTF_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCG2C_SKYCITYBATTLEDATANTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2M_COMMITANSWERNTF_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCC2M_GIVEUPQAQUESTIONNTF_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCG2C_GMFBATTLESTATENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SPACTIVITYCHANGENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SPACTIVITYOFFSETDAYNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_SPRITESTATECHANGENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_INVUNFSTATEM2CNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_DPSNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_NOTICEGUILDARENANEXTTIME_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SKILLCOOLNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_GARDENBANQUETNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GARDENPLANTEVENTNOTICE_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GARDENBANQUETNOTICE_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2M_SYNCPLATFRIEND2MS_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCM2C_NOTICEGUILDWAGEREWARD_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GMFGUILDCOMBATNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GPRONEBATTLEENDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GPRALLFIGHTENDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_RESWARBATTLEDATANTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_RESWARTEAMRESONE_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_SENDGUILDBONUSNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_BATTLELOGREPORT_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCM2C_RESWARGUILDBRIEFNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_CLICKGUILDCAMP_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_PAYMEMBERPRIVILEGENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_SYNGUILINTEGRALSTATE_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_RESWARSTATENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_SYNGUILDINTEGRALSTATE_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_KILLENEMYSCORENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SERVEROPENDAYNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_RESWARRANKSIMPLEINFONTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_GUILDBUFFSIMPLEINFONTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2M_QUERYRESWARREQUET_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCM2C_GUILDBUFFSIMPLEITEMNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2M_GARDENFISHSTOP_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCM2C_HALLICONMNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SYNGUILDINHERITEXP_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_SYNDOINGGUILDINHERIT_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_NOTICEDOINGGUILDINHERIT_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SYNGUILDINHERITNUMINFO_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_RESWARMINEDATANTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_CDCALL_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2M_LEAVELARGEROOM_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCM2C_LARGEROOMROLENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_LARGEROOMLOGINPARAMNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_RESWARTIMENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SYNCMOVENOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2M_HANDLEMICPHONE_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCG2C_CHANGENAMECOUNTNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_GUILDBUFFCDPARAMNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_ANTIADDICTIONREMINDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_HORSEAWARDALLNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_HORSERANKNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_HORSEWAITTIMENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_HORSEANIMATIONNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_HORSEFINALNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_PICURLNTF_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCM2C_NOTIFYMENTORAPPLY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2M_POKERTOURNAMENTREQ_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCM2C_UPDATEPARTNERTOCLIENT_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_MAKEPARTNERRESULTNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_INVFIGHTNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_HORSECOUNTDOWNTIMENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_DOODADITEMADDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GCFZHANLINGNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_INVFIGHTBEFENTERSCENENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_HEROBATTLEDATANTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SCENEPREPAREINFONTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_HEROBATTLETEAMMSGNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_HEROBATTLESYNCNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_HEROBATTLEINCIRCLENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_HEROBATTLEOVERTIME_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2M_PAYPARAMETERINFONTF_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCM2C_PAYPARAMETERINFOINVALIDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_HEROBATTLETEAMROLENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GCFSYNG2CNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_NOTICEGUILDTERRWAR_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_NOTICEGUILDTERRALL_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2N_UPDATESTARTUPTYPENTF_TYPE, FindLink("ctrllink"));
	ProtocolForward::RegistForward(PTCG2C_DOODADITEMUSENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_GUILDBONUSGETALL_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SPECIALSTATENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_RESWARENEMYTIMENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_CLOSECHOOSEHERONTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_LEVELSCRIPTSTATENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SCENESTATENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_QUERYROLESTATEREQ_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_QUERYROLESTATEACK_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_IBSHOPHASBUYNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_UPDATELEAGUEBATTLESEASONINFO_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SKILLINITCOOLNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_NOTIFYLEAGUETEAMCREATE_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_NOTIFYLEAGUETEAMDISSOLVE_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_HEROBATTLECANUSEHERO_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_UPDATELEAGUETEAMSTATE_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_LEAGUEBATTLESTARTMATCHNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_LEAGUEBATTLESTOPMATCHNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_LEAGUEBATTLELOADINFONTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_LEAGUEBATTLEBASEDATANTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_LEAGUEBATTLEONERESULTNFT_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_LEAGUEBATTLERESULTNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_PUSHPRAISENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_HEROBATTLETIPSNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_LEAGUEBATTLESTATENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_LEAGUEBATTLEMATCHTIMEOUTNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_CANCELREDDOTREQ_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCM2C_MIDASEXCEPTIONNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_UPDATELEAGUEELEROOMSTATENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2M_CLOSELEAGUEELENTF_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCM2C_KICKFMAUCHOR_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_GUILDAUCTITEMTIMEFRESH_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_POKERTOURNAMENTENDREFUND_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_NOTICEGUILDTERREND_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_NOTICEGUILDTERRBATTLEWIN_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_NOTICEGUILDTERRBIGICON_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_CLOSEHINTNTF_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCA2C_AUDIOAIDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_COUNTDOWNNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_ALLGIFTIBITEMNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GIFTIBBACKINFONTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_SHARERANDOMGIFTNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_MODIFYGUILDNAMENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_WORLDBOSSGUILDADDATTRSYNCCLIENTNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_MILITARYRANKNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_LOGINACTIVITYSTATUSNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_LOGINACTIVITYAWARDREQ_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCC2M_PAYBUYGOODSFAILNTF_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCC2M_IBGIFTTIPSREQ_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCM2C_ARENASTARDATANTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_NOTIFYSKYTEAMCREATE_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_SKYCRAFTMATCHNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_SKYCRAFTELIROOMNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2M_CLOSESKYCRAFTELINTF_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCG2C_DISPLAYADDITEM_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_UPDATEDISPLAYITEMS_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GUILDCAMPPARTYNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_IBGIFTICONNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_GIFTIBSUCESSNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_ABSPARTYNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_GUILDCAMPPARTYTRADENOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_TEAMREQUESTPLATFREIND2CLIENT_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_CUSTOMBATTLELOADINGNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_CUSTOMBATTLEGMNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_TARJABRIEFNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_PLATFORMSHAREAWARDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_NOTIFYPLATSHARERESULT_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_WEEKEND4V4ROLEDATANTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_HEROKILLNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_MOBABATTLETEAMROLENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_MOBAROLECHANGENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_MOBABATTLETEAMMSGNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_TRANSSKILLNOTFIY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_ENTITYTARGETCHANGENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_MOBASIGNALBROADCAST_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_PETINVITENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_CHATNOTIFYG2C_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_MOBAMINIMAPNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_MOBAADDEXPNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_BMREADYTIMENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_BMFIGHTTIMENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_BMROLESCENESYNCNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_BIGMELEERELIVENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_MOBAHINTNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_TAJIEHELPNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_NOTIFYSTARTUPTYPETOCLIENT_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_HEROBATTLEANCIENTPOWER_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_GROUPCHATMANAGER_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_GROUPCHATQUIT_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_GROUPCHATDISMISS_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_WEDDINGINVITENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_NOTIFYMARRIAGEAPPLY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_BATTLESTATISTICSREPORT_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_BATTLESTATISTICSNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_WEDDINGLOADINFONTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_WEDDINGSTATENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_WEDDINGEVENTNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_HORSEFAILTIPSNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_NOTIFYMARRIAGEDIVORCEAPPLY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_WARNINGRANDOMNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_GROUPCHATISSUECOUNT_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_GROUPCHATAPPLY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_SHAREREPORT_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_THEMEACTIVITYCHANGENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_WEDDINGCARNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_BACKFLOWOPENNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_STARTBATTLEFAILEDM2CNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_TRANSNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_INVFIGHTAGAINREQC2G_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCM2C_LOGINDRAGONGUILDINFO_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_DRAGONGUILDSETTINGCHANGED_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_DRAGONGUILDNAMENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2M_GOBACKREADYSCENENTF_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCM2C_GOALAWARDSREDPOINT_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_BATTLEFIELDREADYINFONTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_BATTLEFIELDRELIVENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_BATTLEFIELDROLEAGAINSTNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_BFFIGHTTIMENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_PAYSCORENTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_NPCFLNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_DAILYTASKEVENTNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2G_UPDATEFREEFLOWHINTINFO_TYPE, FindLink("gslink"));
	ProtocolForward::RegistForward(PTCG2C_DOODADITEMSKILLSNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCC2M_MULACTIVITYICONSYSREQ_TYPE, FindLink("mslink"));
	ProtocolForward::RegistForward(PTCG2C_WORDNOTIFY_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_BIGMELEEPOINTNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_BIGMELEEPOINTOUTLOOKNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_TASKREFRESHNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_KICKACCOUNTJKYDNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCM2C_GUILDCASTFEATSNTF_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_WORLDLEVELNTF2CLIENT_TYPE, FindLink("clientlink"));
	ProtocolForward::RegistForward(PTCG2C_UPDATESTAGEINFONTF_TYPE, FindLink("clientlink"));
	CRpc::RegistRpc(RpcC2T_Reconnect::CreateRpc());
	CRpc::RegistRpc(RpcC2T_ClientLoginRequest::CreateRpc());
	CRpc::RegistRpc(RpcT2M_RegisterGt2Ms::CreateRpc());
	CRpc::RegistRpc(RpcG2T_RegisterGs2Gt::CreateRpc());
	CRpc::RegistRpc(RpcT2G_RegisterGt2Gs::CreateRpc());
	CRpc::RegistRpc(RpcT2N_LoginRequestNew::CreateRpc());
	CRpc::RegistRpc(RpcT2G_ReconnectGs::CreateRpc());
	CRpc::RegistRpc(RpcT2R_RegisterGt2Router::CreateRpc());
	CRpc::RegistRpc(RpcT2N_RegisterGt2Ns::CreateRpc());
	CRpc::RegistRpc(RpcC2T_UpLoadAudioToGate::CreateRpc());
	CRpc::RegistRpc(RpcT2A_UpLoadAudioToAudio::CreateRpc());
	RpcForward::RegistForward(RPCC2G_SYNCTIME_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_SKILLLEVELUP_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_FETCHACHIVEMENTREWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_SELECTCHESTREWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_RESETSKILL_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_CHOOSEPROFESSION_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_BINDSKILL_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_TASKOPERATE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_QUERYLOTTERYCD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_DRAWLOTTERY_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ENHANCEITEM_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_QUERYSCENEDAYCOUNT_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GMCOMMAND_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_DECOMPOSEEQUIPMENT_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_EMBLEMLEVELUP_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_USEITEM_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_JADEOPERATION_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_JADECOMPOSE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_CHECKIN_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETACTIVITYCHEST_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_FASHIONCOMPOSE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_BUYJADESLOT_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETSYSTEMREWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_SWEEP_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_REVIVE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_QUERYSHOPITEM_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_BUYSHOPITEM_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_BUYGOLDANDFATIGUE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ADDTEMPATTR_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_QUERYOPENGAMEACTIVITYTIME_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_SHOWFLOWERPAGE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_SENDFLOWER_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_FETCHCHAPTERCHEST_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_STARTGUILDCARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_QUERYGUILDCARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_CHANGEGUILDCARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ENDGUILDCARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETGUILDCHECKINBOX_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_STAGECOUNTRESET_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETGUILDBONUSLIST_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETGUILDBONUSDETAILINFO_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETGUILDBONUSREWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_OPENSCENECHEST_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_REPORTBATTLE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETONLINEREWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETNEXTDAYREWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_FETCHENEMYDOODADREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GUILDGOBLININFO_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_USESUPPLEMENT_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_SETROLECONFIG_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_LEVELUPSLOTATTR_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_PETOPERATION_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_FETCHMAIL_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_MAILOP_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_BUYTEAMSCENECOUNT_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_EXPFINDBACK_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETTOWERACTIVITYTOP_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETDESIGNATIONREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_SETDESIGNATIONREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETCLASSIFYDESIGNATIONREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETACHIEVEBRIFINFOREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETACHIEVECLASSIFYINFOREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_REQPLAYERAUTOPLAY_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETACHIEVEREWARDREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETLEVELSEALINFO_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_LEVELSEALBUTTONSTATUS_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETACHIEVEPOINTREWARDREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_REQGETLOGINREWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_FIVEDAYREWARDREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GUILDCHECKINBONUSINFO_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_SENDGUILDBONUS_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ASKFORCHECKINBONUS_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_THANKSFORBONUS_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_PVPALLREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_AGREEQAREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETFLOWERLEFTTIME_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETFLOWER_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETFLOWERREWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETFLOWERREWARDLIST_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2A_GETAUDIOLISTREQ_TYPE, FindLink("audiolink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2A_UPLOADAUDIOREQ_TYPE, FindLink("audiolink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ENHANCETRANSTER_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_PVPNOWALLDATA_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETQADATAREQ_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETDRAGONTOPINFO_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_TEAMINVITELISTREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2N_CREATEROLENEW_TYPE, FindLink("ctrllink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2N_SELECTROLENEW_TYPE, FindLink("ctrllink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_IDENTIFYEMBLEM_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_OPENGUILDQAREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETGUILDQADATAREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_MULACTIVITYREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETWATCHINFOBYID_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETMYWATCHRECORD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ENTERWATCHBATTLE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_CHANGEWATCHROLE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_COMMENDWATCHBATTLE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_BOSSRUSHREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2N_RETURNTOSELECTROLE_TYPE, FindLink("ctrllink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_DOENTERSCENE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_TAKEOFFALLJADE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_SWEEPTOWER_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_RESETTOWER_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETGODDESSTRIALREWARDS_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_CHAT_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_DOADDFRIENDNEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_REMOVEFRIENDNEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_ADDBLACKLISTNEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_REMOVEBLACKLISTNEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_FRIENDGIFTOPNEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_RANDOMFRIENDWAITLISTNEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_QUERYPOWERPOINT_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETWORLDBOSSSTATENEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETWORLDBOSSTIMELEFT_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETPAYALLINFO_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_PAYCARDAWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_PAYCLICK_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETACTIVITYINFO_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_SHOWFLOWERPAGENEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_MSGETFLOWERREWARDLIST_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_FETCHTEAMLISTC2M_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_INVHISTORYC2MREQ_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_TEAMREQUESTC2M_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_SYNPETINFO_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_CHOOSEROLLREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_PAYFIRSTAWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GROWTHFUNDAWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_BUYVIPLEVELGIFT_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_TITLELEVELUP_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_FIRSTPASSINFOREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETFIRSTPASSREWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_COMMENDFIRSTPASS_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_FIRSTPASSGETTOPROLEINFO_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_CLIENTQUERYRANKLISTNTF_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_ADDFRIENDNEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_PKREQC2M_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_CLEARPRIVATECHATLIST_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_ASKGUILDBRIEFINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_CREATEORENTERGUILD_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_LEAVEFROMGUILD_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_AUCTIONALLREQ_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_CHANGEOUTLOOKOP_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ITEMFINDBACKINFO_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ITEMFINDBACK_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_REQGUILDLIST_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ACTIVATATLAS_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_AUTOBREAKATLAS_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_BREAKATLAS_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_BUYIBITEM_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_QUERYIBITEM_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_SPRITEOPERATION_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_DEPROGRESSREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_DERANKREQ_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_WORLDBOSSEND_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETRISKMAPINFOS_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_PLAYDICEREQUEST_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_CHANGERISKBOXSTATE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_REENTERRISKBATTLE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_REQGUILDBOSSINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_LEVELSEALEXCHANGE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_ASKGUILDMEMBERS_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_PLAYDICEOVER_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_ASKGUILDSKILLINFONEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_STUDYGUILDSKILLNEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_RISKBUYREQUEST_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_SMELTITEM_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_REFRESHSWEEPREWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETSWEEPTOWERREWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_LEARNGUILDSKILL_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_MTSHOWTOPLIST_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETGUILDBOSSTIMELEFT_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_FETCHGUILDAPP_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GUILDAPPROVALNEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_CHANGEGUILDSETTINGNEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_QUERYSCENETIME_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETGUILDCHECKINRECORDSNEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GUILDCHECKINNEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_QUERYGUILDCHECKINNEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETTOWERFIRSTPASSREWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GUILDFATIGUEOPNEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_FETCHGUILDHISTORYNEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_CHANGEMEMBERPOSITIONNEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETOTHERGUILDBRIEFNEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_ASKGUILDARENAINFONEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_ASKGUILDARENATEAMINFONEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_ADJUSTGUILDARENAROLEPOSNEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GUILDARENASIMPLEDEPLOYNEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_REQGUILDLADDERINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_REQGUILDLADDERRNAKINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_STARTPLANT_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_FETCHPLANTINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_PLANTCULTIVATION_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_PLANTHARVEST_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_TRYFISH_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_FRIENDGARDENPLANTLOG_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GARDENCOOKINGFOOD_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_SKYCITYENTER_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GMFREADYREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_INSPIREREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_REQGUILDBOSSTIMELEFT_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_SKYCITYALLINFOREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_REQGUILDRANKINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETGUILDBINDINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GUILDBINDGROUP_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GUILDJOINBINDGROUP_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GUILDUNBINDGROUP_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GMFJOINREQ_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETSPACTIVITYREWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETSPACTIVITYBIGPRIZE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GARDENBANQUET_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GARDENOVERVIEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETLEVELSEALSELFGIFT_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GARDENBANQUETAWARD_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_ACTIVECOOKBOOK_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GARDENFISHINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_PAYNOTIFY_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GARDENSTEAL_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GARDENEXPELSPRITE_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_REQPLATFRIENDRANKLIST_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_SENDGIFT2PLATFRIEND_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_ASKGUILDWAGEINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETGUILDWAGEREWARD_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_OPENGARDENFARMLAND_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_RESWAREXPLORE_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_QUERYRESWAR_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GOLDCLICK_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GUILDAUCTREQALL_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETGUILDBONUSSENDLIST_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_SENDGUILDBONUSINSENDLIST_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_USEGUILDBUFF_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_RESWARALLINFOREQONE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_STARTRESWARPVE_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_APPLYGUILDARENA_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETAPPLYGUILDLIST_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETINTEGRALBATTLEINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETGUILDINTEGRALINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GUILDCAMPINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_QUERYRESWARROLERANK_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ITEMBUFFOP_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_REQGUILDARENAHISTORY_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_JUSTDANCE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETDAILYTASKINFO_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETDAILYTASKREWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_DAILYTASKASKHELP_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETGUILDDONATEINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_DONATEMEMBERITEM_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_ADDGUILDINHERIT_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_ACEPTGUILDINHERIT_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_REQGUILDINHERITINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_DELGUILDINHERIT_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_QUERYQQVIPINFO_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_DAILYTASKGIVEUP_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_FETCHPLATNOTICE_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_JOINFMROOM_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_RESWARBUFF_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_CHANGENAMENEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_QUERYQQFRIENDSVIPINFO_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ENCHANTEQUIP_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETPARTNERINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETOTHERMENTORSTATUS_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETMYMENTORINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETMYAPPLYSTUDENTINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETMYAPPLYMASTERINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_MENTORMYBEAPPLIEDMSG_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_UPDATEMENTORAPPLYSTUDENTINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_MENTORRELATIONOP_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_TAKEPARTNERCHEST_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETPARTNERDETAILINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETPARTNERLIVENESS_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_LEAVEPARTNER_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETHEROBATTLEINFO_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_CANCELLEAVEPARTNER_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_INVFIGHTREQALL_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_HORSERECONNECT_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_PHOTOGRAPHEFFECT_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GCFCOMMONREQ_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GCFREADYSINFOREQ_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETPARTNERSHOP_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_BUYPARTNERSHOPITEM_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_REQGUILDTERRCITYINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_REQGUILDTERRCHALLINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_REQGUILDTERRALLIANCEINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETPARTNERSHOPRECORD_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_PANDORALOTTERY_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_ALLIANCEGUILDTERR_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GCFFIGHTINFOREQC2M_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_RECALLIANCE_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_TRYALLIANCE_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2A_AUDIOAUTHKEY_TYPE, FindLink("audiolink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2A_AUDIOTEXT_TYPE, FindLink("audiolink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_CLEARGUILDTERRALLIANCE_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_QUERYCLIENTIP_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_SETHEROINHEROBATTLE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETGUILDBONUSLEFT_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETUNITAPPEARANCENEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETHEROBATTLEGAMERECORD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_BUYHEROINHEROBATTLE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETHEROBATTLEWEEKREWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETLEAGUEBATTLEINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETLEAGUETEAMINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_LEAVELEAGUETEAM_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_LEAGUEBATTLEREQ_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ATLASUPSTAR_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_LEAGUEBATTLEREADYREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_FORGEEQUIP_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETLISTSUBSCRIBE_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_SETSUBSCRIBE_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_CHANGEPROFESSION_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETHOLIDAYSTAGEINFO_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETLEAGUEBATTLERECORD_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ITEMSELL_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ENCHANTTRANSFER_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETLEAGUEELEINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_JOINLEAGUEELEBATTLE_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_PERSONALCAREER_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_CHANGEDECLARATION_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_CLICKNEWNOTICE_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_CHANGESKILLSET_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETPAYREWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_REQGUILDTERRINTELLINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_PAYFRIENDITEM_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ITEMCOMPOSE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_QUERYRANKLISTLUA_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETBACKFLOWSPACTTIME_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_WORLDBOSSGUILDADDATTR_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ARGENTAACTIVITY_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_MODIFYMSGUILDNAME_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETPLATFORMSHARECHEST_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_LOGINACTIVITY_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETGROWTHFUNDTIME_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_ARENASTARROLEREQ_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_LEAVESKYTEAM_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_SKYCRAFTMATCHREQ_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_CUSTOMBATTLEOP_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETSKYCRAFTINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETSKYCRAFTRECORD_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETSKYCRAFTELIINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETSKYCRAFTTEAMINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_IBGIFTHISTREQ_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ACTIVATEFASHIONCHARM_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETGUILDCAMPARTYRANDITEM_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_TALKGUILDCAMPSPRITE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GUILDCAMPPARTYREQEXCHANGE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETGUILDCAMPPARTYEXCHANGEINFO_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GUILDCAMPEXCHANGEOPERATE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETGUILDCAMPPARTYREWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GIFTIBREQGOODS_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ARTIFACTCOMPOSE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_REPLYPARTYEXCHANGEITEMOPT_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ABSENTERSCENE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ENCHANTACTIVEATTRIBUTE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_BUYEXTRASKILLEMBLEMSLOT_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_CHANGELIVEVISIBLE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_REPORTBADPLAYER_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_KMATCHCOMMONREQ_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETGUILDPARTYRECEIVEINFO_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_RETURNSMELTSTONE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETPLATSHAREAWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_WEEKEND4V4GETINFO_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETFLOWERACTIVITYREWARD_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_SCENEMOBAOP_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_MOBASIGNALING_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ACTIVATEHAIRCOLOR_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_DHRREQC2G_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETMOBABATTLEGAMERECORD_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETMOBABATTLEBRIEFRECORD_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_PETOPERATIONOTHER_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2N_LOGINRECONNECTREQ_TYPE, FindLink("ctrllink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_ENTERBMREADYSCENE_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_QUERYBIGMELEERANK_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETMOBABATTLEINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETMOBABATTLEWEEKREWARD_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_BUYSPRITEEGG_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETENHANCEATTR_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GROUPCHATLEADERREVIEWLIST_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GROUPCHATFINDROLEINFOLIST_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GROUPCHATFINDTEAMINFOLIST_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GROUPCHATPLAYERISSUEINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GROUPCHATLEADERISSUEINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GROUPCHATLEADERREVIEW_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GROUPCHATPLAYERAPPLY_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GROUPCHATGETGROUPINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GROUPCHATMANAGER_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GROUPCHATQUIT_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GROUPCHATCREATE_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETANCIENTTIMESAWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_SELECTHEROANCIENTPOWER_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GROUPCHATCLEAR_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_HOLDWEDDING_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETALLWEDDINGINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETWEDDINGINVITEINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_ENTERWEDDINGSCENE_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETMARRIAGERELATION_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_MARRIAGERELATIONOP_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_WEDDINGINVITEOPERATOR_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GROUPCHATLEADERADDROLE_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_WEDDINGOPERATOR_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_TACTENTERSCENE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GROUPCHATCHANGENAME_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_DRAGONGROUPROLELIST_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_DRAGONGROUPRECORD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETMARRIAGELIVENESS_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_TAKEMARRIAGECHEST_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_CHECKROLEBEFOREPAY_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ARTIFACTDEITYSTOVEOP_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_STARTWEDDINGCAR_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_THEMEACTIVITYHINT_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_BACKFLOWBUYOP_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_TRANSFORMOP_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_UPGRADEEQUIP_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETFASHIONSYNTHESISINFO_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_FASHIONIBSHOPBUY_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_ENTERBATTLEREADYSCENE_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETDRAGONGUILDLIVENESS_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_CREATEORJOINDRAGONGUILD_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_TAKEDRAGONGUILDCHEST_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_FETCHDGAPPS_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_CHANGEDRAGONGUILDSETTING_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_DRAGONGUILDAPPROVAL_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_LEAVEFROMDRAGONGUILD_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_MODIFYDRAGONGUILDNAME_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_ASKDRAGONGUILDMEMBERS_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_FETCHDRAGONGUILDLIST_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETDRAGONGUILDTASKINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETDRAGONGUILDTASKCHEST_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_BUYDRAGONGUILDSHOPITEM_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETDRAGONGUILDSHOP_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETDRAGONGUILDSHOPRECORD_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_CHANGEDRAGONGUILDPOSITION_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETDRAGONGUILDBINDINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_DRAGONGUILDBINDGROUPREQ_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_DRAGONGUILDJOINBINDGROUP_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_DRAGONGUILDUNBINDGROUP_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GOALAWARDSGETLIST_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GOALAWARDSGETAWARDS_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_BATTLEFIELDAWARDNUMREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_BATTLEFIELDRANKREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETWEEKLYTASKINFO_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETWEEKLYTASKREWARD_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_REFRESHWEEKLYTASK_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_BATTLEFIELDROLEAGAINSTREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETCOMPETEDRAGONINFO_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_SETPRESHOW_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_FUSEEQUIP_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_TURNOVERWEEKLYTASKITEM_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETLUCKYACTIVITYINFO_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_BUYDRAW_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_LOTTERYDRAW_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_NPCFLREQC2M_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETDAILYTASKREFRESHINFO_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETDAILYTASKASKHELP_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_GETDAILYTASKREFRESHRECORD_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2M_DAILYTASKREFRESHOPER_TYPE, FindLink("mslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_SURVIVEREQC2G_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_DOODADITEMALLSKILLREQ_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_PEERBOX_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_QUERYBOXS_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ACTIVATEPRESHOW_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GUILDPARTYSUMMONSPIRIT_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_CHOOSESPECIALEFFECTS_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_ENTERLEISURESCENE_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_BACKFLOWACTIVITYOPERATION_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_GETNEWZONEBENEFIT_TYPE, FindLink("gslink"), FindLink("clientlink"));
	RpcForward::RegistForward(RPCC2G_SELECTCHARGEBACKROLE_TYPE, FindLink("gslink"), FindLink("clientlink"));


	CProtocol::RegistProtoName(PTCC2T_KEEPALIVEPINGACK_TYPE, "PTCC2T_KEEPALIVEPINGACK");
	CProtocol::RegistProtoName(PTCG2T_OPENGATE_TYPE, "PTCG2T_OPENGATE");
	CProtocol::RegistProtoName(PTCG2T_CHANGEGSLINE_TYPE, "PTCG2T_CHANGEGSLINE");
	CProtocol::RegistProtoName(PTCN2T_KICKACCOUNT_TYPE, "PTCN2T_KICKACCOUNT");
	CProtocol::RegistProtoName(PTCM2T_CHATFORWARD_TYPE, "PTCM2T_CHATFORWARD");
	CProtocol::RegistProtoName(PTCM2T_VERSIONNOTIFY_TYPE, "PTCM2T_VERSIONNOTIFY");
	CProtocol::RegistProtoName(PTCM2T_BROADCASTM2T_TYPE, "PTCM2T_BROADCASTM2T");
	CProtocol::RegistProtoName(PTCN2T_NOTIFYGTSESSIONLOGIN_TYPE, "PTCN2T_NOTIFYGTSESSIONLOGIN");
	CProtocol::RegistProtoName(PTCG2T_BROADCASTG2T_TYPE, "PTCG2T_BROADCASTG2T");
	CProtocol::RegistProtoName(PTCM2T_NOTIFYROUTERINFO2GATE_TYPE, "PTCM2T_NOTIFYROUTERINFO2GATE");
	CProtocol::RegistProtoName(PTCN2T_SESSIONCHANGE2GT_TYPE, "PTCN2T_SESSIONCHANGE2GT");
	CProtocol::RegistProtoName(PTCT2G_REGISTSERVERNTF_TYPE, "PTCT2G_REGISTSERVERNTF");
	CProtocol::RegistProtoName(PTCT2C_LOGINCHALLENGE_TYPE, "PTCT2C_LOGINCHALLENGE");
	CProtocol::RegistProtoName(PTCT2C_KEEPALIVEPINGREQ_TYPE, "PTCT2C_KEEPALIVEPINGREQ");
	CProtocol::RegistProtoName(PTCT2C_ERRORNOTIFY_TYPE, "PTCT2C_ERRORNOTIFY");
	CProtocol::RegistProtoName(PTCT2C_CHATNOTIFY_TYPE, "PTCT2C_CHATNOTIFY");
	CProtocol::RegistProtoName(PTCT2G_RECONNECTNTF_TYPE, "PTCT2G_RECONNECTNTF");
	CProtocol::RegistProtoName(PTCT2N_LOGOUTNTF_TYPE, "PTCT2N_LOGOUTNTF");
	CProtocol::RegistProtoName(PTCG2C_NOTIFYACCOUNTDATA_TYPE, "PTCG2C_NOTIFYACCOUNTDATA");
	CProtocol::RegistProtoName(PTCC2G_ENTERSCENEREQ_TYPE, "PTCC2G_ENTERSCENEREQ");
	CProtocol::RegistProtoName(PTCG2C_ENTERSCENENTF_TYPE, "PTCG2C_ENTERSCENENTF");
	CProtocol::RegistProtoName(PTCC2G_LEAVESCENEREQ_TYPE, "PTCC2G_LEAVESCENEREQ");
	CProtocol::RegistProtoName(PTCG2C_LEAVESCENENTF_TYPE, "PTCG2C_LEAVESCENENTF");
	CProtocol::RegistProtoName(PTCC2G_MOVEOPERATIONREQ_TYPE, "PTCC2G_MOVEOPERATIONREQ");
	CProtocol::RegistProtoName(PTCG2C_UNITAPPEAR_TYPE, "PTCG2C_UNITAPPEAR");
	CProtocol::RegistProtoName(PTCG2C_UNITDISAPPEAR_TYPE, "PTCG2C_UNITDISAPPEAR");
	CProtocol::RegistProtoName(PTCC2G_CASTSKILL_TYPE, "PTCC2G_CASTSKILL");
	CProtocol::RegistProtoName(PTCG2C_SKILLRESULT_TYPE, "PTCG2C_SKILLRESULT");
	CProtocol::RegistProtoName(PTCG2C_DEATHNOTIFY_TYPE, "PTCG2C_DEATHNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_ATTRIBUTECHANGENOTIFY_TYPE, "PTCG2C_ATTRIBUTECHANGENOTIFY");
	CProtocol::RegistProtoName(PTCG2C_LEVELCHANGENTF_TYPE, "PTCG2C_LEVELCHANGENTF");
	CProtocol::RegistProtoName(PTCG2C_ITEMCHANGEDNTF_TYPE, "PTCG2C_ITEMCHANGEDNTF");
	CProtocol::RegistProtoName(PTCC2G_UPDATETUTORIAL_TYPE, "PTCC2G_UPDATETUTORIAL");
	CProtocol::RegistProtoName(PTCG2C_OPENSYSTEMNTF_TYPE, "PTCG2C_OPENSYSTEMNTF");
	CProtocol::RegistProtoName(PTCG2C_BATTLERESULTNTF_TYPE, "PTCG2C_BATTLERESULTNTF");
	CProtocol::RegistProtoName(PTCG2C_COMPLETEACHIVEMENT_TYPE, "PTCG2C_COMPLETEACHIVEMENT");
	CProtocol::RegistProtoName(PTCG2C_SKILLCHANGEDNTF_TYPE, "PTCG2C_SKILLCHANGEDNTF");
	CProtocol::RegistProtoName(PTCG2C_GSERRORNOTIFY_TYPE, "PTCG2C_GSERRORNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_CHANGEFASHIONNOTIFY_TYPE, "PTCG2C_CHANGEFASHIONNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_CHECKININFONOTIFY_TYPE, "PTCG2C_CHECKININFONOTIFY");
	CProtocol::RegistProtoName(PTCG2C_ACTIVITYROLENOTIFY_TYPE, "PTCG2C_ACTIVITYROLENOTIFY");
	CProtocol::RegistProtoName(PTCG2C_UPDATEPAYINFO_TYPE, "PTCG2C_UPDATEPAYINFO");
	CProtocol::RegistProtoName(PTCG2C_REWARDCHANGEDNTF_TYPE, "PTCG2C_REWARDCHANGEDNTF");
	CProtocol::RegistProtoName(PTCG2C_LEAVETEAM_TYPE, "PTCG2C_LEAVETEAM");
	CProtocol::RegistProtoName(PTCG2C_REVIVENOTIFY_TYPE, "PTCG2C_REVIVENOTIFY");
	CProtocol::RegistProtoName(PTCG2C_UPDATEBUYGOLDANDFATIGUEINFO_TYPE, "PTCG2C_UPDATEBUYGOLDANDFATIGUEINFO");
	CProtocol::RegistProtoName(PTCC2G_ROLEDEATHINSOLOSCENE_TYPE, "PTCC2G_ROLEDEATHINSOLOSCENE");
	CProtocol::RegistProtoName(PTCG2C_REVIVECOUNTDOWN_TYPE, "PTCG2C_REVIVECOUNTDOWN");
	CProtocol::RegistProtoName(PTCG2C_FATIGUERECOVERTIMENOTIFY_TYPE, "PTCG2C_FATIGUERECOVERTIMENOTIFY");
	CProtocol::RegistProtoName(PTCC2G_ENTERSCENECOOLDOWNQUERY_TYPE, "PTCC2G_ENTERSCENECOOLDOWNQUERY");
	CProtocol::RegistProtoName(PTCG2C_ENTERSCENECOOLDOWNNOTIFY_TYPE, "PTCG2C_ENTERSCENECOOLDOWNNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_FASHOINCHANGEDNTF_TYPE, "PTCG2C_FASHOINCHANGEDNTF");
	CProtocol::RegistProtoName(PTCC2G_OPERATERECORDNTF_TYPE, "PTCC2G_OPERATERECORDNTF");
	CProtocol::RegistProtoName(PTCG2C_GUILDNOTIFYMEMBERCHANGED_TYPE, "PTCG2C_GUILDNOTIFYMEMBERCHANGED");
	CProtocol::RegistProtoName(PTCG2C_GUILDSETTINGCHANGEDNOTIFY_TYPE, "PTCG2C_GUILDSETTINGCHANGEDNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_GUILDCHECKINBOXNTF_TYPE, "PTCG2C_GUILDCHECKINBOXNTF");
	CProtocol::RegistProtoName(PTCG2C_NEWGUILDBONUSNTF_TYPE, "PTCG2C_NEWGUILDBONUSNTF");
	CProtocol::RegistProtoName(PTCG2C_GUILDBESTCARDSNTF_TYPE, "PTCG2C_GUILDBESTCARDSNTF");
	CProtocol::RegistProtoName(PTCG2C_EXECUTELEVELSCRIPTNTF_TYPE, "PTCG2C_EXECUTELEVELSCRIPTNTF");
	CProtocol::RegistProtoName(PTCG2C_ONLINEREWARDNTF_TYPE, "PTCG2C_ONLINEREWARDNTF");
	CProtocol::RegistProtoName(PTCG2C_NEXTDAYREWARDNTF_TYPE, "PTCG2C_NEXTDAYREWARDNTF");
	CProtocol::RegistProtoName(PTCC2G_ONLINEREWARDREPORT_TYPE, "PTCC2G_ONLINEREWARDREPORT");
	CProtocol::RegistProtoName(PTCC2G_NEXTDAYREWARDREPORT_TYPE, "PTCC2G_NEXTDAYREWARDREPORT");
	CProtocol::RegistProtoName(PTCG2C_HINTNOTIFY_TYPE, "PTCG2C_HINTNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_POSITIONDEBUG_TYPE, "PTCG2C_POSITIONDEBUG");
	CProtocol::RegistProtoName(PTCG2C_ENEMYDORPDOODADNTF_TYPE, "PTCG2C_ENEMYDORPDOODADNTF");
	CProtocol::RegistProtoName(PTCG2C_TAKERANDOMTASK_TYPE, "PTCG2C_TAKERANDOMTASK");
	CProtocol::RegistProtoName(PTCG2C_BUFFNOTIFY_TYPE, "PTCG2C_BUFFNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_TEAMSELECTNOTIFY_TYPE, "PTCG2C_TEAMSELECTNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_TEAMINVITENOTIFY_TYPE, "PTCG2C_TEAMINVITENOTIFY");
	CProtocol::RegistProtoName(PTCG2C_FETCHENEMYDOODADNTF_TYPE, "PTCG2C_FETCHENEMYDOODADNTF");
	CProtocol::RegistProtoName(PTCC2G_DELAYNOTIFY_TYPE, "PTCC2G_DELAYNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_PKPREPARENTF_TYPE, "PTCG2C_PKPREPARENTF");
	CProtocol::RegistProtoName(PTCG2C_PKTIMEOUTNTF_TYPE, "PTCG2C_PKTIMEOUTNTF");
	CProtocol::RegistProtoName(PTCG2C_GUILDGOBLINKILLNTF_TYPE, "PTCG2C_GUILDGOBLINKILLNTF");
	CProtocol::RegistProtoName(PTCC2G_SKILLRESULTREQ_TYPE, "PTCC2G_SKILLRESULTREQ");
	CProtocol::RegistProtoName(PTCC2G_SKILLBULLETFIREREPORT_TYPE, "PTCC2G_SKILLBULLETFIREREPORT");
	CProtocol::RegistProtoName(PTCC2G_SKILLBULLETRESULTREQ_TYPE, "PTCC2G_SKILLBULLETRESULTREQ");
	CProtocol::RegistProtoName(PTCG2C_SCENEFINISHSTATECHANGED_TYPE, "PTCG2C_SCENEFINISHSTATECHANGED");
	CProtocol::RegistProtoName(PTCG2C_PKROLEINFONTF_TYPE, "PTCG2C_PKROLEINFONTF");
	CProtocol::RegistProtoName(PTCG2C_ITEMCIRCLEDRAWRESULT_TYPE, "PTCG2C_ITEMCIRCLEDRAWRESULT");
	CProtocol::RegistProtoName(PTCC2G_ADDLEVELSCRIPTEXTSTRING_TYPE, "PTCC2G_ADDLEVELSCRIPTEXTSTRING");
	CProtocol::RegistProtoName(PTCC2G_SCENEDAMAGERANKREPORT_TYPE, "PTCC2G_SCENEDAMAGERANKREPORT");
	CProtocol::RegistProtoName(PTCG2C_SCENEDAMAGERANKNTF_TYPE, "PTCG2C_SCENEDAMAGERANKNTF");
	CProtocol::RegistProtoName(PTCG2C_SCENELEFTDOODAD_TYPE, "PTCG2C_SCENELEFTDOODAD");
	CProtocol::RegistProtoName(PTCC2G_CHANGESUPPLEMENTREPORT_TYPE, "PTCC2G_CHANGESUPPLEMENTREPORT");
	CProtocol::RegistProtoName(PTCG2C_CHANGESUPPLEMENTNTF_TYPE, "PTCG2C_CHANGESUPPLEMENTNTF");
	CProtocol::RegistProtoName(PTCC2G_QTEOPERATION_TYPE, "PTCC2G_QTEOPERATION");
	CProtocol::RegistProtoName(PTCG2C_CORRECTPOSITION_TYPE, "PTCG2C_CORRECTPOSITION");
	CProtocol::RegistProtoName(PTCG2C_TELEPORTNOTICE_TYPE, "PTCG2C_TELEPORTNOTICE");
	CProtocol::RegistProtoName(PTCG2C_ALLYMATCHROLEIDNOTIFY_TYPE, "PTCG2C_ALLYMATCHROLEIDNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_TEAMFULLDATANTF_TYPE, "PTCG2C_TEAMFULLDATANTF");
	CProtocol::RegistProtoName(PTCG2C_PETCHANGENOTFIY_TYPE, "PTCG2C_PETCHANGENOTFIY");
	CProtocol::RegistProtoName(PTCG2C_EXPFINDBACKNTF_TYPE, "PTCG2C_EXPFINDBACKNTF");
	CProtocol::RegistProtoName(PTCC2G_AUTOFIGHTNTF_TYPE, "PTCC2G_AUTOFIGHTNTF");
	CProtocol::RegistProtoName(PTCG2C_TOWERSCENEINFONTF_TYPE, "PTCG2C_TOWERSCENEINFONTF");
	CProtocol::RegistProtoName(PTCG2C_COVERDESIGNATIONNTF_TYPE, "PTCG2C_COVERDESIGNATIONNTF");
	CProtocol::RegistProtoName(PTCG2C_REACHACHIEVENTF_TYPE, "PTCG2C_REACHACHIEVENTF");
	CProtocol::RegistProtoName(PTCG2C_REACHDESIGNATIONNTF_TYPE, "PTCG2C_REACHDESIGNATIONNTF");
	CProtocol::RegistProtoName(PTCC2G_SYNCSCENEFINISH_TYPE, "PTCC2G_SYNCSCENEFINISH");
	CProtocol::RegistProtoName(PTCG2C_LEVELSEALNTF_TYPE, "PTCG2C_LEVELSEALNTF");
	CProtocol::RegistProtoName(PTCG2C_RECONNECTSYNCNOTIFY_TYPE, "PTCG2C_RECONNECTSYNCNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_OUTLOOKCHANGENTF_TYPE, "PTCG2C_OUTLOOKCHANGENTF");
	CProtocol::RegistProtoName(PTCG2C_FIVEDAYREWARDNTF_TYPE, "PTCG2C_FIVEDAYREWARDNTF");
	CProtocol::RegistProtoName(PTCG2C_SYNCSTEPNOTIFY_TYPE, "PTCG2C_SYNCSTEPNOTIFY");
	CProtocol::RegistProtoName(PTCC2G_TSSSDKSENDANTI2SERVER_TYPE, "PTCC2G_TSSSDKSENDANTI2SERVER");
	CProtocol::RegistProtoName(PTCG2C_TSSSDKANTIDATANTF_TYPE, "PTCG2C_TSSSDKANTIDATANTF");
	CProtocol::RegistProtoName(PTCM2C_ANSWERACKNTF_TYPE, "PTCM2C_ANSWERACKNTF");
	CProtocol::RegistProtoName(PTCM2C_PUSHQUESTIONNTF_TYPE, "PTCM2C_PUSHQUESTIONNTF");
	CProtocol::RegistProtoName(PTCG2C_QANOTIFY_TYPE, "PTCG2C_QANOTIFY");
	CProtocol::RegistProtoName(PTCG2C_PVPBATTLEBEGINNTF_TYPE, "PTCG2C_PVPBATTLEBEGINNTF");
	CProtocol::RegistProtoName(PTCG2C_PVPBATTLEENDNTF_TYPE, "PTCG2C_PVPBATTLEENDNTF");
	CProtocol::RegistProtoName(PTCG2C_PVPBATTLEKILL_TYPE, "PTCG2C_PVPBATTLEKILL");
	CProtocol::RegistProtoName(PTCG2C_FLOWERRANKREWARDNTF_TYPE, "PTCG2C_FLOWERRANKREWARDNTF");
	CProtocol::RegistProtoName(PTCM2C_QAOVERNTF_TYPE, "PTCM2C_QAOVERNTF");
	CProtocol::RegistProtoName(PTCM2C_QAROOMRANKNTF_TYPE, "PTCM2C_QAROOMRANKNTF");
	CProtocol::RegistProtoName(PTCG2C_FASTMBCONFIRMNTF_TYPE, "PTCG2C_FASTMBCONFIRMNTF");
	CProtocol::RegistProtoName(PTCC2G_FMBREFUSE_TYPE, "PTCC2G_FMBREFUSE");
	CProtocol::RegistProtoName(PTCG2C_FASTMBDISMISSNTF_TYPE, "PTCG2C_FASTMBDISMISSNTF");
	CProtocol::RegistProtoName(PTCG2C_SELECTROLENTF_TYPE, "PTCG2C_SELECTROLENTF");
	CProtocol::RegistProtoName(PTCG2C_RECEIVEFLOWERNTF_TYPE, "PTCG2C_RECEIVEFLOWERNTF");
	CProtocol::RegistProtoName(PTCG2C_FLOWERRAINNTF_TYPE, "PTCG2C_FLOWERRAINNTF");
	CProtocol::RegistProtoName(PTCG2C_STARTBATTLEFAILEDNTF_TYPE, "PTCG2C_STARTBATTLEFAILEDNTF");
	CProtocol::RegistProtoName(PTCM2C_QAENTERROOMNTF_TYPE, "PTCM2C_QAENTERROOMNTF");
	CProtocol::RegistProtoName(PTCG2C_WATCHBATTLEINFONTF_TYPE, "PTCG2C_WATCHBATTLEINFONTF");
	CProtocol::RegistProtoName(PTCG2C_GMFJOINBATTLEREQ_TYPE, "PTCG2C_GMFJOINBATTLEREQ");
	CProtocol::RegistProtoName(PTCG2C_GMFONEBATTLEENDNTF_TYPE, "PTCG2C_GMFONEBATTLEENDNTF");
	CProtocol::RegistProtoName(PTCG2C_GMFBASEDATANTF_TYPE, "PTCG2C_GMFBASEDATANTF");
	CProtocol::RegistProtoName(PTCG2C_WORLDCHANNELLEFTTIMESNTF_TYPE, "PTCG2C_WORLDCHANNELLEFTTIMESNTF");
	CProtocol::RegistProtoName(PTCG2C_NOTICEGUILDBOSSEND_TYPE, "PTCG2C_NOTICEGUILDBOSSEND");
	CProtocol::RegistProtoName(PTCG2C_UPDATEGUILDARENASTATE_TYPE, "PTCG2C_UPDATEGUILDARENASTATE");
	CProtocol::RegistProtoName(PTCG2C_GUILDBOSSTIMEOUT_TYPE, "PTCG2C_GUILDBOSSTIMEOUT");
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
	CProtocol::RegistProtoName(PTCG2C_NOTIFYCLIENTENTERFIGHT_TYPE, "PTCG2C_NOTIFYCLIENTENTERFIGHT");
	CProtocol::RegistProtoName(PTCG2C_SYNGUILDARENAROLEONLINE_TYPE, "PTCG2C_SYNGUILDARENAROLEONLINE");
	CProtocol::RegistProtoName(PTCG2C_CLIENTONLYBUFFNOTIFY_TYPE, "PTCG2C_CLIENTONLYBUFFNOTIFY");
	CProtocol::RegistProtoName(PTCM2C_MCCHATOFFLINENOTIFY_TYPE, "PTCM2C_MCCHATOFFLINENOTIFY");
	CProtocol::RegistProtoName(PTCM2C_FRIENDOPNTFNEW_TYPE, "PTCM2C_FRIENDOPNTFNEW");
	CProtocol::RegistProtoName(PTCC2M_LOADOFFLINECHATNTF_TYPE, "PTCC2M_LOADOFFLINECHATNTF");
	CProtocol::RegistProtoName(PTCC2M_FRIENDQUERYREPORTNEW_TYPE, "PTCC2M_FRIENDQUERYREPORTNEW");
	CProtocol::RegistProtoName(PTCM2C_FRIENDDEGREEUPNTFNEW_TYPE, "PTCM2C_FRIENDDEGREEUPNTFNEW");
	CProtocol::RegistProtoName(PTCC2M_BLACKLISTREPORTNEW_TYPE, "PTCC2M_BLACKLISTREPORTNEW");
	CProtocol::RegistProtoName(PTCM2C_BLACKLISTNTFNEW_TYPE, "PTCM2C_BLACKLISTNTFNEW");
	CProtocol::RegistProtoName(PTCG2C_SYNGUILDARENADISPLACE_TYPE, "PTCG2C_SYNGUILDARENADISPLACE");
	CProtocol::RegistProtoName(PTCG2C_ONTRANSFERWALL_TYPE, "PTCG2C_ONTRANSFERWALL");
	CProtocol::RegistProtoName(PTCG2C_NOTIFYWATCHICONNUM2CLIENT_TYPE, "PTCG2C_NOTIFYWATCHICONNUM2CLIENT");
	CProtocol::RegistProtoName(PTCG2C_GMFWAITOTHERLOAD_TYPE, "PTCG2C_GMFWAITOTHERLOAD");
	CProtocol::RegistProtoName(PTCG2C_GMFWAITFIGHTBEGIN_TYPE, "PTCG2C_GMFWAITFIGHTBEGIN");
	CProtocol::RegistProtoName(PTCC2M_ROLESTATEREPORTNEW_TYPE, "PTCC2M_ROLESTATEREPORTNEW");
	CProtocol::RegistProtoName(PTCM2C_ROLESTATENTFNEW_TYPE, "PTCM2C_ROLESTATENTFNEW");
	CProtocol::RegistProtoName(PTCM2C_HINTNOTIFYMS_TYPE, "PTCM2C_HINTNOTIFYMS");
	CProtocol::RegistProtoName(PTCG2C_WORLDBOSSATTRNTF_TYPE, "PTCG2C_WORLDBOSSATTRNTF");
	CProtocol::RegistProtoName(PTCC2G_JOINROOM_TYPE, "PTCC2G_JOINROOM");
	CProtocol::RegistProtoName(PTCG2C_JOINROOMREPLY_TYPE, "PTCG2C_JOINROOMREPLY");
	CProtocol::RegistProtoName(PTCC2G_QUITROOM_TYPE, "PTCC2G_QUITROOM");
	CProtocol::RegistProtoName(PTCG2C_UPDATEVOIPROOMMEMBERNTF_TYPE, "PTCG2C_UPDATEVOIPROOMMEMBERNTF");
	CProtocol::RegistProtoName(PTCG2C_PAYALLINFONTF_TYPE, "PTCG2C_PAYALLINFONTF");
	CProtocol::RegistProtoName(PTCG2C_LOGINREWARD2CNTF_TYPE, "PTCG2C_LOGINREWARD2CNTF");
	CProtocol::RegistProtoName(PTCM2C_MSRECEIVEFLOWERNTF_TYPE, "PTCM2C_MSRECEIVEFLOWERNTF");
	CProtocol::RegistProtoName(PTCM2C_MSFLOWERRAINNTF_TYPE, "PTCM2C_MSFLOWERRAINNTF");
	CProtocol::RegistProtoName(PTCM2C_TEAMCHANGEM2CNTF_TYPE, "PTCM2C_TEAMCHANGEM2CNTF");
	CProtocol::RegistProtoName(PTCM2C_LEAVETEAMM2CNTF_TYPE, "PTCM2C_LEAVETEAMM2CNTF");
	CProtocol::RegistProtoName(PTCM2C_TEAMFULLDATAM2CNTF_TYPE, "PTCM2C_TEAMFULLDATAM2CNTF");
	CProtocol::RegistProtoName(PTCM2C_TEAMINVITEM2CNOTIFY_TYPE, "PTCM2C_TEAMINVITEM2CNOTIFY");
	CProtocol::RegistProtoName(PTCM2C_MSERRORNOTIFY_TYPE, "PTCM2C_MSERRORNOTIFY");
	CProtocol::RegistProtoName(PTCM2C_INVITEREFUSEM2CNTF_TYPE, "PTCM2C_INVITEREFUSEM2CNTF");
	CProtocol::RegistProtoName(PTCC2M_TEAMINVITEACKC2M_TYPE, "PTCC2M_TEAMINVITEACKC2M");
	CProtocol::RegistProtoName(PTCM2C_ROLEMATCHSTATEM2CNOTIFY_TYPE, "PTCM2C_ROLEMATCHSTATEM2CNOTIFY");
	CProtocol::RegistProtoName(PTCM2C_FASTMBCONFIRMM2CNTF_TYPE, "PTCM2C_FASTMBCONFIRMM2CNTF");
	CProtocol::RegistProtoName(PTCM2C_FASTMBDISMISSM2CNTF_TYPE, "PTCM2C_FASTMBDISMISSM2CNTF");
	CProtocol::RegistProtoName(PTCC2M_FMBREFUSEC2M_TYPE, "PTCC2M_FMBREFUSEC2M");
	CProtocol::RegistProtoName(PTCG2C_AIDEBUGINFO_TYPE, "PTCG2C_AIDEBUGINFO");
	CProtocol::RegistProtoName(PTCG2C_STARTROLLNTF_TYPE, "PTCG2C_STARTROLLNTF");
	CProtocol::RegistProtoName(PTCC2N_CHECKQUEUINGREQ_TYPE, "PTCC2N_CHECKQUEUINGREQ");
	CProtocol::RegistProtoName(PTCN2C_CHECKQUEUINGNTF_TYPE, "PTCN2C_CHECKQUEUINGNTF");
	CProtocol::RegistProtoName(PTCG2C_NOTICEHUNGRYDOWN_TYPE, "PTCG2C_NOTICEHUNGRYDOWN");
	CProtocol::RegistProtoName(PTCG2C_FIRSTPASSREWARDNTF_TYPE, "PTCG2C_FIRSTPASSREWARDNTF");
	CProtocol::RegistProtoName(PTCG2C_TITLECHANGENOTIFY_TYPE, "PTCG2C_TITLECHANGENOTIFY");
	CProtocol::RegistProtoName(PTCM2C_QAIDNAMENTF_TYPE, "PTCM2C_QAIDNAMENTF");
	CProtocol::RegistProtoName(PTCM2C_PKTIMEOUTM2CNTF_TYPE, "PTCM2C_PKTIMEOUTM2CNTF");
	CProtocol::RegistProtoName(PTCM2C_LOGINGUILDINFO_TYPE, "PTCM2C_LOGINGUILDINFO");
	CProtocol::RegistProtoName(PTCM2C_SENDGUILDSKILLINFO_TYPE, "PTCM2C_SENDGUILDSKILLINFO");
	CProtocol::RegistProtoName(PTCM2C_MSEVENTNOTIFY_TYPE, "PTCM2C_MSEVENTNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_ITEMFINDBACKNTF_TYPE, "PTCG2C_ITEMFINDBACKNTF");
	CProtocol::RegistProtoName(PTCG2C_SPRITECHANGEDNTF_TYPE, "PTCG2C_SPRITECHANGEDNTF");
	CProtocol::RegistProtoName(PTCG2C_FIGHTGROUPCHANGENTF_TYPE, "PTCG2C_FIGHTGROUPCHANGENTF");
	CProtocol::RegistProtoName(PTCG2C_BOSSRUSHONEFINISHNTF_TYPE, "PTCG2C_BOSSRUSHONEFINISHNTF");
	CProtocol::RegistProtoName(PTCG2C_SYNATLASATTR_TYPE, "PTCG2C_SYNATLASATTR");
	CProtocol::RegistProtoName(PTCG2C_PLAYDICENTF_TYPE, "PTCG2C_PLAYDICENTF");
	CProtocol::RegistProtoName(PTCC2G_PLAYDICEOVER_TYPE, "PTCC2G_PLAYDICEOVER");
	CProtocol::RegistProtoName(PTCG2C_IBSHOPICON_TYPE, "PTCG2C_IBSHOPICON");
	CProtocol::RegistProtoName(PTCC2G_REMOVEIBSHOPICON_TYPE, "PTCC2G_REMOVEIBSHOPICON");
	CProtocol::RegistProtoName(PTCG2C_NOTIFYENHANCESUIT_TYPE, "PTCG2C_NOTIFYENHANCESUIT");
	CProtocol::RegistProtoName(PTCM2C_SYNGUILDARENAFIGHTUNITNEW_TYPE, "PTCM2C_SYNGUILDARENAFIGHTUNITNEW");
	CProtocol::RegistProtoName(PTCM2C_SYNGUILDARENABATTLEINFONEW_TYPE, "PTCM2C_SYNGUILDARENABATTLEINFONEW");
	CProtocol::RegistProtoName(PTCM2C_NOTICEGUILDARENABEGINNEW_TYPE, "PTCM2C_NOTICEGUILDARENABEGINNEW");
	CProtocol::RegistProtoName(PTCM2C_SYNGUILDARENAROLEONLINENEW_TYPE, "PTCM2C_SYNGUILDARENAROLEONLINENEW");
	CProtocol::RegistProtoName(PTCM2C_SYNGUILDARENADISPLACENEW_TYPE, "PTCM2C_SYNGUILDARENADISPLACENEW");
	CProtocol::RegistProtoName(PTCG2C_RISKBUYNTF_TYPE, "PTCG2C_RISKBUYNTF");
	CProtocol::RegistProtoName(PTCC2G_SETVOIPMEMBERSTATE_TYPE, "PTCC2G_SETVOIPMEMBERSTATE");
	CProtocol::RegistProtoName(PTCG2C_RISKRECOVERDICENTF_TYPE, "PTCG2C_RISKRECOVERDICENTF");
	CProtocol::RegistProtoName(PTCM2C_NOTIFYGUILDSKILLDATA_TYPE, "PTCM2C_NOTIFYGUILDSKILLDATA");
	CProtocol::RegistProtoName(PTCC2G_OPENSPRITEEGGNTF_TYPE, "PTCC2G_OPENSPRITEEGGNTF");
	CProtocol::RegistProtoName(PTCM2C_GUILDSETINGNOTIFY_TYPE, "PTCM2C_GUILDSETINGNOTIFY");
	CProtocol::RegistProtoName(PTCM2C_GUILDBESTCARDSNTFMS_TYPE, "PTCM2C_GUILDBESTCARDSNTFMS");
	CProtocol::RegistProtoName(PTCG2C_TOWERFIRSTPASSREWARDNTF_TYPE, "PTCG2C_TOWERFIRSTPASSREWARDNTF");
	CProtocol::RegistProtoName(PTCM2C_DERANKCHANGNTF_TYPE, "PTCM2C_DERANKCHANGNTF");
	CProtocol::RegistProtoName(PTCM2C_GMFJOINBATTLEM2CREQ_TYPE, "PTCM2C_GMFJOINBATTLEM2CREQ");
	CProtocol::RegistProtoName(PTCC2M_GMFJOINBATTLEC2MRES_TYPE, "PTCC2M_GMFJOINBATTLEC2MRES");
	CProtocol::RegistProtoName(PTCG2C_HALLICONSNTF_TYPE, "PTCG2C_HALLICONSNTF");
	CProtocol::RegistProtoName(PTCG2C_UPDATETASKSTATUS_TYPE, "PTCG2C_UPDATETASKSTATUS");
	CProtocol::RegistProtoName(PTCM2C_SKYCITYFINALRES_TYPE, "PTCM2C_SKYCITYFINALRES");
	CProtocol::RegistProtoName(PTCG2C_SKYCITYESTIMATERES_TYPE, "PTCG2C_SKYCITYESTIMATERES");
	CProtocol::RegistProtoName(PTCG2C_SKYCITYTIMERES_TYPE, "PTCG2C_SKYCITYTIMERES");
	CProtocol::RegistProtoName(PTCG2C_NOTIFYIDIPMESSAGEGS_TYPE, "PTCG2C_NOTIFYIDIPMESSAGEGS");
	CProtocol::RegistProtoName(PTCM2C_NOTIFYIDIPMESSAGEMS_TYPE, "PTCM2C_NOTIFYIDIPMESSAGEMS");
	CProtocol::RegistProtoName(PTCG2C_IDIPPUNISHINFONTF_TYPE, "PTCG2C_IDIPPUNISHINFONTF");
	CProtocol::RegistProtoName(PTCM2C_IDIPPUNISHINFOMSNTF_TYPE, "PTCM2C_IDIPPUNISHINFOMSNTF");
	CProtocol::RegistProtoName(PTCM2C_IDIPCLEARCHATNTF_TYPE, "PTCM2C_IDIPCLEARCHATNTF");
	CProtocol::RegistProtoName(PTCG2C_SKYCITYTEAMRES_TYPE, "PTCG2C_SKYCITYTEAMRES");
	CProtocol::RegistProtoName(PTCC2M_GUILDCARDRANKREQ_TYPE, "PTCC2M_GUILDCARDRANKREQ");
	CProtocol::RegistProtoName(PTCM2C_GUILDCARDRANKNTF_TYPE, "PTCM2C_GUILDCARDRANKNTF");
	CProtocol::RegistProtoName(PTCM2C_NOTICEGUILDLADDERSTART_TYPE, "PTCM2C_NOTICEGUILDLADDERSTART");
	CProtocol::RegistProtoName(PTCG2C_GMFKICKNTY_TYPE, "PTCG2C_GMFKICKNTY");
	CProtocol::RegistProtoName(PTCC2M_GUILDCARDMATCHREQ_TYPE, "PTCC2M_GUILDCARDMATCHREQ");
	CProtocol::RegistProtoName(PTCM2C_GUILDCARDMATCHNTF_TYPE, "PTCM2C_GUILDCARDMATCHNTF");
	CProtocol::RegistProtoName(PTCC2M_OPENPRIVATECHATNTF_TYPE, "PTCC2M_OPENPRIVATECHATNTF");
	CProtocol::RegistProtoName(PTCG2C_SKYCITYBATTLEDATANTF_TYPE, "PTCG2C_SKYCITYBATTLEDATANTF");
	CProtocol::RegistProtoName(PTCC2M_COMMITANSWERNTF_TYPE, "PTCC2M_COMMITANSWERNTF");
	CProtocol::RegistProtoName(PTCC2M_GIVEUPQAQUESTIONNTF_TYPE, "PTCC2M_GIVEUPQAQUESTIONNTF");
	CProtocol::RegistProtoName(PTCG2C_GMFBATTLESTATENTF_TYPE, "PTCG2C_GMFBATTLESTATENTF");
	CProtocol::RegistProtoName(PTCG2C_SPACTIVITYCHANGENTF_TYPE, "PTCG2C_SPACTIVITYCHANGENTF");
	CProtocol::RegistProtoName(PTCG2C_SPACTIVITYOFFSETDAYNTF_TYPE, "PTCG2C_SPACTIVITYOFFSETDAYNTF");
	CProtocol::RegistProtoName(PTCM2C_SPRITESTATECHANGENTF_TYPE, "PTCM2C_SPRITESTATECHANGENTF");
	CProtocol::RegistProtoName(PTCM2C_INVUNFSTATEM2CNTF_TYPE, "PTCM2C_INVUNFSTATEM2CNTF");
	CProtocol::RegistProtoName(PTCG2C_DPSNOTIFY_TYPE, "PTCG2C_DPSNOTIFY");
	CProtocol::RegistProtoName(PTCM2C_NOTICEGUILDARENANEXTTIME_TYPE, "PTCM2C_NOTICEGUILDARENANEXTTIME");
	CProtocol::RegistProtoName(PTCG2C_SKILLCOOLNTF_TYPE, "PTCG2C_SKILLCOOLNTF");
	CProtocol::RegistProtoName(PTCM2C_GARDENBANQUETNTF_TYPE, "PTCM2C_GARDENBANQUETNTF");
	CProtocol::RegistProtoName(PTCG2C_GARDENPLANTEVENTNOTICE_TYPE, "PTCG2C_GARDENPLANTEVENTNOTICE");
	CProtocol::RegistProtoName(PTCG2C_GARDENBANQUETNOTICE_TYPE, "PTCG2C_GARDENBANQUETNOTICE");
	CProtocol::RegistProtoName(PTCC2M_SYNCPLATFRIEND2MS_TYPE, "PTCC2M_SYNCPLATFRIEND2MS");
	CProtocol::RegistProtoName(PTCM2C_NOTICEGUILDWAGEREWARD_TYPE, "PTCM2C_NOTICEGUILDWAGEREWARD");
	CProtocol::RegistProtoName(PTCG2C_GMFGUILDCOMBATNTF_TYPE, "PTCG2C_GMFGUILDCOMBATNTF");
	CProtocol::RegistProtoName(PTCG2C_GPRONEBATTLEENDNTF_TYPE, "PTCG2C_GPRONEBATTLEENDNTF");
	CProtocol::RegistProtoName(PTCG2C_GPRALLFIGHTENDNTF_TYPE, "PTCG2C_GPRALLFIGHTENDNTF");
	CProtocol::RegistProtoName(PTCG2C_RESWARBATTLEDATANTF_TYPE, "PTCG2C_RESWARBATTLEDATANTF");
	CProtocol::RegistProtoName(PTCG2C_RESWARTEAMRESONE_TYPE, "PTCG2C_RESWARTEAMRESONE");
	CProtocol::RegistProtoName(PTCM2C_SENDGUILDBONUSNTF_TYPE, "PTCM2C_SENDGUILDBONUSNTF");
	CProtocol::RegistProtoName(PTCC2G_BATTLELOGREPORT_TYPE, "PTCC2G_BATTLELOGREPORT");
	CProtocol::RegistProtoName(PTCM2C_RESWARGUILDBRIEFNTF_TYPE, "PTCM2C_RESWARGUILDBRIEFNTF");
	CProtocol::RegistProtoName(PTCC2G_CLICKGUILDCAMP_TYPE, "PTCC2G_CLICKGUILDCAMP");
	CProtocol::RegistProtoName(PTCG2C_PAYMEMBERPRIVILEGENTF_TYPE, "PTCG2C_PAYMEMBERPRIVILEGENTF");
	CProtocol::RegistProtoName(PTCM2C_SYNGUILINTEGRALSTATE_TYPE, "PTCM2C_SYNGUILINTEGRALSTATE");
	CProtocol::RegistProtoName(PTCM2C_RESWARSTATENTF_TYPE, "PTCM2C_RESWARSTATENTF");
	CProtocol::RegistProtoName(PTCM2C_SYNGUILDINTEGRALSTATE_TYPE, "PTCM2C_SYNGUILDINTEGRALSTATE");
	CProtocol::RegistProtoName(PTCG2C_KILLENEMYSCORENTF_TYPE, "PTCG2C_KILLENEMYSCORENTF");
	CProtocol::RegistProtoName(PTCG2C_SERVEROPENDAYNTF_TYPE, "PTCG2C_SERVEROPENDAYNTF");
	CProtocol::RegistProtoName(PTCM2C_RESWARRANKSIMPLEINFONTF_TYPE, "PTCM2C_RESWARRANKSIMPLEINFONTF");
	CProtocol::RegistProtoName(PTCM2C_GUILDBUFFSIMPLEINFONTF_TYPE, "PTCM2C_GUILDBUFFSIMPLEINFONTF");
	CProtocol::RegistProtoName(PTCC2M_QUERYRESWARREQUET_TYPE, "PTCC2M_QUERYRESWARREQUET");
	CProtocol::RegistProtoName(PTCM2C_GUILDBUFFSIMPLEITEMNTF_TYPE, "PTCM2C_GUILDBUFFSIMPLEITEMNTF");
	CProtocol::RegistProtoName(PTCC2M_GARDENFISHSTOP_TYPE, "PTCC2M_GARDENFISHSTOP");
	CProtocol::RegistProtoName(PTCM2C_HALLICONMNTF_TYPE, "PTCM2C_HALLICONMNTF");
	CProtocol::RegistProtoName(PTCG2C_SYNGUILDINHERITEXP_TYPE, "PTCG2C_SYNGUILDINHERITEXP");
	CProtocol::RegistProtoName(PTCC2G_SYNDOINGGUILDINHERIT_TYPE, "PTCC2G_SYNDOINGGUILDINHERIT");
	CProtocol::RegistProtoName(PTCG2C_NOTICEDOINGGUILDINHERIT_TYPE, "PTCG2C_NOTICEDOINGGUILDINHERIT");
	CProtocol::RegistProtoName(PTCG2C_SYNGUILDINHERITNUMINFO_TYPE, "PTCG2C_SYNGUILDINHERITNUMINFO");
	CProtocol::RegistProtoName(PTCM2C_RESWARMINEDATANTF_TYPE, "PTCM2C_RESWARMINEDATANTF");
	CProtocol::RegistProtoName(PTCG2C_CDCALL_TYPE, "PTCG2C_CDCALL");
	CProtocol::RegistProtoName(PTCC2M_LEAVELARGEROOM_TYPE, "PTCC2M_LEAVELARGEROOM");
	CProtocol::RegistProtoName(PTCM2C_LARGEROOMROLENTF_TYPE, "PTCM2C_LARGEROOMROLENTF");
	CProtocol::RegistProtoName(PTCM2C_LARGEROOMLOGINPARAMNTF_TYPE, "PTCM2C_LARGEROOMLOGINPARAMNTF");
	CProtocol::RegistProtoName(PTCM2C_RESWARTIMENTF_TYPE, "PTCM2C_RESWARTIMENTF");
	CProtocol::RegistProtoName(PTCG2C_SYNCMOVENOTIFY_TYPE, "PTCG2C_SYNCMOVENOTIFY");
	CProtocol::RegistProtoName(PTCC2M_HANDLEMICPHONE_TYPE, "PTCC2M_HANDLEMICPHONE");
	CProtocol::RegistProtoName(PTCG2C_CHANGENAMECOUNTNTF_TYPE, "PTCG2C_CHANGENAMECOUNTNTF");
	CProtocol::RegistProtoName(PTCM2C_GUILDBUFFCDPARAMNTF_TYPE, "PTCM2C_GUILDBUFFCDPARAMNTF");
	CProtocol::RegistProtoName(PTCG2C_ANTIADDICTIONREMINDNTF_TYPE, "PTCG2C_ANTIADDICTIONREMINDNTF");
	CProtocol::RegistProtoName(PTCG2C_HORSEAWARDALLNTF_TYPE, "PTCG2C_HORSEAWARDALLNTF");
	CProtocol::RegistProtoName(PTCG2C_HORSERANKNTF_TYPE, "PTCG2C_HORSERANKNTF");
	CProtocol::RegistProtoName(PTCG2C_HORSEWAITTIMENTF_TYPE, "PTCG2C_HORSEWAITTIMENTF");
	CProtocol::RegistProtoName(PTCG2C_HORSEANIMATIONNTF_TYPE, "PTCG2C_HORSEANIMATIONNTF");
	CProtocol::RegistProtoName(PTCG2C_HORSEFINALNTF_TYPE, "PTCG2C_HORSEFINALNTF");
	CProtocol::RegistProtoName(PTCC2G_PICURLNTF_TYPE, "PTCC2G_PICURLNTF");
	CProtocol::RegistProtoName(PTCM2C_NOTIFYMENTORAPPLY_TYPE, "PTCM2C_NOTIFYMENTORAPPLY");
	CProtocol::RegistProtoName(PTCC2M_POKERTOURNAMENTREQ_TYPE, "PTCC2M_POKERTOURNAMENTREQ");
	CProtocol::RegistProtoName(PTCM2C_UPDATEPARTNERTOCLIENT_TYPE, "PTCM2C_UPDATEPARTNERTOCLIENT");
	CProtocol::RegistProtoName(PTCM2C_MAKEPARTNERRESULTNTF_TYPE, "PTCM2C_MAKEPARTNERRESULTNTF");
	CProtocol::RegistProtoName(PTCM2C_INVFIGHTNOTIFY_TYPE, "PTCM2C_INVFIGHTNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_HORSECOUNTDOWNTIMENTF_TYPE, "PTCG2C_HORSECOUNTDOWNTIMENTF");
	CProtocol::RegistProtoName(PTCG2C_DOODADITEMADDNTF_TYPE, "PTCG2C_DOODADITEMADDNTF");
	CProtocol::RegistProtoName(PTCG2C_GCFZHANLINGNOTIFY_TYPE, "PTCG2C_GCFZHANLINGNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_INVFIGHTBEFENTERSCENENTF_TYPE, "PTCG2C_INVFIGHTBEFENTERSCENENTF");
	CProtocol::RegistProtoName(PTCG2C_HEROBATTLEDATANTF_TYPE, "PTCG2C_HEROBATTLEDATANTF");
	CProtocol::RegistProtoName(PTCG2C_SCENEPREPAREINFONTF_TYPE, "PTCG2C_SCENEPREPAREINFONTF");
	CProtocol::RegistProtoName(PTCG2C_HEROBATTLETEAMMSGNTF_TYPE, "PTCG2C_HEROBATTLETEAMMSGNTF");
	CProtocol::RegistProtoName(PTCG2C_HEROBATTLESYNCNTF_TYPE, "PTCG2C_HEROBATTLESYNCNTF");
	CProtocol::RegistProtoName(PTCG2C_HEROBATTLEINCIRCLENTF_TYPE, "PTCG2C_HEROBATTLEINCIRCLENTF");
	CProtocol::RegistProtoName(PTCG2C_HEROBATTLEOVERTIME_TYPE, "PTCG2C_HEROBATTLEOVERTIME");
	CProtocol::RegistProtoName(PTCC2M_PAYPARAMETERINFONTF_TYPE, "PTCC2M_PAYPARAMETERINFONTF");
	CProtocol::RegistProtoName(PTCM2C_PAYPARAMETERINFOINVALIDNTF_TYPE, "PTCM2C_PAYPARAMETERINFOINVALIDNTF");
	CProtocol::RegistProtoName(PTCG2C_HEROBATTLETEAMROLENTF_TYPE, "PTCG2C_HEROBATTLETEAMROLENTF");
	CProtocol::RegistProtoName(PTCG2C_GCFSYNG2CNTF_TYPE, "PTCG2C_GCFSYNG2CNTF");
	CProtocol::RegistProtoName(PTCM2C_NOTICEGUILDTERRWAR_TYPE, "PTCM2C_NOTICEGUILDTERRWAR");
	CProtocol::RegistProtoName(PTCM2C_NOTICEGUILDTERRALL_TYPE, "PTCM2C_NOTICEGUILDTERRALL");
	CProtocol::RegistProtoName(PTCC2N_UPDATESTARTUPTYPENTF_TYPE, "PTCC2N_UPDATESTARTUPTYPENTF");
	CProtocol::RegistProtoName(PTCG2C_DOODADITEMUSENTF_TYPE, "PTCG2C_DOODADITEMUSENTF");
	CProtocol::RegistProtoName(PTCM2C_GUILDBONUSGETALL_TYPE, "PTCM2C_GUILDBONUSGETALL");
	CProtocol::RegistProtoName(PTCG2C_SPECIALSTATENTF_TYPE, "PTCG2C_SPECIALSTATENTF");
	CProtocol::RegistProtoName(PTCM2C_RESWARENEMYTIMENTF_TYPE, "PTCM2C_RESWARENEMYTIMENTF");
	CProtocol::RegistProtoName(PTCG2C_CLOSECHOOSEHERONTF_TYPE, "PTCG2C_CLOSECHOOSEHERONTF");
	CProtocol::RegistProtoName(PTCG2C_LEVELSCRIPTSTATENTF_TYPE, "PTCG2C_LEVELSCRIPTSTATENTF");
	CProtocol::RegistProtoName(PTCG2C_SCENESTATENTF_TYPE, "PTCG2C_SCENESTATENTF");
	CProtocol::RegistProtoName(PTCC2G_QUERYROLESTATEREQ_TYPE, "PTCC2G_QUERYROLESTATEREQ");
	CProtocol::RegistProtoName(PTCG2C_QUERYROLESTATEACK_TYPE, "PTCG2C_QUERYROLESTATEACK");
	CProtocol::RegistProtoName(PTCG2C_IBSHOPHASBUYNTF_TYPE, "PTCG2C_IBSHOPHASBUYNTF");
	CProtocol::RegistProtoName(PTCM2C_UPDATELEAGUEBATTLESEASONINFO_TYPE, "PTCM2C_UPDATELEAGUEBATTLESEASONINFO");
	CProtocol::RegistProtoName(PTCG2C_SKILLINITCOOLNTF_TYPE, "PTCG2C_SKILLINITCOOLNTF");
	CProtocol::RegistProtoName(PTCM2C_NOTIFYLEAGUETEAMCREATE_TYPE, "PTCM2C_NOTIFYLEAGUETEAMCREATE");
	CProtocol::RegistProtoName(PTCM2C_NOTIFYLEAGUETEAMDISSOLVE_TYPE, "PTCM2C_NOTIFYLEAGUETEAMDISSOLVE");
	CProtocol::RegistProtoName(PTCG2C_HEROBATTLECANUSEHERO_TYPE, "PTCG2C_HEROBATTLECANUSEHERO");
	CProtocol::RegistProtoName(PTCM2C_UPDATELEAGUETEAMSTATE_TYPE, "PTCM2C_UPDATELEAGUETEAMSTATE");
	CProtocol::RegistProtoName(PTCM2C_LEAGUEBATTLESTARTMATCHNTF_TYPE, "PTCM2C_LEAGUEBATTLESTARTMATCHNTF");
	CProtocol::RegistProtoName(PTCM2C_LEAGUEBATTLESTOPMATCHNTF_TYPE, "PTCM2C_LEAGUEBATTLESTOPMATCHNTF");
	CProtocol::RegistProtoName(PTCG2C_LEAGUEBATTLELOADINFONTF_TYPE, "PTCG2C_LEAGUEBATTLELOADINFONTF");
	CProtocol::RegistProtoName(PTCG2C_LEAGUEBATTLEBASEDATANTF_TYPE, "PTCG2C_LEAGUEBATTLEBASEDATANTF");
	CProtocol::RegistProtoName(PTCG2C_LEAGUEBATTLEONERESULTNFT_TYPE, "PTCG2C_LEAGUEBATTLEONERESULTNFT");
	CProtocol::RegistProtoName(PTCG2C_LEAGUEBATTLERESULTNTF_TYPE, "PTCG2C_LEAGUEBATTLERESULTNTF");
	CProtocol::RegistProtoName(PTCG2C_PUSHPRAISENTF_TYPE, "PTCG2C_PUSHPRAISENTF");
	CProtocol::RegistProtoName(PTCG2C_HEROBATTLETIPSNTF_TYPE, "PTCG2C_HEROBATTLETIPSNTF");
	CProtocol::RegistProtoName(PTCG2C_LEAGUEBATTLESTATENTF_TYPE, "PTCG2C_LEAGUEBATTLESTATENTF");
	CProtocol::RegistProtoName(PTCM2C_LEAGUEBATTLEMATCHTIMEOUTNTF_TYPE, "PTCM2C_LEAGUEBATTLEMATCHTIMEOUTNTF");
	CProtocol::RegistProtoName(PTCC2G_CANCELREDDOTREQ_TYPE, "PTCC2G_CANCELREDDOTREQ");
	CProtocol::RegistProtoName(PTCM2C_MIDASEXCEPTIONNTF_TYPE, "PTCM2C_MIDASEXCEPTIONNTF");
	CProtocol::RegistProtoName(PTCM2C_UPDATELEAGUEELEROOMSTATENTF_TYPE, "PTCM2C_UPDATELEAGUEELEROOMSTATENTF");
	CProtocol::RegistProtoName(PTCC2M_CLOSELEAGUEELENTF_TYPE, "PTCC2M_CLOSELEAGUEELENTF");
	CProtocol::RegistProtoName(PTCM2C_KICKFMAUCHOR_TYPE, "PTCM2C_KICKFMAUCHOR");
	CProtocol::RegistProtoName(PTCM2C_GUILDAUCTITEMTIMEFRESH_TYPE, "PTCM2C_GUILDAUCTITEMTIMEFRESH");
	CProtocol::RegistProtoName(PTCM2C_POKERTOURNAMENTENDREFUND_TYPE, "PTCM2C_POKERTOURNAMENTENDREFUND");
	CProtocol::RegistProtoName(PTCM2C_NOTICEGUILDTERREND_TYPE, "PTCM2C_NOTICEGUILDTERREND");
	CProtocol::RegistProtoName(PTCM2C_NOTICEGUILDTERRBATTLEWIN_TYPE, "PTCM2C_NOTICEGUILDTERRBATTLEWIN");
	CProtocol::RegistProtoName(PTCM2C_NOTICEGUILDTERRBIGICON_TYPE, "PTCM2C_NOTICEGUILDTERRBIGICON");
	CProtocol::RegistProtoName(PTCC2G_CLOSEHINTNTF_TYPE, "PTCC2G_CLOSEHINTNTF");
	CProtocol::RegistProtoName(PTCA2C_AUDIOAIDNTF_TYPE, "PTCA2C_AUDIOAIDNTF");
	CProtocol::RegistProtoName(PTCG2C_COUNTDOWNNTF_TYPE, "PTCG2C_COUNTDOWNNTF");
	CProtocol::RegistProtoName(PTCG2C_ALLGIFTIBITEMNTF_TYPE, "PTCG2C_ALLGIFTIBITEMNTF");
	CProtocol::RegistProtoName(PTCG2C_GIFTIBBACKINFONTF_TYPE, "PTCG2C_GIFTIBBACKINFONTF");
	CProtocol::RegistProtoName(PTCG2C_SHARERANDOMGIFTNTF_TYPE, "PTCG2C_SHARERANDOMGIFTNTF");
	CProtocol::RegistProtoName(PTCM2C_MODIFYGUILDNAMENTF_TYPE, "PTCM2C_MODIFYGUILDNAMENTF");
	CProtocol::RegistProtoName(PTCG2C_WORLDBOSSGUILDADDATTRSYNCCLIENTNTF_TYPE, "PTCG2C_WORLDBOSSGUILDADDATTRSYNCCLIENTNTF");
	CProtocol::RegistProtoName(PTCG2C_MILITARYRANKNTF_TYPE, "PTCG2C_MILITARYRANKNTF");
	CProtocol::RegistProtoName(PTCG2C_LOGINACTIVITYSTATUSNTF_TYPE, "PTCG2C_LOGINACTIVITYSTATUSNTF");
	CProtocol::RegistProtoName(PTCC2G_LOGINACTIVITYAWARDREQ_TYPE, "PTCC2G_LOGINACTIVITYAWARDREQ");
	CProtocol::RegistProtoName(PTCC2M_PAYBUYGOODSFAILNTF_TYPE, "PTCC2M_PAYBUYGOODSFAILNTF");
	CProtocol::RegistProtoName(PTCC2M_IBGIFTTIPSREQ_TYPE, "PTCC2M_IBGIFTTIPSREQ");
	CProtocol::RegistProtoName(PTCM2C_ARENASTARDATANTF_TYPE, "PTCM2C_ARENASTARDATANTF");
	CProtocol::RegistProtoName(PTCM2C_NOTIFYSKYTEAMCREATE_TYPE, "PTCM2C_NOTIFYSKYTEAMCREATE");
	CProtocol::RegistProtoName(PTCM2C_SKYCRAFTMATCHNTF_TYPE, "PTCM2C_SKYCRAFTMATCHNTF");
	CProtocol::RegistProtoName(PTCM2C_SKYCRAFTELIROOMNTF_TYPE, "PTCM2C_SKYCRAFTELIROOMNTF");
	CProtocol::RegistProtoName(PTCC2M_CLOSESKYCRAFTELINTF_TYPE, "PTCC2M_CLOSESKYCRAFTELINTF");
	CProtocol::RegistProtoName(PTCG2C_DISPLAYADDITEM_TYPE, "PTCG2C_DISPLAYADDITEM");
	CProtocol::RegistProtoName(PTCG2C_UPDATEDISPLAYITEMS_TYPE, "PTCG2C_UPDATEDISPLAYITEMS");
	CProtocol::RegistProtoName(PTCG2C_GUILDCAMPPARTYNOTIFY_TYPE, "PTCG2C_GUILDCAMPPARTYNOTIFY");
	CProtocol::RegistProtoName(PTCM2C_IBGIFTICONNTF_TYPE, "PTCM2C_IBGIFTICONNTF");
	CProtocol::RegistProtoName(PTCM2C_GIFTIBSUCESSNTF_TYPE, "PTCM2C_GIFTIBSUCESSNTF");
	CProtocol::RegistProtoName(PTCG2C_ABSPARTYNTF_TYPE, "PTCG2C_ABSPARTYNTF");
	CProtocol::RegistProtoName(PTCG2C_GUILDCAMPPARTYTRADENOTIFY_TYPE, "PTCG2C_GUILDCAMPPARTYTRADENOTIFY");
	CProtocol::RegistProtoName(PTCM2C_TEAMREQUESTPLATFREIND2CLIENT_TYPE, "PTCM2C_TEAMREQUESTPLATFREIND2CLIENT");
	CProtocol::RegistProtoName(PTCG2C_CUSTOMBATTLELOADINGNTF_TYPE, "PTCG2C_CUSTOMBATTLELOADINGNTF");
	CProtocol::RegistProtoName(PTCM2C_CUSTOMBATTLEGMNOTIFY_TYPE, "PTCM2C_CUSTOMBATTLEGMNOTIFY");
	CProtocol::RegistProtoName(PTCM2C_TARJABRIEFNTF_TYPE, "PTCM2C_TARJABRIEFNTF");
	CProtocol::RegistProtoName(PTCG2C_PLATFORMSHAREAWARDNTF_TYPE, "PTCG2C_PLATFORMSHAREAWARDNTF");
	CProtocol::RegistProtoName(PTCC2G_NOTIFYPLATSHARERESULT_TYPE, "PTCC2G_NOTIFYPLATSHARERESULT");
	CProtocol::RegistProtoName(PTCG2C_WEEKEND4V4ROLEDATANTF_TYPE, "PTCG2C_WEEKEND4V4ROLEDATANTF");
	CProtocol::RegistProtoName(PTCG2C_HEROKILLNOTIFY_TYPE, "PTCG2C_HEROKILLNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_MOBABATTLETEAMROLENTF_TYPE, "PTCG2C_MOBABATTLETEAMROLENTF");
	CProtocol::RegistProtoName(PTCG2C_MOBAROLECHANGENTF_TYPE, "PTCG2C_MOBAROLECHANGENTF");
	CProtocol::RegistProtoName(PTCG2C_MOBABATTLETEAMMSGNTF_TYPE, "PTCG2C_MOBABATTLETEAMMSGNTF");
	CProtocol::RegistProtoName(PTCG2C_TRANSSKILLNOTFIY_TYPE, "PTCG2C_TRANSSKILLNOTFIY");
	CProtocol::RegistProtoName(PTCG2C_ENTITYTARGETCHANGENTF_TYPE, "PTCG2C_ENTITYTARGETCHANGENTF");
	CProtocol::RegistProtoName(PTCG2C_MOBASIGNALBROADCAST_TYPE, "PTCG2C_MOBASIGNALBROADCAST");
	CProtocol::RegistProtoName(PTCG2C_PETINVITENTF_TYPE, "PTCG2C_PETINVITENTF");
	CProtocol::RegistProtoName(PTCG2C_CHATNOTIFYG2C_TYPE, "PTCG2C_CHATNOTIFYG2C");
	CProtocol::RegistProtoName(PTCG2C_MOBAMINIMAPNTF_TYPE, "PTCG2C_MOBAMINIMAPNTF");
	CProtocol::RegistProtoName(PTCG2C_MOBAADDEXPNTF_TYPE, "PTCG2C_MOBAADDEXPNTF");
	CProtocol::RegistProtoName(PTCG2C_BMREADYTIMENTF_TYPE, "PTCG2C_BMREADYTIMENTF");
	CProtocol::RegistProtoName(PTCG2C_BMFIGHTTIMENTF_TYPE, "PTCG2C_BMFIGHTTIMENTF");
	CProtocol::RegistProtoName(PTCG2C_BMROLESCENESYNCNTF_TYPE, "PTCG2C_BMROLESCENESYNCNTF");
	CProtocol::RegistProtoName(PTCG2C_BIGMELEERELIVENTF_TYPE, "PTCG2C_BIGMELEERELIVENTF");
	CProtocol::RegistProtoName(PTCG2C_MOBAHINTNTF_TYPE, "PTCG2C_MOBAHINTNTF");
	CProtocol::RegistProtoName(PTCG2C_TAJIEHELPNOTIFY_TYPE, "PTCG2C_TAJIEHELPNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_NOTIFYSTARTUPTYPETOCLIENT_TYPE, "PTCG2C_NOTIFYSTARTUPTYPETOCLIENT");
	CProtocol::RegistProtoName(PTCG2C_HEROBATTLEANCIENTPOWER_TYPE, "PTCG2C_HEROBATTLEANCIENTPOWER");
	CProtocol::RegistProtoName(PTCM2C_GROUPCHATMANAGER_TYPE, "PTCM2C_GROUPCHATMANAGER");
	CProtocol::RegistProtoName(PTCM2C_GROUPCHATQUIT_TYPE, "PTCM2C_GROUPCHATQUIT");
	CProtocol::RegistProtoName(PTCM2C_GROUPCHATDISMISS_TYPE, "PTCM2C_GROUPCHATDISMISS");
	CProtocol::RegistProtoName(PTCM2C_WEDDINGINVITENTF_TYPE, "PTCM2C_WEDDINGINVITENTF");
	CProtocol::RegistProtoName(PTCM2C_NOTIFYMARRIAGEAPPLY_TYPE, "PTCM2C_NOTIFYMARRIAGEAPPLY");
	CProtocol::RegistProtoName(PTCC2G_BATTLESTATISTICSREPORT_TYPE, "PTCC2G_BATTLESTATISTICSREPORT");
	CProtocol::RegistProtoName(PTCG2C_BATTLESTATISTICSNTF_TYPE, "PTCG2C_BATTLESTATISTICSNTF");
	CProtocol::RegistProtoName(PTCG2C_WEDDINGLOADINFONTF_TYPE, "PTCG2C_WEDDINGLOADINFONTF");
	CProtocol::RegistProtoName(PTCG2C_WEDDINGSTATENTF_TYPE, "PTCG2C_WEDDINGSTATENTF");
	CProtocol::RegistProtoName(PTCG2C_WEDDINGEVENTNTF_TYPE, "PTCG2C_WEDDINGEVENTNTF");
	CProtocol::RegistProtoName(PTCG2C_HORSEFAILTIPSNTF_TYPE, "PTCG2C_HORSEFAILTIPSNTF");
	CProtocol::RegistProtoName(PTCM2C_NOTIFYMARRIAGEDIVORCEAPPLY_TYPE, "PTCM2C_NOTIFYMARRIAGEDIVORCEAPPLY");
	CProtocol::RegistProtoName(PTCG2C_WARNINGRANDOMNTF_TYPE, "PTCG2C_WARNINGRANDOMNTF");
	CProtocol::RegistProtoName(PTCM2C_GROUPCHATISSUECOUNT_TYPE, "PTCM2C_GROUPCHATISSUECOUNT");
	CProtocol::RegistProtoName(PTCM2C_GROUPCHATAPPLY_TYPE, "PTCM2C_GROUPCHATAPPLY");
	CProtocol::RegistProtoName(PTCC2G_SHAREREPORT_TYPE, "PTCC2G_SHAREREPORT");
	CProtocol::RegistProtoName(PTCG2C_THEMEACTIVITYCHANGENTF_TYPE, "PTCG2C_THEMEACTIVITYCHANGENTF");
	CProtocol::RegistProtoName(PTCG2C_WEDDINGCARNTF_TYPE, "PTCG2C_WEDDINGCARNTF");
	CProtocol::RegistProtoName(PTCG2C_BACKFLOWOPENNTF_TYPE, "PTCG2C_BACKFLOWOPENNTF");
	CProtocol::RegistProtoName(PTCM2C_STARTBATTLEFAILEDM2CNTF_TYPE, "PTCM2C_STARTBATTLEFAILEDM2CNTF");
	CProtocol::RegistProtoName(PTCG2C_TRANSNOTIFY_TYPE, "PTCG2C_TRANSNOTIFY");
	CProtocol::RegistProtoName(PTCC2G_INVFIGHTAGAINREQC2G_TYPE, "PTCC2G_INVFIGHTAGAINREQC2G");
	CProtocol::RegistProtoName(PTCM2C_LOGINDRAGONGUILDINFO_TYPE, "PTCM2C_LOGINDRAGONGUILDINFO");
	CProtocol::RegistProtoName(PTCM2C_DRAGONGUILDSETTINGCHANGED_TYPE, "PTCM2C_DRAGONGUILDSETTINGCHANGED");
	CProtocol::RegistProtoName(PTCM2C_DRAGONGUILDNAMENTF_TYPE, "PTCM2C_DRAGONGUILDNAMENTF");
	CProtocol::RegistProtoName(PTCC2M_GOBACKREADYSCENENTF_TYPE, "PTCC2M_GOBACKREADYSCENENTF");
	CProtocol::RegistProtoName(PTCM2C_GOALAWARDSREDPOINT_TYPE, "PTCM2C_GOALAWARDSREDPOINT");
	CProtocol::RegistProtoName(PTCG2C_BATTLEFIELDREADYINFONTF_TYPE, "PTCG2C_BATTLEFIELDREADYINFONTF");
	CProtocol::RegistProtoName(PTCG2C_BATTLEFIELDRELIVENTF_TYPE, "PTCG2C_BATTLEFIELDRELIVENTF");
	CProtocol::RegistProtoName(PTCG2C_BATTLEFIELDROLEAGAINSTNTF_TYPE, "PTCG2C_BATTLEFIELDROLEAGAINSTNTF");
	CProtocol::RegistProtoName(PTCG2C_BFFIGHTTIMENTF_TYPE, "PTCG2C_BFFIGHTTIMENTF");
	CProtocol::RegistProtoName(PTCG2C_PAYSCORENTF_TYPE, "PTCG2C_PAYSCORENTF");
	CProtocol::RegistProtoName(PTCG2C_NPCFLNTF_TYPE, "PTCG2C_NPCFLNTF");
	CProtocol::RegistProtoName(PTCM2C_DAILYTASKEVENTNTF_TYPE, "PTCM2C_DAILYTASKEVENTNTF");
	CProtocol::RegistProtoName(PTCC2G_UPDATEFREEFLOWHINTINFO_TYPE, "PTCC2G_UPDATEFREEFLOWHINTINFO");
	CProtocol::RegistProtoName(PTCG2C_DOODADITEMSKILLSNTF_TYPE, "PTCG2C_DOODADITEMSKILLSNTF");
	CProtocol::RegistProtoName(PTCC2M_MULACTIVITYICONSYSREQ_TYPE, "PTCC2M_MULACTIVITYICONSYSREQ");
	CProtocol::RegistProtoName(PTCG2C_WORDNOTIFY_TYPE, "PTCG2C_WORDNOTIFY");
	CProtocol::RegistProtoName(PTCG2C_BIGMELEEPOINTNTF_TYPE, "PTCG2C_BIGMELEEPOINTNTF");
	CProtocol::RegistProtoName(PTCG2C_BIGMELEEPOINTOUTLOOKNTF_TYPE, "PTCG2C_BIGMELEEPOINTOUTLOOKNTF");
	CProtocol::RegistProtoName(PTCM2C_TASKREFRESHNTF_TYPE, "PTCM2C_TASKREFRESHNTF");
	CProtocol::RegistProtoName(PTCG2C_KICKACCOUNTJKYDNTF_TYPE, "PTCG2C_KICKACCOUNTJKYDNTF");
	CProtocol::RegistProtoName(PTCM2C_GUILDCASTFEATSNTF_TYPE, "PTCM2C_GUILDCASTFEATSNTF");
	CProtocol::RegistProtoName(PTCG2C_WORLDLEVELNTF2CLIENT_TYPE, "PTCG2C_WORLDLEVELNTF2CLIENT");
	CProtocol::RegistProtoName(PTCG2C_UPDATESTAGEINFONTF_TYPE, "PTCG2C_UPDATESTAGEINFONTF");
	CProtocol::RegistProtoName(RPCC2T_RECONNECT_TYPE, "RPCC2T_RECONNECT");
	CProtocol::RegistProtoName(RPCC2T_CLIENTLOGINREQUEST_TYPE, "RPCC2T_CLIENTLOGINREQUEST");
	CProtocol::RegistProtoName(RPCT2M_REGISTERGT2MS_TYPE, "RPCT2M_REGISTERGT2MS");
	CProtocol::RegistProtoName(RPCG2T_REGISTERGS2GT_TYPE, "RPCG2T_REGISTERGS2GT");
	CProtocol::RegistProtoName(RPCT2G_REGISTERGT2GS_TYPE, "RPCT2G_REGISTERGT2GS");
	CProtocol::RegistProtoName(RPCT2N_LOGINREQUESTNEW_TYPE, "RPCT2N_LOGINREQUESTNEW");
	CProtocol::RegistProtoName(RPCT2G_RECONNECTGS_TYPE, "RPCT2G_RECONNECTGS");
	CProtocol::RegistProtoName(RPCT2R_REGISTERGT2ROUTER_TYPE, "RPCT2R_REGISTERGT2ROUTER");
	CProtocol::RegistProtoName(RPCT2N_REGISTERGT2NS_TYPE, "RPCT2N_REGISTERGT2NS");
	CProtocol::RegistProtoName(RPCC2T_UPLOADAUDIOTOGATE_TYPE, "RPCC2T_UPLOADAUDIOTOGATE");
	CProtocol::RegistProtoName(RPCT2A_UPLOADAUDIOTOAUDIO_TYPE, "RPCT2A_UPLOADAUDIOTOAUDIO");
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
	CProtocol::RegistProtoName(RPCC2M_FETCHMAIL_TYPE, "RPCC2M_FETCHMAIL");
	CProtocol::RegistProtoName(RPCC2M_MAILOP_TYPE, "RPCC2M_MAILOP");
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
	CProtocol::RegistProtoName(RPCC2A_GETAUDIOLISTREQ_TYPE, "RPCC2A_GETAUDIOLISTREQ");
	CProtocol::RegistProtoName(RPCC2A_UPLOADAUDIOREQ_TYPE, "RPCC2A_UPLOADAUDIOREQ");
	CProtocol::RegistProtoName(RPCC2G_ENHANCETRANSTER_TYPE, "RPCC2G_ENHANCETRANSTER");
	CProtocol::RegistProtoName(RPCC2G_PVPNOWALLDATA_TYPE, "RPCC2G_PVPNOWALLDATA");
	CProtocol::RegistProtoName(RPCC2M_GETQADATAREQ_TYPE, "RPCC2M_GETQADATAREQ");
	CProtocol::RegistProtoName(RPCC2G_GETDRAGONTOPINFO_TYPE, "RPCC2G_GETDRAGONTOPINFO");
	CProtocol::RegistProtoName(RPCC2G_TEAMINVITELISTREQ_TYPE, "RPCC2G_TEAMINVITELISTREQ");
	CProtocol::RegistProtoName(RPCC2N_CREATEROLENEW_TYPE, "RPCC2N_CREATEROLENEW");
	CProtocol::RegistProtoName(RPCC2N_SELECTROLENEW_TYPE, "RPCC2N_SELECTROLENEW");
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
	CProtocol::RegistProtoName(RPCC2N_RETURNTOSELECTROLE_TYPE, "RPCC2N_RETURNTOSELECTROLE");
	CProtocol::RegistProtoName(RPCC2G_DOENTERSCENE_TYPE, "RPCC2G_DOENTERSCENE");
	CProtocol::RegistProtoName(RPCC2G_TAKEOFFALLJADE_TYPE, "RPCC2G_TAKEOFFALLJADE");
	CProtocol::RegistProtoName(RPCC2G_SWEEPTOWER_TYPE, "RPCC2G_SWEEPTOWER");
	CProtocol::RegistProtoName(RPCC2G_RESETTOWER_TYPE, "RPCC2G_RESETTOWER");
	CProtocol::RegistProtoName(RPCC2G_GETGODDESSTRIALREWARDS_TYPE, "RPCC2G_GETGODDESSTRIALREWARDS");
	CProtocol::RegistProtoName(RPCC2M_CHAT_TYPE, "RPCC2M_CHAT");
	CProtocol::RegistProtoName(RPCC2M_DOADDFRIENDNEW_TYPE, "RPCC2M_DOADDFRIENDNEW");
	CProtocol::RegistProtoName(RPCC2M_REMOVEFRIENDNEW_TYPE, "RPCC2M_REMOVEFRIENDNEW");
	CProtocol::RegistProtoName(RPCC2M_ADDBLACKLISTNEW_TYPE, "RPCC2M_ADDBLACKLISTNEW");
	CProtocol::RegistProtoName(RPCC2M_REMOVEBLACKLISTNEW_TYPE, "RPCC2M_REMOVEBLACKLISTNEW");
	CProtocol::RegistProtoName(RPCC2M_FRIENDGIFTOPNEW_TYPE, "RPCC2M_FRIENDGIFTOPNEW");
	CProtocol::RegistProtoName(RPCC2M_RANDOMFRIENDWAITLISTNEW_TYPE, "RPCC2M_RANDOMFRIENDWAITLISTNEW");
	CProtocol::RegistProtoName(RPCC2G_QUERYPOWERPOINT_TYPE, "RPCC2G_QUERYPOWERPOINT");
	CProtocol::RegistProtoName(RPCC2M_GETWORLDBOSSSTATENEW_TYPE, "RPCC2M_GETWORLDBOSSSTATENEW");
	CProtocol::RegistProtoName(RPCC2M_GETWORLDBOSSTIMELEFT_TYPE, "RPCC2M_GETWORLDBOSSTIMELEFT");
	CProtocol::RegistProtoName(RPCC2G_GETPAYALLINFO_TYPE, "RPCC2G_GETPAYALLINFO");
	CProtocol::RegistProtoName(RPCC2G_PAYCARDAWARD_TYPE, "RPCC2G_PAYCARDAWARD");
	CProtocol::RegistProtoName(RPCC2G_PAYCLICK_TYPE, "RPCC2G_PAYCLICK");
	CProtocol::RegistProtoName(RPCC2G_GETACTIVITYINFO_TYPE, "RPCC2G_GETACTIVITYINFO");
	CProtocol::RegistProtoName(RPCC2M_SHOWFLOWERPAGENEW_TYPE, "RPCC2M_SHOWFLOWERPAGENEW");
	CProtocol::RegistProtoName(RPCC2M_MSGETFLOWERREWARDLIST_TYPE, "RPCC2M_MSGETFLOWERREWARDLIST");
	CProtocol::RegistProtoName(RPCC2M_FETCHTEAMLISTC2M_TYPE, "RPCC2M_FETCHTEAMLISTC2M");
	CProtocol::RegistProtoName(RPCC2M_INVHISTORYC2MREQ_TYPE, "RPCC2M_INVHISTORYC2MREQ");
	CProtocol::RegistProtoName(RPCC2M_TEAMREQUESTC2M_TYPE, "RPCC2M_TEAMREQUESTC2M");
	CProtocol::RegistProtoName(RPCC2G_SYNPETINFO_TYPE, "RPCC2G_SYNPETINFO");
	CProtocol::RegistProtoName(RPCC2G_CHOOSEROLLREQ_TYPE, "RPCC2G_CHOOSEROLLREQ");
	CProtocol::RegistProtoName(RPCC2G_PAYFIRSTAWARD_TYPE, "RPCC2G_PAYFIRSTAWARD");
	CProtocol::RegistProtoName(RPCC2G_GROWTHFUNDAWARD_TYPE, "RPCC2G_GROWTHFUNDAWARD");
	CProtocol::RegistProtoName(RPCC2G_BUYVIPLEVELGIFT_TYPE, "RPCC2G_BUYVIPLEVELGIFT");
	CProtocol::RegistProtoName(RPCC2G_TITLELEVELUP_TYPE, "RPCC2G_TITLELEVELUP");
	CProtocol::RegistProtoName(RPCC2G_FIRSTPASSINFOREQ_TYPE, "RPCC2G_FIRSTPASSINFOREQ");
	CProtocol::RegistProtoName(RPCC2G_GETFIRSTPASSREWARD_TYPE, "RPCC2G_GETFIRSTPASSREWARD");
	CProtocol::RegistProtoName(RPCC2G_COMMENDFIRSTPASS_TYPE, "RPCC2G_COMMENDFIRSTPASS");
	CProtocol::RegistProtoName(RPCC2G_FIRSTPASSGETTOPROLEINFO_TYPE, "RPCC2G_FIRSTPASSGETTOPROLEINFO");
	CProtocol::RegistProtoName(RPCC2M_CLIENTQUERYRANKLISTNTF_TYPE, "RPCC2M_CLIENTQUERYRANKLISTNTF");
	CProtocol::RegistProtoName(RPCC2M_ADDFRIENDNEW_TYPE, "RPCC2M_ADDFRIENDNEW");
	CProtocol::RegistProtoName(RPCC2M_PKREQC2M_TYPE, "RPCC2M_PKREQC2M");
	CProtocol::RegistProtoName(RPCC2M_CLEARPRIVATECHATLIST_TYPE, "RPCC2M_CLEARPRIVATECHATLIST");
	CProtocol::RegistProtoName(RPCC2M_ASKGUILDBRIEFINFO_TYPE, "RPCC2M_ASKGUILDBRIEFINFO");
	CProtocol::RegistProtoName(RPCC2M_CREATEORENTERGUILD_TYPE, "RPCC2M_CREATEORENTERGUILD");
	CProtocol::RegistProtoName(RPCC2M_LEAVEFROMGUILD_TYPE, "RPCC2M_LEAVEFROMGUILD");
	CProtocol::RegistProtoName(RPCC2M_AUCTIONALLREQ_TYPE, "RPCC2M_AUCTIONALLREQ");
	CProtocol::RegistProtoName(RPCC2G_CHANGEOUTLOOKOP_TYPE, "RPCC2G_CHANGEOUTLOOKOP");
	CProtocol::RegistProtoName(RPCC2G_ITEMFINDBACKINFO_TYPE, "RPCC2G_ITEMFINDBACKINFO");
	CProtocol::RegistProtoName(RPCC2G_ITEMFINDBACK_TYPE, "RPCC2G_ITEMFINDBACK");
	CProtocol::RegistProtoName(RPCC2M_REQGUILDLIST_TYPE, "RPCC2M_REQGUILDLIST");
	CProtocol::RegistProtoName(RPCC2G_ACTIVATATLAS_TYPE, "RPCC2G_ACTIVATATLAS");
	CProtocol::RegistProtoName(RPCC2G_AUTOBREAKATLAS_TYPE, "RPCC2G_AUTOBREAKATLAS");
	CProtocol::RegistProtoName(RPCC2G_BREAKATLAS_TYPE, "RPCC2G_BREAKATLAS");
	CProtocol::RegistProtoName(RPCC2G_BUYIBITEM_TYPE, "RPCC2G_BUYIBITEM");
	CProtocol::RegistProtoName(RPCC2G_QUERYIBITEM_TYPE, "RPCC2G_QUERYIBITEM");
	CProtocol::RegistProtoName(RPCC2G_SPRITEOPERATION_TYPE, "RPCC2G_SPRITEOPERATION");
	CProtocol::RegistProtoName(RPCC2G_DEPROGRESSREQ_TYPE, "RPCC2G_DEPROGRESSREQ");
	CProtocol::RegistProtoName(RPCC2M_DERANKREQ_TYPE, "RPCC2M_DERANKREQ");
	CProtocol::RegistProtoName(RPCC2M_WORLDBOSSEND_TYPE, "RPCC2M_WORLDBOSSEND");
	CProtocol::RegistProtoName(RPCC2G_GETRISKMAPINFOS_TYPE, "RPCC2G_GETRISKMAPINFOS");
	CProtocol::RegistProtoName(RPCC2G_PLAYDICEREQUEST_TYPE, "RPCC2G_PLAYDICEREQUEST");
	CProtocol::RegistProtoName(RPCC2G_CHANGERISKBOXSTATE_TYPE, "RPCC2G_CHANGERISKBOXSTATE");
	CProtocol::RegistProtoName(RPCC2G_REENTERRISKBATTLE_TYPE, "RPCC2G_REENTERRISKBATTLE");
	CProtocol::RegistProtoName(RPCC2M_REQGUILDBOSSINFO_TYPE, "RPCC2M_REQGUILDBOSSINFO");
	CProtocol::RegistProtoName(RPCC2G_LEVELSEALEXCHANGE_TYPE, "RPCC2G_LEVELSEALEXCHANGE");
	CProtocol::RegistProtoName(RPCC2M_ASKGUILDMEMBERS_TYPE, "RPCC2M_ASKGUILDMEMBERS");
	CProtocol::RegistProtoName(RPCC2G_PLAYDICEOVER_TYPE, "RPCC2G_PLAYDICEOVER");
	CProtocol::RegistProtoName(RPCC2M_ASKGUILDSKILLINFONEW_TYPE, "RPCC2M_ASKGUILDSKILLINFONEW");
	CProtocol::RegistProtoName(RPCC2M_STUDYGUILDSKILLNEW_TYPE, "RPCC2M_STUDYGUILDSKILLNEW");
	CProtocol::RegistProtoName(RPCC2G_RISKBUYREQUEST_TYPE, "RPCC2G_RISKBUYREQUEST");
	CProtocol::RegistProtoName(RPCC2G_SMELTITEM_TYPE, "RPCC2G_SMELTITEM");
	CProtocol::RegistProtoName(RPCC2G_REFRESHSWEEPREWARD_TYPE, "RPCC2G_REFRESHSWEEPREWARD");
	CProtocol::RegistProtoName(RPCC2G_GETSWEEPTOWERREWARD_TYPE, "RPCC2G_GETSWEEPTOWERREWARD");
	CProtocol::RegistProtoName(RPCC2G_LEARNGUILDSKILL_TYPE, "RPCC2G_LEARNGUILDSKILL");
	CProtocol::RegistProtoName(RPCC2M_MTSHOWTOPLIST_TYPE, "RPCC2M_MTSHOWTOPLIST");
	CProtocol::RegistProtoName(RPCC2M_GETGUILDBOSSTIMELEFT_TYPE, "RPCC2M_GETGUILDBOSSTIMELEFT");
	CProtocol::RegistProtoName(RPCC2M_FETCHGUILDAPP_TYPE, "RPCC2M_FETCHGUILDAPP");
	CProtocol::RegistProtoName(RPCC2M_GUILDAPPROVALNEW_TYPE, "RPCC2M_GUILDAPPROVALNEW");
	CProtocol::RegistProtoName(RPCC2M_CHANGEGUILDSETTINGNEW_TYPE, "RPCC2M_CHANGEGUILDSETTINGNEW");
	CProtocol::RegistProtoName(RPCC2G_QUERYSCENETIME_TYPE, "RPCC2G_QUERYSCENETIME");
	CProtocol::RegistProtoName(RPCC2M_GETGUILDCHECKINRECORDSNEW_TYPE, "RPCC2M_GETGUILDCHECKINRECORDSNEW");
	CProtocol::RegistProtoName(RPCC2M_GUILDCHECKINNEW_TYPE, "RPCC2M_GUILDCHECKINNEW");
	CProtocol::RegistProtoName(RPCC2M_QUERYGUILDCHECKINNEW_TYPE, "RPCC2M_QUERYGUILDCHECKINNEW");
	CProtocol::RegistProtoName(RPCC2G_GETTOWERFIRSTPASSREWARD_TYPE, "RPCC2G_GETTOWERFIRSTPASSREWARD");
	CProtocol::RegistProtoName(RPCC2M_GUILDFATIGUEOPNEW_TYPE, "RPCC2M_GUILDFATIGUEOPNEW");
	CProtocol::RegistProtoName(RPCC2M_FETCHGUILDHISTORYNEW_TYPE, "RPCC2M_FETCHGUILDHISTORYNEW");
	CProtocol::RegistProtoName(RPCC2M_CHANGEMEMBERPOSITIONNEW_TYPE, "RPCC2M_CHANGEMEMBERPOSITIONNEW");
	CProtocol::RegistProtoName(RPCC2M_GETOTHERGUILDBRIEFNEW_TYPE, "RPCC2M_GETOTHERGUILDBRIEFNEW");
	CProtocol::RegistProtoName(RPCC2M_ASKGUILDARENAINFONEW_TYPE, "RPCC2M_ASKGUILDARENAINFONEW");
	CProtocol::RegistProtoName(RPCC2M_ASKGUILDARENATEAMINFONEW_TYPE, "RPCC2M_ASKGUILDARENATEAMINFONEW");
	CProtocol::RegistProtoName(RPCC2M_ADJUSTGUILDARENAROLEPOSNEW_TYPE, "RPCC2M_ADJUSTGUILDARENAROLEPOSNEW");
	CProtocol::RegistProtoName(RPCC2M_GUILDARENASIMPLEDEPLOYNEW_TYPE, "RPCC2M_GUILDARENASIMPLEDEPLOYNEW");
	CProtocol::RegistProtoName(RPCC2M_REQGUILDLADDERINFO_TYPE, "RPCC2M_REQGUILDLADDERINFO");
	CProtocol::RegistProtoName(RPCC2M_REQGUILDLADDERRNAKINFO_TYPE, "RPCC2M_REQGUILDLADDERRNAKINFO");
	CProtocol::RegistProtoName(RPCC2M_STARTPLANT_TYPE, "RPCC2M_STARTPLANT");
	CProtocol::RegistProtoName(RPCC2M_FETCHPLANTINFO_TYPE, "RPCC2M_FETCHPLANTINFO");
	CProtocol::RegistProtoName(RPCC2M_PLANTCULTIVATION_TYPE, "RPCC2M_PLANTCULTIVATION");
	CProtocol::RegistProtoName(RPCC2M_PLANTHARVEST_TYPE, "RPCC2M_PLANTHARVEST");
	CProtocol::RegistProtoName(RPCC2M_TRYFISH_TYPE, "RPCC2M_TRYFISH");
	CProtocol::RegistProtoName(RPCC2M_FRIENDGARDENPLANTLOG_TYPE, "RPCC2M_FRIENDGARDENPLANTLOG");
	CProtocol::RegistProtoName(RPCC2M_GARDENCOOKINGFOOD_TYPE, "RPCC2M_GARDENCOOKINGFOOD");
	CProtocol::RegistProtoName(RPCC2M_SKYCITYENTER_TYPE, "RPCC2M_SKYCITYENTER");
	CProtocol::RegistProtoName(RPCC2G_GMFREADYREQ_TYPE, "RPCC2G_GMFREADYREQ");
	CProtocol::RegistProtoName(RPCC2G_INSPIREREQ_TYPE, "RPCC2G_INSPIREREQ");
	CProtocol::RegistProtoName(RPCC2G_REQGUILDBOSSTIMELEFT_TYPE, "RPCC2G_REQGUILDBOSSTIMELEFT");
	CProtocol::RegistProtoName(RPCC2G_SKYCITYALLINFOREQ_TYPE, "RPCC2G_SKYCITYALLINFOREQ");
	CProtocol::RegistProtoName(RPCC2M_REQGUILDRANKINFO_TYPE, "RPCC2M_REQGUILDRANKINFO");
	CProtocol::RegistProtoName(RPCC2M_GETGUILDBINDINFO_TYPE, "RPCC2M_GETGUILDBINDINFO");
	CProtocol::RegistProtoName(RPCC2M_GUILDBINDGROUP_TYPE, "RPCC2M_GUILDBINDGROUP");
	CProtocol::RegistProtoName(RPCC2M_GUILDJOINBINDGROUP_TYPE, "RPCC2M_GUILDJOINBINDGROUP");
	CProtocol::RegistProtoName(RPCC2M_GUILDUNBINDGROUP_TYPE, "RPCC2M_GUILDUNBINDGROUP");
	CProtocol::RegistProtoName(RPCC2M_GMFJOINREQ_TYPE, "RPCC2M_GMFJOINREQ");
	CProtocol::RegistProtoName(RPCC2G_GETSPACTIVITYREWARD_TYPE, "RPCC2G_GETSPACTIVITYREWARD");
	CProtocol::RegistProtoName(RPCC2G_GETSPACTIVITYBIGPRIZE_TYPE, "RPCC2G_GETSPACTIVITYBIGPRIZE");
	CProtocol::RegistProtoName(RPCC2M_GARDENBANQUET_TYPE, "RPCC2M_GARDENBANQUET");
	CProtocol::RegistProtoName(RPCC2M_GARDENOVERVIEW_TYPE, "RPCC2M_GARDENOVERVIEW");
	CProtocol::RegistProtoName(RPCC2G_GETLEVELSEALSELFGIFT_TYPE, "RPCC2G_GETLEVELSEALSELFGIFT");
	CProtocol::RegistProtoName(RPCC2M_GARDENBANQUETAWARD_TYPE, "RPCC2M_GARDENBANQUETAWARD");
	CProtocol::RegistProtoName(RPCC2M_ACTIVECOOKBOOK_TYPE, "RPCC2M_ACTIVECOOKBOOK");
	CProtocol::RegistProtoName(RPCC2M_GARDENFISHINFO_TYPE, "RPCC2M_GARDENFISHINFO");
	CProtocol::RegistProtoName(RPCC2M_PAYNOTIFY_TYPE, "RPCC2M_PAYNOTIFY");
	CProtocol::RegistProtoName(RPCC2M_GARDENSTEAL_TYPE, "RPCC2M_GARDENSTEAL");
	CProtocol::RegistProtoName(RPCC2M_GARDENEXPELSPRITE_TYPE, "RPCC2M_GARDENEXPELSPRITE");
	CProtocol::RegistProtoName(RPCC2M_REQPLATFRIENDRANKLIST_TYPE, "RPCC2M_REQPLATFRIENDRANKLIST");
	CProtocol::RegistProtoName(RPCC2M_SENDGIFT2PLATFRIEND_TYPE, "RPCC2M_SENDGIFT2PLATFRIEND");
	CProtocol::RegistProtoName(RPCC2M_ASKGUILDWAGEINFO_TYPE, "RPCC2M_ASKGUILDWAGEINFO");
	CProtocol::RegistProtoName(RPCC2M_GETGUILDWAGEREWARD_TYPE, "RPCC2M_GETGUILDWAGEREWARD");
	CProtocol::RegistProtoName(RPCC2M_OPENGARDENFARMLAND_TYPE, "RPCC2M_OPENGARDENFARMLAND");
	CProtocol::RegistProtoName(RPCC2M_RESWAREXPLORE_TYPE, "RPCC2M_RESWAREXPLORE");
	CProtocol::RegistProtoName(RPCC2M_QUERYRESWAR_TYPE, "RPCC2M_QUERYRESWAR");
	CProtocol::RegistProtoName(RPCC2G_GOLDCLICK_TYPE, "RPCC2G_GOLDCLICK");
	CProtocol::RegistProtoName(RPCC2M_GUILDAUCTREQALL_TYPE, "RPCC2M_GUILDAUCTREQALL");
	CProtocol::RegistProtoName(RPCC2M_GETGUILDBONUSSENDLIST_TYPE, "RPCC2M_GETGUILDBONUSSENDLIST");
	CProtocol::RegistProtoName(RPCC2M_SENDGUILDBONUSINSENDLIST_TYPE, "RPCC2M_SENDGUILDBONUSINSENDLIST");
	CProtocol::RegistProtoName(RPCC2M_USEGUILDBUFF_TYPE, "RPCC2M_USEGUILDBUFF");
	CProtocol::RegistProtoName(RPCC2G_RESWARALLINFOREQONE_TYPE, "RPCC2G_RESWARALLINFOREQONE");
	CProtocol::RegistProtoName(RPCC2M_STARTRESWARPVE_TYPE, "RPCC2M_STARTRESWARPVE");
	CProtocol::RegistProtoName(RPCC2M_APPLYGUILDARENA_TYPE, "RPCC2M_APPLYGUILDARENA");
	CProtocol::RegistProtoName(RPCC2M_GETAPPLYGUILDLIST_TYPE, "RPCC2M_GETAPPLYGUILDLIST");
	CProtocol::RegistProtoName(RPCC2M_GETINTEGRALBATTLEINFO_TYPE, "RPCC2M_GETINTEGRALBATTLEINFO");
	CProtocol::RegistProtoName(RPCC2M_GETGUILDINTEGRALINFO_TYPE, "RPCC2M_GETGUILDINTEGRALINFO");
	CProtocol::RegistProtoName(RPCC2M_GUILDCAMPINFO_TYPE, "RPCC2M_GUILDCAMPINFO");
	CProtocol::RegistProtoName(RPCC2M_QUERYRESWARROLERANK_TYPE, "RPCC2M_QUERYRESWARROLERANK");
	CProtocol::RegistProtoName(RPCC2G_ITEMBUFFOP_TYPE, "RPCC2G_ITEMBUFFOP");
	CProtocol::RegistProtoName(RPCC2M_REQGUILDARENAHISTORY_TYPE, "RPCC2M_REQGUILDARENAHISTORY");
	CProtocol::RegistProtoName(RPCC2G_JUSTDANCE_TYPE, "RPCC2G_JUSTDANCE");
	CProtocol::RegistProtoName(RPCC2G_GETDAILYTASKINFO_TYPE, "RPCC2G_GETDAILYTASKINFO");
	CProtocol::RegistProtoName(RPCC2G_GETDAILYTASKREWARD_TYPE, "RPCC2G_GETDAILYTASKREWARD");
	CProtocol::RegistProtoName(RPCC2G_DAILYTASKASKHELP_TYPE, "RPCC2G_DAILYTASKASKHELP");
	CProtocol::RegistProtoName(RPCC2M_GETGUILDDONATEINFO_TYPE, "RPCC2M_GETGUILDDONATEINFO");
	CProtocol::RegistProtoName(RPCC2M_DONATEMEMBERITEM_TYPE, "RPCC2M_DONATEMEMBERITEM");
	CProtocol::RegistProtoName(RPCC2M_ADDGUILDINHERIT_TYPE, "RPCC2M_ADDGUILDINHERIT");
	CProtocol::RegistProtoName(RPCC2M_ACEPTGUILDINHERIT_TYPE, "RPCC2M_ACEPTGUILDINHERIT");
	CProtocol::RegistProtoName(RPCC2M_REQGUILDINHERITINFO_TYPE, "RPCC2M_REQGUILDINHERITINFO");
	CProtocol::RegistProtoName(RPCC2M_DELGUILDINHERIT_TYPE, "RPCC2M_DELGUILDINHERIT");
	CProtocol::RegistProtoName(RPCC2G_QUERYQQVIPINFO_TYPE, "RPCC2G_QUERYQQVIPINFO");
	CProtocol::RegistProtoName(RPCC2G_DAILYTASKGIVEUP_TYPE, "RPCC2G_DAILYTASKGIVEUP");
	CProtocol::RegistProtoName(RPCC2M_FETCHPLATNOTICE_TYPE, "RPCC2M_FETCHPLATNOTICE");
	CProtocol::RegistProtoName(RPCC2M_JOINFMROOM_TYPE, "RPCC2M_JOINFMROOM");
	CProtocol::RegistProtoName(RPCC2G_RESWARBUFF_TYPE, "RPCC2G_RESWARBUFF");
	CProtocol::RegistProtoName(RPCC2M_CHANGENAMENEW_TYPE, "RPCC2M_CHANGENAMENEW");
	CProtocol::RegistProtoName(RPCC2G_QUERYQQFRIENDSVIPINFO_TYPE, "RPCC2G_QUERYQQFRIENDSVIPINFO");
	CProtocol::RegistProtoName(RPCC2G_ENCHANTEQUIP_TYPE, "RPCC2G_ENCHANTEQUIP");
	CProtocol::RegistProtoName(RPCC2M_GETPARTNERINFO_TYPE, "RPCC2M_GETPARTNERINFO");
	CProtocol::RegistProtoName(RPCC2M_GETOTHERMENTORSTATUS_TYPE, "RPCC2M_GETOTHERMENTORSTATUS");
	CProtocol::RegistProtoName(RPCC2M_GETMYMENTORINFO_TYPE, "RPCC2M_GETMYMENTORINFO");
	CProtocol::RegistProtoName(RPCC2M_GETMYAPPLYSTUDENTINFO_TYPE, "RPCC2M_GETMYAPPLYSTUDENTINFO");
	CProtocol::RegistProtoName(RPCC2M_GETMYAPPLYMASTERINFO_TYPE, "RPCC2M_GETMYAPPLYMASTERINFO");
	CProtocol::RegistProtoName(RPCC2M_MENTORMYBEAPPLIEDMSG_TYPE, "RPCC2M_MENTORMYBEAPPLIEDMSG");
	CProtocol::RegistProtoName(RPCC2M_UPDATEMENTORAPPLYSTUDENTINFO_TYPE, "RPCC2M_UPDATEMENTORAPPLYSTUDENTINFO");
	CProtocol::RegistProtoName(RPCC2M_MENTORRELATIONOP_TYPE, "RPCC2M_MENTORRELATIONOP");
	CProtocol::RegistProtoName(RPCC2G_TAKEPARTNERCHEST_TYPE, "RPCC2G_TAKEPARTNERCHEST");
	CProtocol::RegistProtoName(RPCC2M_GETPARTNERDETAILINFO_TYPE, "RPCC2M_GETPARTNERDETAILINFO");
	CProtocol::RegistProtoName(RPCC2M_GETPARTNERLIVENESS_TYPE, "RPCC2M_GETPARTNERLIVENESS");
	CProtocol::RegistProtoName(RPCC2M_LEAVEPARTNER_TYPE, "RPCC2M_LEAVEPARTNER");
	CProtocol::RegistProtoName(RPCC2G_GETHEROBATTLEINFO_TYPE, "RPCC2G_GETHEROBATTLEINFO");
	CProtocol::RegistProtoName(RPCC2M_CANCELLEAVEPARTNER_TYPE, "RPCC2M_CANCELLEAVEPARTNER");
	CProtocol::RegistProtoName(RPCC2M_INVFIGHTREQALL_TYPE, "RPCC2M_INVFIGHTREQALL");
	CProtocol::RegistProtoName(RPCC2G_HORSERECONNECT_TYPE, "RPCC2G_HORSERECONNECT");
	CProtocol::RegistProtoName(RPCC2G_PHOTOGRAPHEFFECT_TYPE, "RPCC2G_PHOTOGRAPHEFFECT");
	CProtocol::RegistProtoName(RPCC2M_GCFCOMMONREQ_TYPE, "RPCC2M_GCFCOMMONREQ");
	CProtocol::RegistProtoName(RPCC2M_GCFREADYSINFOREQ_TYPE, "RPCC2M_GCFREADYSINFOREQ");
	CProtocol::RegistProtoName(RPCC2M_GETPARTNERSHOP_TYPE, "RPCC2M_GETPARTNERSHOP");
	CProtocol::RegistProtoName(RPCC2M_BUYPARTNERSHOPITEM_TYPE, "RPCC2M_BUYPARTNERSHOPITEM");
	CProtocol::RegistProtoName(RPCC2M_REQGUILDTERRCITYINFO_TYPE, "RPCC2M_REQGUILDTERRCITYINFO");
	CProtocol::RegistProtoName(RPCC2M_REQGUILDTERRCHALLINFO_TYPE, "RPCC2M_REQGUILDTERRCHALLINFO");
	CProtocol::RegistProtoName(RPCC2M_REQGUILDTERRALLIANCEINFO_TYPE, "RPCC2M_REQGUILDTERRALLIANCEINFO");
	CProtocol::RegistProtoName(RPCC2M_GETPARTNERSHOPRECORD_TYPE, "RPCC2M_GETPARTNERSHOPRECORD");
	CProtocol::RegistProtoName(RPCC2G_PANDORALOTTERY_TYPE, "RPCC2G_PANDORALOTTERY");
	CProtocol::RegistProtoName(RPCC2M_ALLIANCEGUILDTERR_TYPE, "RPCC2M_ALLIANCEGUILDTERR");
	CProtocol::RegistProtoName(RPCC2M_GCFFIGHTINFOREQC2M_TYPE, "RPCC2M_GCFFIGHTINFOREQC2M");
	CProtocol::RegistProtoName(RPCC2M_RECALLIANCE_TYPE, "RPCC2M_RECALLIANCE");
	CProtocol::RegistProtoName(RPCC2M_TRYALLIANCE_TYPE, "RPCC2M_TRYALLIANCE");
	CProtocol::RegistProtoName(RPCC2A_AUDIOAUTHKEY_TYPE, "RPCC2A_AUDIOAUTHKEY");
	CProtocol::RegistProtoName(RPCC2A_AUDIOTEXT_TYPE, "RPCC2A_AUDIOTEXT");
	CProtocol::RegistProtoName(RPCC2M_CLEARGUILDTERRALLIANCE_TYPE, "RPCC2M_CLEARGUILDTERRALLIANCE");
	CProtocol::RegistProtoName(RPCC2G_QUERYCLIENTIP_TYPE, "RPCC2G_QUERYCLIENTIP");
	CProtocol::RegistProtoName(RPCC2G_SETHEROINHEROBATTLE_TYPE, "RPCC2G_SETHEROINHEROBATTLE");
	CProtocol::RegistProtoName(RPCC2G_GETGUILDBONUSLEFT_TYPE, "RPCC2G_GETGUILDBONUSLEFT");
	CProtocol::RegistProtoName(RPCC2M_GETUNITAPPEARANCENEW_TYPE, "RPCC2M_GETUNITAPPEARANCENEW");
	CProtocol::RegistProtoName(RPCC2G_GETHEROBATTLEGAMERECORD_TYPE, "RPCC2G_GETHEROBATTLEGAMERECORD");
	CProtocol::RegistProtoName(RPCC2G_BUYHEROINHEROBATTLE_TYPE, "RPCC2G_BUYHEROINHEROBATTLE");
	CProtocol::RegistProtoName(RPCC2G_GETHEROBATTLEWEEKREWARD_TYPE, "RPCC2G_GETHEROBATTLEWEEKREWARD");
	CProtocol::RegistProtoName(RPCC2M_GETLEAGUEBATTLEINFO_TYPE, "RPCC2M_GETLEAGUEBATTLEINFO");
	CProtocol::RegistProtoName(RPCC2M_GETLEAGUETEAMINFO_TYPE, "RPCC2M_GETLEAGUETEAMINFO");
	CProtocol::RegistProtoName(RPCC2M_LEAVELEAGUETEAM_TYPE, "RPCC2M_LEAVELEAGUETEAM");
	CProtocol::RegistProtoName(RPCC2M_LEAGUEBATTLEREQ_TYPE, "RPCC2M_LEAGUEBATTLEREQ");
	CProtocol::RegistProtoName(RPCC2G_ATLASUPSTAR_TYPE, "RPCC2G_ATLASUPSTAR");
	CProtocol::RegistProtoName(RPCC2G_LEAGUEBATTLEREADYREQ_TYPE, "RPCC2G_LEAGUEBATTLEREADYREQ");
	CProtocol::RegistProtoName(RPCC2G_FORGEEQUIP_TYPE, "RPCC2G_FORGEEQUIP");
	CProtocol::RegistProtoName(RPCC2M_GETLISTSUBSCRIBE_TYPE, "RPCC2M_GETLISTSUBSCRIBE");
	CProtocol::RegistProtoName(RPCC2M_SETSUBSCRIBE_TYPE, "RPCC2M_SETSUBSCRIBE");
	CProtocol::RegistProtoName(RPCC2G_CHANGEPROFESSION_TYPE, "RPCC2G_CHANGEPROFESSION");
	CProtocol::RegistProtoName(RPCC2G_GETHOLIDAYSTAGEINFO_TYPE, "RPCC2G_GETHOLIDAYSTAGEINFO");
	CProtocol::RegistProtoName(RPCC2M_GETLEAGUEBATTLERECORD_TYPE, "RPCC2M_GETLEAGUEBATTLERECORD");
	CProtocol::RegistProtoName(RPCC2G_ITEMSELL_TYPE, "RPCC2G_ITEMSELL");
	CProtocol::RegistProtoName(RPCC2G_ENCHANTTRANSFER_TYPE, "RPCC2G_ENCHANTTRANSFER");
	CProtocol::RegistProtoName(RPCC2M_GETLEAGUEELEINFO_TYPE, "RPCC2M_GETLEAGUEELEINFO");
	CProtocol::RegistProtoName(RPCC2M_JOINLEAGUEELEBATTLE_TYPE, "RPCC2M_JOINLEAGUEELEBATTLE");
	CProtocol::RegistProtoName(RPCC2G_PERSONALCAREER_TYPE, "RPCC2G_PERSONALCAREER");
	CProtocol::RegistProtoName(RPCC2G_CHANGEDECLARATION_TYPE, "RPCC2G_CHANGEDECLARATION");
	CProtocol::RegistProtoName(RPCC2M_CLICKNEWNOTICE_TYPE, "RPCC2M_CLICKNEWNOTICE");
	CProtocol::RegistProtoName(RPCC2G_CHANGESKILLSET_TYPE, "RPCC2G_CHANGESKILLSET");
	CProtocol::RegistProtoName(RPCC2G_GETPAYREWARD_TYPE, "RPCC2G_GETPAYREWARD");
	CProtocol::RegistProtoName(RPCC2M_REQGUILDTERRINTELLINFO_TYPE, "RPCC2M_REQGUILDTERRINTELLINFO");
	CProtocol::RegistProtoName(RPCC2G_PAYFRIENDITEM_TYPE, "RPCC2G_PAYFRIENDITEM");
	CProtocol::RegistProtoName(RPCC2G_ITEMCOMPOSE_TYPE, "RPCC2G_ITEMCOMPOSE");
	CProtocol::RegistProtoName(RPCC2M_QUERYRANKLISTLUA_TYPE, "RPCC2M_QUERYRANKLISTLUA");
	CProtocol::RegistProtoName(RPCC2G_GETBACKFLOWSPACTTIME_TYPE, "RPCC2G_GETBACKFLOWSPACTTIME");
	CProtocol::RegistProtoName(RPCC2M_WORLDBOSSGUILDADDATTR_TYPE, "RPCC2M_WORLDBOSSGUILDADDATTR");
	CProtocol::RegistProtoName(RPCC2G_ARGENTAACTIVITY_TYPE, "RPCC2G_ARGENTAACTIVITY");
	CProtocol::RegistProtoName(RPCC2M_MODIFYMSGUILDNAME_TYPE, "RPCC2M_MODIFYMSGUILDNAME");
	CProtocol::RegistProtoName(RPCC2G_GETPLATFORMSHARECHEST_TYPE, "RPCC2G_GETPLATFORMSHARECHEST");
	CProtocol::RegistProtoName(RPCC2G_LOGINACTIVITY_TYPE, "RPCC2G_LOGINACTIVITY");
	CProtocol::RegistProtoName(RPCC2G_GETGROWTHFUNDTIME_TYPE, "RPCC2G_GETGROWTHFUNDTIME");
	CProtocol::RegistProtoName(RPCC2M_ARENASTARROLEREQ_TYPE, "RPCC2M_ARENASTARROLEREQ");
	CProtocol::RegistProtoName(RPCC2M_LEAVESKYTEAM_TYPE, "RPCC2M_LEAVESKYTEAM");
	CProtocol::RegistProtoName(RPCC2M_SKYCRAFTMATCHREQ_TYPE, "RPCC2M_SKYCRAFTMATCHREQ");
	CProtocol::RegistProtoName(RPCC2M_CUSTOMBATTLEOP_TYPE, "RPCC2M_CUSTOMBATTLEOP");
	CProtocol::RegistProtoName(RPCC2M_GETSKYCRAFTINFO_TYPE, "RPCC2M_GETSKYCRAFTINFO");
	CProtocol::RegistProtoName(RPCC2M_GETSKYCRAFTRECORD_TYPE, "RPCC2M_GETSKYCRAFTRECORD");
	CProtocol::RegistProtoName(RPCC2M_GETSKYCRAFTELIINFO_TYPE, "RPCC2M_GETSKYCRAFTELIINFO");
	CProtocol::RegistProtoName(RPCC2M_GETSKYCRAFTTEAMINFO_TYPE, "RPCC2M_GETSKYCRAFTTEAMINFO");
	CProtocol::RegistProtoName(RPCC2M_IBGIFTHISTREQ_TYPE, "RPCC2M_IBGIFTHISTREQ");
	CProtocol::RegistProtoName(RPCC2G_ACTIVATEFASHIONCHARM_TYPE, "RPCC2G_ACTIVATEFASHIONCHARM");
	CProtocol::RegistProtoName(RPCC2G_GETGUILDCAMPARTYRANDITEM_TYPE, "RPCC2G_GETGUILDCAMPARTYRANDITEM");
	CProtocol::RegistProtoName(RPCC2G_TALKGUILDCAMPSPRITE_TYPE, "RPCC2G_TALKGUILDCAMPSPRITE");
	CProtocol::RegistProtoName(RPCC2G_GUILDCAMPPARTYREQEXCHANGE_TYPE, "RPCC2G_GUILDCAMPPARTYREQEXCHANGE");
	CProtocol::RegistProtoName(RPCC2G_GETGUILDCAMPPARTYEXCHANGEINFO_TYPE, "RPCC2G_GETGUILDCAMPPARTYEXCHANGEINFO");
	CProtocol::RegistProtoName(RPCC2G_GUILDCAMPEXCHANGEOPERATE_TYPE, "RPCC2G_GUILDCAMPEXCHANGEOPERATE");
	CProtocol::RegistProtoName(RPCC2G_GETGUILDCAMPPARTYREWARD_TYPE, "RPCC2G_GETGUILDCAMPPARTYREWARD");
	CProtocol::RegistProtoName(RPCC2M_GIFTIBREQGOODS_TYPE, "RPCC2M_GIFTIBREQGOODS");
	CProtocol::RegistProtoName(RPCC2G_ARTIFACTCOMPOSE_TYPE, "RPCC2G_ARTIFACTCOMPOSE");
	CProtocol::RegistProtoName(RPCC2G_REPLYPARTYEXCHANGEITEMOPT_TYPE, "RPCC2G_REPLYPARTYEXCHANGEITEMOPT");
	CProtocol::RegistProtoName(RPCC2G_ABSENTERSCENE_TYPE, "RPCC2G_ABSENTERSCENE");
	CProtocol::RegistProtoName(RPCC2G_ENCHANTACTIVEATTRIBUTE_TYPE, "RPCC2G_ENCHANTACTIVEATTRIBUTE");
	CProtocol::RegistProtoName(RPCC2G_BUYEXTRASKILLEMBLEMSLOT_TYPE, "RPCC2G_BUYEXTRASKILLEMBLEMSLOT");
	CProtocol::RegistProtoName(RPCC2G_CHANGELIVEVISIBLE_TYPE, "RPCC2G_CHANGELIVEVISIBLE");
	CProtocol::RegistProtoName(RPCC2G_REPORTBADPLAYER_TYPE, "RPCC2G_REPORTBADPLAYER");
	CProtocol::RegistProtoName(RPCC2M_KMATCHCOMMONREQ_TYPE, "RPCC2M_KMATCHCOMMONREQ");
	CProtocol::RegistProtoName(RPCC2G_GETGUILDPARTYRECEIVEINFO_TYPE, "RPCC2G_GETGUILDPARTYRECEIVEINFO");
	CProtocol::RegistProtoName(RPCC2G_RETURNSMELTSTONE_TYPE, "RPCC2G_RETURNSMELTSTONE");
	CProtocol::RegistProtoName(RPCC2G_GETPLATSHAREAWARD_TYPE, "RPCC2G_GETPLATSHAREAWARD");
	CProtocol::RegistProtoName(RPCC2G_WEEKEND4V4GETINFO_TYPE, "RPCC2G_WEEKEND4V4GETINFO");
	CProtocol::RegistProtoName(RPCC2M_GETFLOWERACTIVITYREWARD_TYPE, "RPCC2M_GETFLOWERACTIVITYREWARD");
	CProtocol::RegistProtoName(RPCC2G_SCENEMOBAOP_TYPE, "RPCC2G_SCENEMOBAOP");
	CProtocol::RegistProtoName(RPCC2G_MOBASIGNALING_TYPE, "RPCC2G_MOBASIGNALING");
	CProtocol::RegistProtoName(RPCC2G_ACTIVATEHAIRCOLOR_TYPE, "RPCC2G_ACTIVATEHAIRCOLOR");
	CProtocol::RegistProtoName(RPCC2G_DHRREQC2G_TYPE, "RPCC2G_DHRREQC2G");
	CProtocol::RegistProtoName(RPCC2M_GETMOBABATTLEGAMERECORD_TYPE, "RPCC2M_GETMOBABATTLEGAMERECORD");
	CProtocol::RegistProtoName(RPCC2M_GETMOBABATTLEBRIEFRECORD_TYPE, "RPCC2M_GETMOBABATTLEBRIEFRECORD");
	CProtocol::RegistProtoName(RPCC2G_PETOPERATIONOTHER_TYPE, "RPCC2G_PETOPERATIONOTHER");
	CProtocol::RegistProtoName(RPCC2N_LOGINRECONNECTREQ_TYPE, "RPCC2N_LOGINRECONNECTREQ");
	CProtocol::RegistProtoName(RPCC2M_ENTERBMREADYSCENE_TYPE, "RPCC2M_ENTERBMREADYSCENE");
	CProtocol::RegistProtoName(RPCC2M_QUERYBIGMELEERANK_TYPE, "RPCC2M_QUERYBIGMELEERANK");
	CProtocol::RegistProtoName(RPCC2M_GETMOBABATTLEINFO_TYPE, "RPCC2M_GETMOBABATTLEINFO");
	CProtocol::RegistProtoName(RPCC2M_GETMOBABATTLEWEEKREWARD_TYPE, "RPCC2M_GETMOBABATTLEWEEKREWARD");
	CProtocol::RegistProtoName(RPCC2G_BUYSPRITEEGG_TYPE, "RPCC2G_BUYSPRITEEGG");
	CProtocol::RegistProtoName(RPCC2G_GETENHANCEATTR_TYPE, "RPCC2G_GETENHANCEATTR");
	CProtocol::RegistProtoName(RPCC2M_GROUPCHATLEADERREVIEWLIST_TYPE, "RPCC2M_GROUPCHATLEADERREVIEWLIST");
	CProtocol::RegistProtoName(RPCC2M_GROUPCHATFINDROLEINFOLIST_TYPE, "RPCC2M_GROUPCHATFINDROLEINFOLIST");
	CProtocol::RegistProtoName(RPCC2M_GROUPCHATFINDTEAMINFOLIST_TYPE, "RPCC2M_GROUPCHATFINDTEAMINFOLIST");
	CProtocol::RegistProtoName(RPCC2M_GROUPCHATPLAYERISSUEINFO_TYPE, "RPCC2M_GROUPCHATPLAYERISSUEINFO");
	CProtocol::RegistProtoName(RPCC2M_GROUPCHATLEADERISSUEINFO_TYPE, "RPCC2M_GROUPCHATLEADERISSUEINFO");
	CProtocol::RegistProtoName(RPCC2M_GROUPCHATLEADERREVIEW_TYPE, "RPCC2M_GROUPCHATLEADERREVIEW");
	CProtocol::RegistProtoName(RPCC2M_GROUPCHATPLAYERAPPLY_TYPE, "RPCC2M_GROUPCHATPLAYERAPPLY");
	CProtocol::RegistProtoName(RPCC2M_GROUPCHATGETGROUPINFO_TYPE, "RPCC2M_GROUPCHATGETGROUPINFO");
	CProtocol::RegistProtoName(RPCC2M_GROUPCHATMANAGER_TYPE, "RPCC2M_GROUPCHATMANAGER");
	CProtocol::RegistProtoName(RPCC2M_GROUPCHATQUIT_TYPE, "RPCC2M_GROUPCHATQUIT");
	CProtocol::RegistProtoName(RPCC2M_GROUPCHATCREATE_TYPE, "RPCC2M_GROUPCHATCREATE");
	CProtocol::RegistProtoName(RPCC2G_GETANCIENTTIMESAWARD_TYPE, "RPCC2G_GETANCIENTTIMESAWARD");
	CProtocol::RegistProtoName(RPCC2G_SELECTHEROANCIENTPOWER_TYPE, "RPCC2G_SELECTHEROANCIENTPOWER");
	CProtocol::RegistProtoName(RPCC2M_GROUPCHATCLEAR_TYPE, "RPCC2M_GROUPCHATCLEAR");
	CProtocol::RegistProtoName(RPCC2M_HOLDWEDDING_TYPE, "RPCC2M_HOLDWEDDING");
	CProtocol::RegistProtoName(RPCC2M_GETALLWEDDINGINFO_TYPE, "RPCC2M_GETALLWEDDINGINFO");
	CProtocol::RegistProtoName(RPCC2M_GETWEDDINGINVITEINFO_TYPE, "RPCC2M_GETWEDDINGINVITEINFO");
	CProtocol::RegistProtoName(RPCC2M_ENTERWEDDINGSCENE_TYPE, "RPCC2M_ENTERWEDDINGSCENE");
	CProtocol::RegistProtoName(RPCC2M_GETMARRIAGERELATION_TYPE, "RPCC2M_GETMARRIAGERELATION");
	CProtocol::RegistProtoName(RPCC2M_MARRIAGERELATIONOP_TYPE, "RPCC2M_MARRIAGERELATIONOP");
	CProtocol::RegistProtoName(RPCC2M_WEDDINGINVITEOPERATOR_TYPE, "RPCC2M_WEDDINGINVITEOPERATOR");
	CProtocol::RegistProtoName(RPCC2M_GROUPCHATLEADERADDROLE_TYPE, "RPCC2M_GROUPCHATLEADERADDROLE");
	CProtocol::RegistProtoName(RPCC2G_WEDDINGOPERATOR_TYPE, "RPCC2G_WEDDINGOPERATOR");
	CProtocol::RegistProtoName(RPCC2G_TACTENTERSCENE_TYPE, "RPCC2G_TACTENTERSCENE");
	CProtocol::RegistProtoName(RPCC2M_GROUPCHATCHANGENAME_TYPE, "RPCC2M_GROUPCHATCHANGENAME");
	CProtocol::RegistProtoName(RPCC2G_DRAGONGROUPROLELIST_TYPE, "RPCC2G_DRAGONGROUPROLELIST");
	CProtocol::RegistProtoName(RPCC2G_DRAGONGROUPRECORD_TYPE, "RPCC2G_DRAGONGROUPRECORD");
	CProtocol::RegistProtoName(RPCC2M_GETMARRIAGELIVENESS_TYPE, "RPCC2M_GETMARRIAGELIVENESS");
	CProtocol::RegistProtoName(RPCC2M_TAKEMARRIAGECHEST_TYPE, "RPCC2M_TAKEMARRIAGECHEST");
	CProtocol::RegistProtoName(RPCC2M_CHECKROLEBEFOREPAY_TYPE, "RPCC2M_CHECKROLEBEFOREPAY");
	CProtocol::RegistProtoName(RPCC2G_ARTIFACTDEITYSTOVEOP_TYPE, "RPCC2G_ARTIFACTDEITYSTOVEOP");
	CProtocol::RegistProtoName(RPCC2M_STARTWEDDINGCAR_TYPE, "RPCC2M_STARTWEDDINGCAR");
	CProtocol::RegistProtoName(RPCC2G_THEMEACTIVITYHINT_TYPE, "RPCC2G_THEMEACTIVITYHINT");
	CProtocol::RegistProtoName(RPCC2G_BACKFLOWBUYOP_TYPE, "RPCC2G_BACKFLOWBUYOP");
	CProtocol::RegistProtoName(RPCC2G_TRANSFORMOP_TYPE, "RPCC2G_TRANSFORMOP");
	CProtocol::RegistProtoName(RPCC2G_UPGRADEEQUIP_TYPE, "RPCC2G_UPGRADEEQUIP");
	CProtocol::RegistProtoName(RPCC2G_GETFASHIONSYNTHESISINFO_TYPE, "RPCC2G_GETFASHIONSYNTHESISINFO");
	CProtocol::RegistProtoName(RPCC2G_FASHIONIBSHOPBUY_TYPE, "RPCC2G_FASHIONIBSHOPBUY");
	CProtocol::RegistProtoName(RPCC2M_ENTERBATTLEREADYSCENE_TYPE, "RPCC2M_ENTERBATTLEREADYSCENE");
	CProtocol::RegistProtoName(RPCC2M_GETDRAGONGUILDLIVENESS_TYPE, "RPCC2M_GETDRAGONGUILDLIVENESS");
	CProtocol::RegistProtoName(RPCC2M_CREATEORJOINDRAGONGUILD_TYPE, "RPCC2M_CREATEORJOINDRAGONGUILD");
	CProtocol::RegistProtoName(RPCC2G_TAKEDRAGONGUILDCHEST_TYPE, "RPCC2G_TAKEDRAGONGUILDCHEST");
	CProtocol::RegistProtoName(RPCC2M_FETCHDGAPPS_TYPE, "RPCC2M_FETCHDGAPPS");
	CProtocol::RegistProtoName(RPCC2M_CHANGEDRAGONGUILDSETTING_TYPE, "RPCC2M_CHANGEDRAGONGUILDSETTING");
	CProtocol::RegistProtoName(RPCC2M_DRAGONGUILDAPPROVAL_TYPE, "RPCC2M_DRAGONGUILDAPPROVAL");
	CProtocol::RegistProtoName(RPCC2M_LEAVEFROMDRAGONGUILD_TYPE, "RPCC2M_LEAVEFROMDRAGONGUILD");
	CProtocol::RegistProtoName(RPCC2M_MODIFYDRAGONGUILDNAME_TYPE, "RPCC2M_MODIFYDRAGONGUILDNAME");
	CProtocol::RegistProtoName(RPCC2M_ASKDRAGONGUILDMEMBERS_TYPE, "RPCC2M_ASKDRAGONGUILDMEMBERS");
	CProtocol::RegistProtoName(RPCC2M_FETCHDRAGONGUILDLIST_TYPE, "RPCC2M_FETCHDRAGONGUILDLIST");
	CProtocol::RegistProtoName(RPCC2M_GETDRAGONGUILDTASKINFO_TYPE, "RPCC2M_GETDRAGONGUILDTASKINFO");
	CProtocol::RegistProtoName(RPCC2G_GETDRAGONGUILDTASKCHEST_TYPE, "RPCC2G_GETDRAGONGUILDTASKCHEST");
	CProtocol::RegistProtoName(RPCC2M_BUYDRAGONGUILDSHOPITEM_TYPE, "RPCC2M_BUYDRAGONGUILDSHOPITEM");
	CProtocol::RegistProtoName(RPCC2M_GETDRAGONGUILDSHOP_TYPE, "RPCC2M_GETDRAGONGUILDSHOP");
	CProtocol::RegistProtoName(RPCC2M_GETDRAGONGUILDSHOPRECORD_TYPE, "RPCC2M_GETDRAGONGUILDSHOPRECORD");
	CProtocol::RegistProtoName(RPCC2M_CHANGEDRAGONGUILDPOSITION_TYPE, "RPCC2M_CHANGEDRAGONGUILDPOSITION");
	CProtocol::RegistProtoName(RPCC2M_GETDRAGONGUILDBINDINFO_TYPE, "RPCC2M_GETDRAGONGUILDBINDINFO");
	CProtocol::RegistProtoName(RPCC2M_DRAGONGUILDBINDGROUPREQ_TYPE, "RPCC2M_DRAGONGUILDBINDGROUPREQ");
	CProtocol::RegistProtoName(RPCC2M_DRAGONGUILDJOINBINDGROUP_TYPE, "RPCC2M_DRAGONGUILDJOINBINDGROUP");
	CProtocol::RegistProtoName(RPCC2M_DRAGONGUILDUNBINDGROUP_TYPE, "RPCC2M_DRAGONGUILDUNBINDGROUP");
	CProtocol::RegistProtoName(RPCC2M_GOALAWARDSGETLIST_TYPE, "RPCC2M_GOALAWARDSGETLIST");
	CProtocol::RegistProtoName(RPCC2M_GOALAWARDSGETAWARDS_TYPE, "RPCC2M_GOALAWARDSGETAWARDS");
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
	CProtocol::RegistProtoName(RPCC2G_GETLUCKYACTIVITYINFO_TYPE, "RPCC2G_GETLUCKYACTIVITYINFO");
	CProtocol::RegistProtoName(RPCC2G_BUYDRAW_TYPE, "RPCC2G_BUYDRAW");
	CProtocol::RegistProtoName(RPCC2G_LOTTERYDRAW_TYPE, "RPCC2G_LOTTERYDRAW");
	CProtocol::RegistProtoName(RPCC2M_NPCFLREQC2M_TYPE, "RPCC2M_NPCFLREQC2M");
	CProtocol::RegistProtoName(RPCC2M_GETDAILYTASKREFRESHINFO_TYPE, "RPCC2M_GETDAILYTASKREFRESHINFO");
	CProtocol::RegistProtoName(RPCC2M_GETDAILYTASKASKHELP_TYPE, "RPCC2M_GETDAILYTASKASKHELP");
	CProtocol::RegistProtoName(RPCC2M_GETDAILYTASKREFRESHRECORD_TYPE, "RPCC2M_GETDAILYTASKREFRESHRECORD");
	CProtocol::RegistProtoName(RPCC2M_DAILYTASKREFRESHOPER_TYPE, "RPCC2M_DAILYTASKREFRESHOPER");
	CProtocol::RegistProtoName(RPCC2G_SURVIVEREQC2G_TYPE, "RPCC2G_SURVIVEREQC2G");
	CProtocol::RegistProtoName(RPCC2G_DOODADITEMALLSKILLREQ_TYPE, "RPCC2G_DOODADITEMALLSKILLREQ");
	CProtocol::RegistProtoName(RPCC2G_PEERBOX_TYPE, "RPCC2G_PEERBOX");
	CProtocol::RegistProtoName(RPCC2G_QUERYBOXS_TYPE, "RPCC2G_QUERYBOXS");
	CProtocol::RegistProtoName(RPCC2G_ACTIVATEPRESHOW_TYPE, "RPCC2G_ACTIVATEPRESHOW");
	CProtocol::RegistProtoName(RPCC2G_GUILDPARTYSUMMONSPIRIT_TYPE, "RPCC2G_GUILDPARTYSUMMONSPIRIT");
	CProtocol::RegistProtoName(RPCC2G_CHOOSESPECIALEFFECTS_TYPE, "RPCC2G_CHOOSESPECIALEFFECTS");
	CProtocol::RegistProtoName(RPCC2G_ENTERLEISURESCENE_TYPE, "RPCC2G_ENTERLEISURESCENE");
	CProtocol::RegistProtoName(RPCC2G_BACKFLOWACTIVITYOPERATION_TYPE, "RPCC2G_BACKFLOWACTIVITYOPERATION");
	CProtocol::RegistProtoName(RPCC2G_GETNEWZONEBENEFIT_TYPE, "RPCC2G_GETNEWZONEBENEFIT");
	CProtocol::RegistProtoName(RPCC2G_SELECTCHARGEBACKROLE_TYPE, "RPCC2G_SELECTCHARGEBACKROLE");

}