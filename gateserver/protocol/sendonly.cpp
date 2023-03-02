#include "pch.h"
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

void PtcT2G_RegistServerNtf::Process(UINT32 dwConnID) {}
void PtcT2C_LoginChallenge::Process(UINT32 dwConnID) {}
void PtcT2C_KeepAlivePingReq::Process(UINT32 dwConnID) {}
void PtcT2C_ErrorNotify::Process(UINT32 dwConnID) {}
void PtcT2C_ChatNotify::Process(UINT32 dwConnID) {}
void PtcT2G_ReconnectNtf::Process(UINT32 dwConnID) {}
void PtcT2N_LogoutNtf::Process(UINT32 dwConnID) {}
void PtcG2C_NotifyAccountData::Process(UINT32 dwConnID) {}
void PtcC2G_EnterSceneReq::Process(UINT32 dwConnID) {}
void PtcG2C_EnterSceneNtf::Process(UINT32 dwConnID) {}
void PtcC2G_LeaveSceneReq::Process(UINT32 dwConnID) {}
void PtcG2C_LeaveSceneNtf::Process(UINT32 dwConnID) {}
void PtcC2G_MoveOperationReq::Process(UINT32 dwConnID) {}
void PtcG2C_UnitAppear::Process(UINT32 dwConnID) {}
void PtcG2C_UnitDisappear::Process(UINT32 dwConnID) {}
void PtcC2G_CastSkill::Process(UINT32 dwConnID) {}
void PtcG2C_SkillResult::Process(UINT32 dwConnID) {}
void PtcG2C_DeathNotify::Process(UINT32 dwConnID) {}
void PtcG2C_AttributeChangeNotify::Process(UINT32 dwConnID) {}
void PtcG2C_LevelChangeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ItemChangedNtf::Process(UINT32 dwConnID) {}
void PtcC2G_UpdateTutorial::Process(UINT32 dwConnID) {}
void PtcG2C_OpenSystemNtf::Process(UINT32 dwConnID) {}
void PtcG2C_BattleResultNtf::Process(UINT32 dwConnID) {}
void PtcG2C_CompleteAchivement::Process(UINT32 dwConnID) {}
void PtcG2C_SkillChangedNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GSErrorNotify::Process(UINT32 dwConnID) {}
void PtcG2C_ChangeFashionNotify::Process(UINT32 dwConnID) {}
void PtcG2C_CheckinInfoNotify::Process(UINT32 dwConnID) {}
void PtcG2C_ActivityRoleNotify::Process(UINT32 dwConnID) {}
void PtcG2C_UpdatePayInfo::Process(UINT32 dwConnID) {}
void PtcG2C_RewardChangedNtf::Process(UINT32 dwConnID) {}
void PtcG2C_LeaveTeam::Process(UINT32 dwConnID) {}
void PtcG2C_ReviveNotify::Process(UINT32 dwConnID) {}
void PtcG2C_UpdateBuyGoldAndFatigueInfo::Process(UINT32 dwConnID) {}
void PtcC2G_RoleDeathInSoloScene::Process(UINT32 dwConnID) {}
void PtcG2C_ReviveCountdown::Process(UINT32 dwConnID) {}
void PtcG2C_FatigueRecoverTimeNotify::Process(UINT32 dwConnID) {}
void PtcC2G_EnterSceneCoolDownQuery::Process(UINT32 dwConnID) {}
void PtcG2C_EnterSceneCoolDownNotify::Process(UINT32 dwConnID) {}
void PtcG2C_FashoinChangedNtf::Process(UINT32 dwConnID) {}
void PtcC2G_OperateRecordNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GuildNotifyMemberChanged::Process(UINT32 dwConnID) {}
void PtcG2C_GuildSettingChangedNotify::Process(UINT32 dwConnID) {}
void PtcG2C_GuildCheckinBoxNtf::Process(UINT32 dwConnID) {}
void PtcG2C_NewGuildBonusNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GuildBestCardsNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ExecuteLevelScriptNtf::Process(UINT32 dwConnID) {}
void PtcG2C_OnlineRewardNtf::Process(UINT32 dwConnID) {}
void PtcG2C_NextDayRewardNtf::Process(UINT32 dwConnID) {}
void PtcC2G_OnlineRewardReport::Process(UINT32 dwConnID) {}
void PtcC2G_NextDayRewardReport::Process(UINT32 dwConnID) {}
void PtcG2C_HintNotify::Process(UINT32 dwConnID) {}
void PtcG2C_PositionDebug::Process(UINT32 dwConnID) {}
void PtcG2C_EnemyDorpDoodadNtf::Process(UINT32 dwConnID) {}
void PtcG2C_TakeRandomTask::Process(UINT32 dwConnID) {}
void PtcG2C_BuffNotify::Process(UINT32 dwConnID) {}
void PtcG2C_TeamSelectNotify::Process(UINT32 dwConnID) {}
void PtcG2C_TeamInviteNotify::Process(UINT32 dwConnID) {}
void PtcG2C_FetchEnemyDoodadNtf::Process(UINT32 dwConnID) {}
void PtcC2G_DelayNotify::Process(UINT32 dwConnID) {}
void PtcG2C_PkPrepareNtf::Process(UINT32 dwConnID) {}
void PtcG2C_PkTimeoutNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GuildGoblinKillNtf::Process(UINT32 dwConnID) {}
void PtcC2G_SkillResultReq::Process(UINT32 dwConnID) {}
void PtcC2G_SkillBulletFireReport::Process(UINT32 dwConnID) {}
void PtcC2G_SkillBulletResultReq::Process(UINT32 dwConnID) {}
void PtcG2C_SceneFinishStateChanged::Process(UINT32 dwConnID) {}
void PtcG2C_PkRoleInfoNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ItemCircleDrawResult::Process(UINT32 dwConnID) {}
void PtcC2G_AddLevelScriptExtString::Process(UINT32 dwConnID) {}
void PtcC2G_SceneDamageRankReport::Process(UINT32 dwConnID) {}
void PtcG2C_SceneDamageRankNtf::Process(UINT32 dwConnID) {}
void PtcG2C_SceneLeftDoodad::Process(UINT32 dwConnID) {}
void PtcC2G_ChangeSupplementReport::Process(UINT32 dwConnID) {}
void PtcG2C_ChangeSupplementNtf::Process(UINT32 dwConnID) {}
void PtcC2G_QTEOperation::Process(UINT32 dwConnID) {}
void PtcG2C_CorrectPosition::Process(UINT32 dwConnID) {}
void PtcG2C_TeleportNotice::Process(UINT32 dwConnID) {}
void PtcG2C_AllyMatchRoleIDNotify::Process(UINT32 dwConnID) {}
void PtcG2C_TeamFullDataNtf::Process(UINT32 dwConnID) {}
void PtcG2C_PetChangeNotfiy::Process(UINT32 dwConnID) {}
void PtcG2C_ExpFindBackNtf::Process(UINT32 dwConnID) {}
void PtcC2G_AutoFightNTF::Process(UINT32 dwConnID) {}
void PtcG2C_TowerSceneInfoNtf::Process(UINT32 dwConnID) {}
void PtcG2C_CoverDesignationNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ReachAchieveNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ReachDesignationNtf::Process(UINT32 dwConnID) {}
void PtcC2G_SyncSceneFinish::Process(UINT32 dwConnID) {}
void PtcG2C_LevelSealNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ReconnectSyncNotify::Process(UINT32 dwConnID) {}
void PtcG2C_OutLookChangeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_FiveDayRewardNTF::Process(UINT32 dwConnID) {}
void PtcG2C_SyncStepNotify::Process(UINT32 dwConnID) {}
void PtcC2G_TssSdkSendAnti2Server::Process(UINT32 dwConnID) {}
void PtcG2C_TssSdkAntiDataNtf::Process(UINT32 dwConnID) {}
void PtcM2C_AnswerAckNtf::Process(UINT32 dwConnID) {}
void PtcM2C_PushQuestionNtf::Process(UINT32 dwConnID) {}
void PtcG2C_QANotify::Process(UINT32 dwConnID) {}
void PtcG2C_PvpBattleBeginNtf::Process(UINT32 dwConnID) {}
void PtcG2C_PvpBattleEndNtf::Process(UINT32 dwConnID) {}
void PtcG2C_PvpBattleKill::Process(UINT32 dwConnID) {}
void PtcG2C_FlowerRankRewardNtf::Process(UINT32 dwConnID) {}
void PtcM2C_QAOverNtf::Process(UINT32 dwConnID) {}
void PtcM2C_QARoomRankNtf::Process(UINT32 dwConnID) {}
void PtcG2C_FastMBConfirmNtf::Process(UINT32 dwConnID) {}
void PtcC2G_FMBRefuse::Process(UINT32 dwConnID) {}
void PtcG2C_fastMBDismissNtf::Process(UINT32 dwConnID) {}
void PtcG2C_SelectRoleNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ReceiveFlowerNtf::Process(UINT32 dwConnID) {}
void PtcG2C_FlowerRainNtf::Process(UINT32 dwConnID) {}
void PtcG2C_StartBattleFailedNtf::Process(UINT32 dwConnID) {}
void PtcM2C_QAEnterRoomNtf::Process(UINT32 dwConnID) {}
void PtcG2C_WatchBattleInfoNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GmfJoinBattleReq::Process(UINT32 dwConnID) {}
void PtcG2C_GmfOneBattleEndNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GmfBaseDataNtf::Process(UINT32 dwConnID) {}
void PtcG2C_WorldChannelLeftTimesNtf::Process(UINT32 dwConnID) {}
void PtcG2C_NoticeGuildBossEnd::Process(UINT32 dwConnID) {}
void PtcG2C_UpdateGuildArenaState::Process(UINT32 dwConnID) {}
void PtcG2C_GuildBossTimeOut::Process(UINT32 dwConnID) {}
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
void PtcG2C_NotifyClientEnterFight::Process(UINT32 dwConnID) {}
void PtcG2C_SynGuildArenaRoleOnline::Process(UINT32 dwConnID) {}
void PtcG2C_ClientOnlyBuffNotify::Process(UINT32 dwConnID) {}
void PtcM2C_MCChatOffLineNotify::Process(UINT32 dwConnID) {}
void PtcM2C_FriendOpNtfNew::Process(UINT32 dwConnID) {}
void PtcC2M_LoadOfflineChatNtf::Process(UINT32 dwConnID) {}
void PtcC2M_FriendQueryReportNew::Process(UINT32 dwConnID) {}
void PtcM2C_FriendDegreeUpNtfNew::Process(UINT32 dwConnID) {}
void PtcC2M_BlackListReportNew::Process(UINT32 dwConnID) {}
void PtcM2C_BlackListNtfNew::Process(UINT32 dwConnID) {}
void PtcG2C_synguildarenadisplace::Process(UINT32 dwConnID) {}
void PtcG2C_OnTransferWall::Process(UINT32 dwConnID) {}
void PtcG2C_NotifyWatchIconNum2Client::Process(UINT32 dwConnID) {}
void PtcG2C_GmfWaitOtherLoad::Process(UINT32 dwConnID) {}
void PtcG2C_GmfWaitFightBegin::Process(UINT32 dwConnID) {}
void PtcC2M_RoleStateReportNew::Process(UINT32 dwConnID) {}
void PtcM2C_RoleStateNtfNew::Process(UINT32 dwConnID) {}
void PtcM2C_HintNotifyMS::Process(UINT32 dwConnID) {}
void PtcG2C_WorldBossAttrNtf::Process(UINT32 dwConnID) {}
void PtcC2G_JoinRoom::Process(UINT32 dwConnID) {}
void PtcG2C_JoinRoomReply::Process(UINT32 dwConnID) {}
void PtcC2G_QuitRoom::Process(UINT32 dwConnID) {}
void PtcG2C_UpdateVoipRoomMemberNtf::Process(UINT32 dwConnID) {}
void PtcG2C_PayAllInfoNtf::Process(UINT32 dwConnID) {}
void PtcG2C_LoginReward2CNtf::Process(UINT32 dwConnID) {}
void PtcM2C_MSReceiveFlowerNtf::Process(UINT32 dwConnID) {}
void PtcM2C_MSFlowerRainNtf::Process(UINT32 dwConnID) {}
void PtcM2C_TeamChangeM2CNtf::Process(UINT32 dwConnID) {}
void PtcM2C_LeaveTeamM2CNtf::Process(UINT32 dwConnID) {}
void PtcM2C_TeamFullDataM2CNtf::Process(UINT32 dwConnID) {}
void PtcM2C_TeamInviteM2CNotify::Process(UINT32 dwConnID) {}
void PtcM2C_MSErrorNotify::Process(UINT32 dwConnID) {}
void PtcM2C_InviteRefuseM2CNtf::Process(UINT32 dwConnID) {}
void PtcC2M_TeamInviteAckC2M::Process(UINT32 dwConnID) {}
void PtcM2C_RoleMatchStateM2CNotify::Process(UINT32 dwConnID) {}
void PtcM2C_FastMBConfirmM2CNtf::Process(UINT32 dwConnID) {}
void PtcM2C_fastMBDismissM2CNtf::Process(UINT32 dwConnID) {}
void PtcC2M_FMBRefuseC2M::Process(UINT32 dwConnID) {}
void PtcG2C_AIDebugInfo::Process(UINT32 dwConnID) {}
void PtcG2C_StartRollNtf::Process(UINT32 dwConnID) {}
void PtcC2N_CheckQueuingReq::Process(UINT32 dwConnID) {}
void PtcN2C_CheckQueuingNtf::Process(UINT32 dwConnID) {}
void PtcG2C_NoticeHungryDown::Process(UINT32 dwConnID) {}
void PtcG2C_FirstPassRewardNtf::Process(UINT32 dwConnID) {}
void PtcG2C_TitleChangeNotify::Process(UINT32 dwConnID) {}
void PtcM2C_QAIDNameNtf::Process(UINT32 dwConnID) {}
void PtcM2C_PkTimeoutM2CNtf::Process(UINT32 dwConnID) {}
void PtcM2C_LoginGuildInfo::Process(UINT32 dwConnID) {}
void PtcM2C_SendGuildSkillInfo::Process(UINT32 dwConnID) {}
void PtcM2C_MSEventNotify::Process(UINT32 dwConnID) {}
void PtcG2C_ItemFindBackNtf::Process(UINT32 dwConnID) {}
void PtcG2C_SpriteChangedNtf::Process(UINT32 dwConnID) {}
void PtcG2C_FightGroupChangeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_BossRushOneFinishNtf::Process(UINT32 dwConnID) {}
void PtcG2C_SynAtlasAttr::Process(UINT32 dwConnID) {}
void PtcG2C_PlayDiceNtf::Process(UINT32 dwConnID) {}
void PtcC2G_PlayDiceOver::Process(UINT32 dwConnID) {}
void PtcG2C_IBShopIcon::Process(UINT32 dwConnID) {}
void PtcC2G_RemoveIBShopIcon::Process(UINT32 dwConnID) {}
void PtcG2C_NotifyEnhanceSuit::Process(UINT32 dwConnID) {}
void PtcM2C_SynGuildArenaFightUnitNew::Process(UINT32 dwConnID) {}
void PtcM2C_SynGuildArenaBattleInfoNew::Process(UINT32 dwConnID) {}
void PtcM2C_NoticeGuildArenaBeginNew::Process(UINT32 dwConnID) {}
void PtcM2C_SynGuildArenaRoleOnlineNew::Process(UINT32 dwConnID) {}
void PtcM2C_synguildarenadisplaceNew::Process(UINT32 dwConnID) {}
void PtcG2C_RiskBuyNtf::Process(UINT32 dwConnID) {}
void PtcC2G_SetVoipMemberState::Process(UINT32 dwConnID) {}
void PtcG2C_RiskRecoverDiceNtf::Process(UINT32 dwConnID) {}
void PtcM2C_NotifyGuildSkillData::Process(UINT32 dwConnID) {}
void PtcC2G_OpenSpriteEggNtf::Process(UINT32 dwConnID) {}
void PtcM2C_GuildSetingNotify::Process(UINT32 dwConnID) {}
void PtcM2C_GuildBestCardsNtfMs::Process(UINT32 dwConnID) {}
void PtcG2C_TowerFirstPassRewardNtf::Process(UINT32 dwConnID) {}
void PtcM2C_DERankChangNtf::Process(UINT32 dwConnID) {}
void PtcM2C_GmfJoinBattleM2CReq::Process(UINT32 dwConnID) {}
void PtcC2M_GmfJoinBattleC2MRes::Process(UINT32 dwConnID) {}
void PtcG2C_HallIconSNtf::Process(UINT32 dwConnID) {}
void PtcG2C_UpdateTaskStatus::Process(UINT32 dwConnID) {}
void PtcM2C_SkyCityFinalRes::Process(UINT32 dwConnID) {}
void PtcG2C_SkyCityEstimateRes::Process(UINT32 dwConnID) {}
void PtcG2C_SkyCityTimeRes::Process(UINT32 dwConnID) {}
void PtcG2C_NotifyIdipMessageGs::Process(UINT32 dwConnID) {}
void PtcM2C_NotifyIdipMessageMs::Process(UINT32 dwConnID) {}
void PtcG2C_IdipPunishInfoNtf::Process(UINT32 dwConnID) {}
void PtcM2C_IdipPunishInfoMsNtf::Process(UINT32 dwConnID) {}
void PtcM2C_IdipClearChatNtf::Process(UINT32 dwConnID) {}
void PtcG2C_SkyCityTeamRes::Process(UINT32 dwConnID) {}
void PtcC2M_GuildCardRankReq::Process(UINT32 dwConnID) {}
void PtcM2C_GuildCardRankNtf::Process(UINT32 dwConnID) {}
void PtcM2C_NoticeGuildLadderStart::Process(UINT32 dwConnID) {}
void PtcG2C_GmfKickNty::Process(UINT32 dwConnID) {}
void PtcC2M_GuildCardMatchReq::Process(UINT32 dwConnID) {}
void PtcM2C_GuildCardMatchNtf::Process(UINT32 dwConnID) {}
void PtcC2M_OpenPrivateChatNtf::Process(UINT32 dwConnID) {}
void PtcG2C_SkyCityBattleDataNtf::Process(UINT32 dwConnID) {}
void PtcC2M_CommitAnswerNtf::Process(UINT32 dwConnID) {}
void PtcC2M_GiveUpQAQuestionNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GmfBattleStateNtf::Process(UINT32 dwConnID) {}
void PtcG2C_SpActivityChangeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_SpActivityOffsetDayNtf::Process(UINT32 dwConnID) {}
void PtcM2C_SpriteStateChangeNtf::Process(UINT32 dwConnID) {}
void PtcM2C_InvUnfStateM2CNtf::Process(UINT32 dwConnID) {}
void PtcG2C_DPSNotify::Process(UINT32 dwConnID) {}
void PtcM2C_NoticeGuildArenaNextTime::Process(UINT32 dwConnID) {}
void PtcG2C_SkillCoolNtf::Process(UINT32 dwConnID) {}
void PtcM2C_GardenBanquetNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GardenPlantEventNotice::Process(UINT32 dwConnID) {}
void PtcG2C_GardenBanquetNotice::Process(UINT32 dwConnID) {}
void PtcC2M_SyncPlatFriend2MS::Process(UINT32 dwConnID) {}
void PtcM2C_NoticeGuildWageReward::Process(UINT32 dwConnID) {}
void PtcG2C_GmfGuildCombatNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GprOneBattleEndNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GprAllFightEndNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ResWarBattleDataNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ResWarTeamResOne::Process(UINT32 dwConnID) {}
void PtcM2C_SendGuildBonusNtf::Process(UINT32 dwConnID) {}
void PtcC2G_BattleLogReport::Process(UINT32 dwConnID) {}
void PtcM2C_ResWarGuildBriefNtf::Process(UINT32 dwConnID) {}
void PtcC2G_ClickGuildCamp::Process(UINT32 dwConnID) {}
void PtcG2C_PayMemberPrivilegeNtf::Process(UINT32 dwConnID) {}
void PtcM2C_SynGuilIntegralState::Process(UINT32 dwConnID) {}
void PtcM2C_ResWarStateNtf::Process(UINT32 dwConnID) {}
void PtcM2C_SynGuildIntegralState::Process(UINT32 dwConnID) {}
void PtcG2C_KillEnemyScoreNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ServerOpenDayNtf::Process(UINT32 dwConnID) {}
void PtcM2C_ResWarRankSimpleInfoNtf::Process(UINT32 dwConnID) {}
void PtcM2C_GuildBuffSimpleInfoNtf::Process(UINT32 dwConnID) {}
void PtcC2M_QueryResWarRequet::Process(UINT32 dwConnID) {}
void PtcM2C_GuildBuffSimpleItemNtf::Process(UINT32 dwConnID) {}
void PtcC2M_GardenFishStop::Process(UINT32 dwConnID) {}
void PtcM2C_HallIconMNtf::Process(UINT32 dwConnID) {}
void PtcG2C_synGuildInheritExp::Process(UINT32 dwConnID) {}
void PtcC2G_SynDoingGuildInherit::Process(UINT32 dwConnID) {}
void PtcG2C_NoticeDoingGuildInherit::Process(UINT32 dwConnID) {}
void PtcG2C_SynGuildInheritNumInfo::Process(UINT32 dwConnID) {}
void PtcM2C_ResWarMineDataNtf::Process(UINT32 dwConnID) {}
void PtcG2C_CdCall::Process(UINT32 dwConnID) {}
void PtcC2M_LeaveLargeRoom::Process(UINT32 dwConnID) {}
void PtcM2C_LargeRoomRoleNtf::Process(UINT32 dwConnID) {}
void PtcM2C_LargeRoomLoginParamNtf::Process(UINT32 dwConnID) {}
void PtcM2C_ResWarTimeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_SyncMoveNotify::Process(UINT32 dwConnID) {}
void PtcC2M_HandleMicphone::Process(UINT32 dwConnID) {}
void PtcG2C_ChangeNameCountNtf::Process(UINT32 dwConnID) {}
void PtcM2C_GuildBuffCDParamNtf::Process(UINT32 dwConnID) {}
void PtcG2C_AntiAddictionRemindNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HorseAwardAllNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HorseRankNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HorseWaitTimeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HorseAnimationNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HorseFinalNtf::Process(UINT32 dwConnID) {}
void PtcC2G_PicUrlNtf::Process(UINT32 dwConnID) {}
void PtcM2C_NotifyMentorApply::Process(UINT32 dwConnID) {}
void PtcC2M_PokerTournamentReq::Process(UINT32 dwConnID) {}
void PtcM2C_UpdatePartnerToClient::Process(UINT32 dwConnID) {}
void PtcM2C_MakePartnerResultNtf::Process(UINT32 dwConnID) {}
void PtcM2C_InvFightNotify::Process(UINT32 dwConnID) {}
void PtcG2C_HorseCountDownTimeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_DoodadItemAddNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GCFZhanLingNotify::Process(UINT32 dwConnID) {}
void PtcG2C_InvFightBefEnterSceneNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HeroBattleDataNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ScenePrepareInfoNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HeroBattleTeamMsgNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HeroBattleSyncNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HeroBattleInCircleNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HeroBattleOverTime::Process(UINT32 dwConnID) {}
void PtcC2M_PayParameterInfoNtf::Process(UINT32 dwConnID) {}
void PtcM2C_PayParameterInfoInvalidNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HeroBattleTeamRoleNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GCFSynG2CNtf::Process(UINT32 dwConnID) {}
void PtcM2C_NoticeGuildTerrWar::Process(UINT32 dwConnID) {}
void PtcM2C_NoticeGuildTerrall::Process(UINT32 dwConnID) {}
void PtcC2N_UpdateStartUpTypeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_DoodadItemUseNtf::Process(UINT32 dwConnID) {}
void PtcM2C_GuildBonusGetAll::Process(UINT32 dwConnID) {}
void PtcG2C_SpecialStateNtf::Process(UINT32 dwConnID) {}
void PtcM2C_ResWarEnemyTimeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_CloseChooseHeroNtf::Process(UINT32 dwConnID) {}
void PtcG2C_LevelScriptStateNtf::Process(UINT32 dwConnID) {}
void PtcG2C_SceneStateNtf::Process(UINT32 dwConnID) {}
void PtcC2G_QueryRoleStateReq::Process(UINT32 dwConnID) {}
void PtcG2C_QueryRoleStateAck::Process(UINT32 dwConnID) {}
void PtcG2C_IBShopHasBuyNtf::Process(UINT32 dwConnID) {}
void PtcM2C_UpdateLeagueBattleSeasonInfo::Process(UINT32 dwConnID) {}
void PtcG2C_SkillInitCoolNtf::Process(UINT32 dwConnID) {}
void PtcM2C_NotifyLeagueTeamCreate::Process(UINT32 dwConnID) {}
void PtcM2C_NotifyLeagueTeamDissolve::Process(UINT32 dwConnID) {}
void PtcG2C_HeroBattleCanUseHero::Process(UINT32 dwConnID) {}
void PtcM2C_UpdateLeagueTeamState::Process(UINT32 dwConnID) {}
void PtcM2C_LeagueBattleStartMatchNtf::Process(UINT32 dwConnID) {}
void PtcM2C_LeagueBattleStopMatchNtf::Process(UINT32 dwConnID) {}
void PtcG2C_LeagueBattleLoadInfoNtf::Process(UINT32 dwConnID) {}
void PtcG2C_LeagueBattleBaseDataNtf::Process(UINT32 dwConnID) {}
void PtcG2C_LeagueBattleOneResultNft::Process(UINT32 dwConnID) {}
void PtcG2C_LeagueBattleResultNtf::Process(UINT32 dwConnID) {}
void PtcG2C_PushPraiseNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HeroBattleTipsNtf::Process(UINT32 dwConnID) {}
void PtcG2C_LeagueBattleStateNtf::Process(UINT32 dwConnID) {}
void PtcM2C_LeagueBattleMatchTimeoutNtf::Process(UINT32 dwConnID) {}
void PtcC2G_CancelRedDotReq::Process(UINT32 dwConnID) {}
void PtcM2C_MidasExceptionNtf::Process(UINT32 dwConnID) {}
void PtcM2C_UpdateLeagueEleRoomStateNtf::Process(UINT32 dwConnID) {}
void PtcC2M_CloseLeagueEleNtf::Process(UINT32 dwConnID) {}
void PtcM2C_KickFMAuchor::Process(UINT32 dwConnID) {}
void PtcM2C_GuildAuctItemTimeFresh::Process(UINT32 dwConnID) {}
void PtcM2C_PokerTournamentEndReFund::Process(UINT32 dwConnID) {}
void PtcM2C_NoticeGuildTerrEnd::Process(UINT32 dwConnID) {}
void PtcM2C_NoticeGuildTerrBattleWin::Process(UINT32 dwConnID) {}
void PtcM2C_NoticeGuildTerrBigIcon::Process(UINT32 dwConnID) {}
void PtcC2G_CloseHintNtf::Process(UINT32 dwConnID) {}
void PtcA2C_AudioAIDNtf::Process(UINT32 dwConnID) {}
void PtcG2C_CountDownNtf::Process(UINT32 dwConnID) {}
void PtcG2C_AllGiftIBItemNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GiftIBBackInfoNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ShareRandomGiftNtf::Process(UINT32 dwConnID) {}
void PtcM2C_ModifyGuildNameNtf::Process(UINT32 dwConnID) {}
void PtcG2C_WorldBossGuildAddAttrSyncClientNtf::Process(UINT32 dwConnID) {}
void PtcG2C_MilitaryrankNtf::Process(UINT32 dwConnID) {}
void PtcG2C_LoginActivityStatusNtf::Process(UINT32 dwConnID) {}
void PtcC2G_LoginActivityAwardReq::Process(UINT32 dwConnID) {}
void PtcC2M_PayBuyGoodsFailNtf::Process(UINT32 dwConnID) {}
void PtcC2M_IBGiftTipsReq::Process(UINT32 dwConnID) {}
void PtcM2C_ArenaStarDataNtf::Process(UINT32 dwConnID) {}
void PtcM2C_NotifySkyTeamCreate::Process(UINT32 dwConnID) {}
void PtcM2C_SkyCraftMatchNtf::Process(UINT32 dwConnID) {}
void PtcM2C_SkyCraftEliRoomNtf::Process(UINT32 dwConnID) {}
void PtcC2M_CloseSkyCraftEliNtf::Process(UINT32 dwConnID) {}
void PtcG2C_DisplayAddItem::Process(UINT32 dwConnID) {}
void PtcG2C_UpdateDisplayItems::Process(UINT32 dwConnID) {}
void PtcG2C_guildcamppartyNotify::Process(UINT32 dwConnID) {}
void PtcM2C_IBGiftIconNtf::Process(UINT32 dwConnID) {}
void PtcM2C_GiftIBSucessNtf::Process(UINT32 dwConnID) {}
void PtcG2C_AbsPartyNtf::Process(UINT32 dwConnID) {}
void PtcG2C_GuildCampPartyTradeNotify::Process(UINT32 dwConnID) {}
void PtcM2C_TeamRequestPlatFreind2Client::Process(UINT32 dwConnID) {}
void PtcG2C_CustomBattleLoadingNtf::Process(UINT32 dwConnID) {}
void PtcM2C_CustomBattleGMNotify::Process(UINT32 dwConnID) {}
void PtcM2C_TarjaBriefNtf::Process(UINT32 dwConnID) {}
void PtcG2C_PlatformShareAwardNtf::Process(UINT32 dwConnID) {}
void PtcC2G_NotifyPlatShareResult::Process(UINT32 dwConnID) {}
void PtcG2C_WeekEnd4v4RoleDataNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HeroKillNotify::Process(UINT32 dwConnID) {}
void PtcG2C_MobaBattleTeamRoleNtf::Process(UINT32 dwConnID) {}
void PtcG2C_MobaRoleChangeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_MobaBattleTeamMsgNtf::Process(UINT32 dwConnID) {}
void PtcG2C_TransSkillNotfiy::Process(UINT32 dwConnID) {}
void PtcG2C_EntityTargetChangeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_MobaSignalBroadcast::Process(UINT32 dwConnID) {}
void PtcG2C_PetInviteNtf::Process(UINT32 dwConnID) {}
void PtcG2C_ChatNotifyG2C::Process(UINT32 dwConnID) {}
void PtcG2C_MobaMiniMapNtf::Process(UINT32 dwConnID) {}
void PtcG2C_MobaAddExpNtf::Process(UINT32 dwConnID) {}
void PtcG2C_BMReadyTimeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_BMFightTimeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_BMRoleSceneSyncNtf::Process(UINT32 dwConnID) {}
void PtcG2C_BigMeleeReliveNtf::Process(UINT32 dwConnID) {}
void PtcG2C_MobaHintNtf::Process(UINT32 dwConnID) {}
void PtcG2C_TajieHelpNotify::Process(UINT32 dwConnID) {}
void PtcG2C_NotifyStartUpTypeToClient::Process(UINT32 dwConnID) {}
void PtcG2C_HeroBattleAncientPower::Process(UINT32 dwConnID) {}
void PtcM2C_GroupChatManager::Process(UINT32 dwConnID) {}
void PtcM2C_GroupChatQuit::Process(UINT32 dwConnID) {}
void PtcM2C_GroupChatDismiss::Process(UINT32 dwConnID) {}
void PtcM2C_WeddingInviteNtf::Process(UINT32 dwConnID) {}
void PtcM2C_NotifyMarriageApply::Process(UINT32 dwConnID) {}
void PtcC2G_BattleStatisticsReport::Process(UINT32 dwConnID) {}
void PtcG2C_BattleStatisticsNtf::Process(UINT32 dwConnID) {}
void PtcG2C_WeddingLoadInfoNtf::Process(UINT32 dwConnID) {}
void PtcG2C_WeddingStateNtf::Process(UINT32 dwConnID) {}
void PtcG2C_WeddingEventNtf::Process(UINT32 dwConnID) {}
void PtcG2C_HorseFailTipsNtf::Process(UINT32 dwConnID) {}
void PtcM2C_NotifyMarriageDivorceApply::Process(UINT32 dwConnID) {}
void PtcG2C_WarningRandomNtf::Process(UINT32 dwConnID) {}
void PtcM2C_GroupChatIssueCount::Process(UINT32 dwConnID) {}
void PtcM2C_GroupChatApply::Process(UINT32 dwConnID) {}
void PtcC2G_ShareReport::Process(UINT32 dwConnID) {}
void PtcG2C_ThemeActivityChangeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_WeddingCarNtf::Process(UINT32 dwConnID) {}
void PtcG2C_BackFlowOpenNtf::Process(UINT32 dwConnID) {}
void PtcM2C_StartBattleFailedM2CNtf::Process(UINT32 dwConnID) {}
void PtcG2C_TransNotify::Process(UINT32 dwConnID) {}
void PtcC2G_InvfightAgainReqC2G::Process(UINT32 dwConnID) {}
void PtcM2C_LoginDragonGuildInfo::Process(UINT32 dwConnID) {}
void PtcM2C_DragonGuildSettingChanged::Process(UINT32 dwConnID) {}
void PtcM2C_DragonGuildNameNtf::Process(UINT32 dwConnID) {}
void PtcC2M_GoBackReadySceneNtf::Process(UINT32 dwConnID) {}
void PtcM2C_GoalAwardsRedPoint::Process(UINT32 dwConnID) {}
void PtcG2C_BattleFieldReadyInfoNtf::Process(UINT32 dwConnID) {}
void PtcG2C_BattleFieldReliveNtf::Process(UINT32 dwConnID) {}
void PtcG2C_BattleFieldRoleAgainstNtf::Process(UINT32 dwConnID) {}
void PtcG2C_BFFightTimeNtf::Process(UINT32 dwConnID) {}
void PtcG2C_PayScoreNtf::Process(UINT32 dwConnID) {}
void PtcG2C_NpcFlNtf::Process(UINT32 dwConnID) {}
void PtcM2C_DailyTaskEventNtf::Process(UINT32 dwConnID) {}
void PtcC2G_UpdateFreeflowHintInfo::Process(UINT32 dwConnID) {}
void PtcG2C_DoodadItemSkillsNtf::Process(UINT32 dwConnID) {}
void PtcC2M_MulActivityIconSysReq::Process(UINT32 dwConnID) {}
void PtcG2C_WordNotify::Process(UINT32 dwConnID) {}
void PtcG2C_BigMeleePointNtf::Process(UINT32 dwConnID) {}
void PtcG2C_BigMeleePointOutLookNtf::Process(UINT32 dwConnID) {}
void PtcM2C_TaskRefreshNtf::Process(UINT32 dwConnID) {}
void PtcG2C_KickAccountJkydNtf::Process(UINT32 dwConnID) {}
void PtcM2C_GuildCastFeatsNtf::Process(UINT32 dwConnID) {}
void PtcG2C_WorldLevelNtf2Client::Process(UINT32 dwConnID) {}
void PtcG2C_UpdateStageInfoNtf::Process(UINT32 dwConnID) {}

