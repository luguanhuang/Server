#include "pch.h"
#include "forward.h"
#include "config.h"
#include "ptcregister.h"
#include "crpc.h"
#include "protocol.h"

// generate by ProtoGen at date: 2017/11/6 17:39:44

#include "qa/ptcg2m_gmcloseqaroom.h"
#include "role/ptcg2m_updaterolesummary.h"
#include "scene/ptcg2m_scenedestroyed.h"
#include "chat/ptcg2m_noticeforwardntf.h"
#include "battlewatch/ptcg2m_syncliveinfotoms.h"
#include "guild/ptcg2m_rolesfinishscenetomaster.h"
#include "scene/ptcg2m_sceneendtomaster.h"
#include "guildcamp/ptcg2m_updateguildcampresult.h"
#include "chat/ptcc2m_loadofflinechatntf.h"
#include "friend/ptcc2m_friendqueryreportnew.h"
#include "friend/ptcc2m_blacklistreportnew.h"
#include "chat/ptcg2m_reportwatchntf.h"
#include "levelseal/ptcg2m_gmlevelsealinfo.h"
#include "mail/ptcg2m_addmailgs.h"
#include "common/ptcc2m_rolestatereportnew.h"
#include "chat/ptcg2m_gmsendchat.h"
#include "guild/ptcg2m_addguildbossattr.h"
#include "common/ptcg2m_firstenterscenentf.h"
#include "guild/ptcg2m_gmsetguildbosstime.h"
#include "scene/ptcg2m_synscenerewardg2m.h"
#include "guild/ptcg2m_noticeguildbossdie.h"
#include "guild/ptcg2m_createguildbosssingle.h"
#include "worldboss/ptcg2m_worldbosssyncms.h"
#include "common/ptcg2m_reloadversionntf.h"
#include "team/ptcc2m_teaminviteackc2m.h"
#include "team/ptcc2m_fmbrefusec2m.h"
#include "team/ptcg2m_teameventnotify.h"
#include "stage/ptcg2m_deprogresssyn.h"
#include "title/ptcg2m_getmaxtitlentf.h"
#include "guild/ptcg2m_reqguildsimpledata.h"
#include "guild/ptcg2m_synguildbosshurt.h"
#include "guild/ptcg2m_synaddguildactivity.h"
#include "guild/ptcc2m_gmfjoinbattlec2mres.h"
#include "guild/ptcg2m_gmfupdatedatag2m.h"
#include "guild/ptcg2m_synguildcontribute.h"
#include "guild/ptcc2m_guildcardrankreq.h"
#include "guild/ptcg2m_notifyscenepkwin.h"
#include "guild/ptcc2m_guildcardmatchreq.h"
#include "skycity/ptcg2m_skycityteamresultntf.h"
#include "chat/ptcc2m_openprivatechatntf.h"
#include "qa/ptcc2m_commitanswerntf.h"
#include "qa/ptcc2m_giveupqaquestionntf.h"
#include "broadcast/ptcg2m_broadcast.h"
#include "platfriend/ptcc2m_syncplatfriend2ms.h"
#include "scene/ptcg2m_scenecreated.h"
#include "login/ptcg2m_rolelogin.h"
#include "login/ptcg2m_rolelogout.h"
#include "role/ptcn2m_newrolecreated.h"
#include "scene/ptcg2m_rolechangescenentf.h"
#include "scene/ptcg2m_enterscenereqtoms.h"
#include "reswar/ptcg2m_reswarteamresultntf.h"
#include "reswar/ptcc2m_queryreswarrequet.h"
#include "garden/ptcc2m_gardenfishstop.h"
#include "pvp/ptck2m_updateworldmatchnum.h"
#include "cross/ptck2m_crosszonestatusntf.h"
#include "gm/ptcp2m_updateplatnoticemsntf.h"
#include "common/ptcn2m_updatefunctionopen2ms.h"
#include "guild/ptcg2m_noticeguildinheritend.h"
#include "common/ptcg2m_mscrashrecoverg2m.h"
#include "chat/ptcc2m_leavelargeroom.h"
#include "battlewatch/ptcg2m_syncallliveinfotoms.h"
#include "chat/ptcf2m_fmwhiterolentf.h"
#include "chat/ptcf2m_largeroomradiontf.h"
#include "common/ptcg2m_enterhallnotice.h"
#include "common/ptcr2m_crossgsclosentf2ms.h"
#include "chat/ptcc2m_handlemicphone.h"
#include "chat/ptcf2m_fmlargeroomcloseparmntf.h"
#include "scene/ptcg2m_sceneemptynotify.h"
#include "task/ptcg2m_removeaskitemntf.h"
#include "partner/ptcg2m_addpartnerlivenessg2m.h"
#include "pokertournament/ptcc2m_pokertournamentreq.h"
#include "guildcastle/ptcg2m_gcfsyng2m.h"
#include "pay/ptcc2m_payparameterinfontf.h"
#include "pay/ptcg2m_payadddiamondntf.h"
#include "pay/ptcg2m_payconsumentf.h"
#include "chat/ptcf2m_audiochanneltoms.h"
#include "leaguebattle/ptcg2m_leaguebattleupdatedatag2m.h"
#include "server/ptci2m_serverinfontf.h"
#include "leaguebattle/ptcg2m_leagueeliminationupdateg2m.h"
#include "league/ptcc2m_closeleagueelentf.h"
#include "chat/ptcf2m_fmwhitelistreloadntf.h"
#include "guild/ptcg2m_synguildbossattr.h"
#include "role/ptcg2m_levelchangentfms.h"
#include "pk/ptck2m_pkmatchinworldntf.h"
#include "leaguebattle/ptck2m_synccrossleagueteam2ms.h"
#include "common/ptck2m_syncrolesummarytoms.h"
#include "league/ptck2m_leaguematchworldresultntf.h"
#include "herobattle/ptck2m_heroworldroomntf.h"
#include "common/ptck2m_syncservertime2ms.h"
#include "leaguebattle/ptck2m_leaguecrosselintfms.h"
#include "leaguebattle/ptck2m_leaguecrossrewardntf.h"
#include "role/ptcg2m_opensystemntfms.h"
#include "common/ptck2m_broadcastworld2ms.h"
#include "common/ptcf2m_fmmasterinfontf.h"
#include "gm/ptcp2m_kickaccountntf.h"
#include "idip/ptci2m_banaccountlistntf.h"
#include "idip/ptci2m_delbanaccountntf.h"
#include "platfriend/ptcs2m_s2minvfightmsgntf.h"
#include "invfight/ptck2m_invfightcrossntf.h"
#include "common/ptcg2m_updateglobalvalue.h"
#include "pay/ptcc2m_paybuygoodsfailntf.h"
#include "pay/ptcc2m_ibgifttipsreq.h"
#include "skycraft/ptcc2m_closeskycraftelintf.h"
#include "scene/ptck2m_changesceneworldreq.h"
#include "custombattle/ptcg2m_reportcustombattleresult.h"
#include "team/ptcs2m_teamrequestplatfreindconfirm.h"
#include "match/ptck2m_kmatchedk2mntf.h"
#include "battle/ptcg2m_reportbadplayer2ms.h"
#include "team/ptcg2m_tarjainfontf.h"
#include "team/ptcg2m_sceneteamresultntf.h"
#include "team/ptcg2m_teamcostleaderresetntf.h"
#include "mobabattle/ptcg2m_addmobabattlerecord.h"
#include "session/ptcg2m_clientsessionchangeg2m.h"
#include "login/ptcg2m_roleloginreconnectntf.h"
#include "bigmelee/ptck2m_bmcrossntfdatantf.h"
#include "bigmelee/ptck2m_enterbmfightscenentf.h"
#include "mayhem/ptck2m_mayhemawardntf.h"
#include "mayhem/ptck2m_mayhembattlefieldreport.h"
#include "team/ptcg2m_teamtrangs2ms.h"
#include "team/ptch2m_teamtrants2ms.h"
#include "wedding/ptcg2m_updateweddingstateg2m.h"
#include "chat/ptcg2m_privatechat.h"
#include "invfight/ptcg2m_invfightagainreqg2m.h"
#include "battlefield/ptck2m_enterbffightscenentf.h"
#include "dragonguild/ptcg2m_reqdragonguildsimpledata.h"
#include "dragonguild/ptcg2m_costdragonguildreceivecount.h"
#include "dragonguild/ptcg2m_checkdragonguildtask.h"
#include "battlefield/ptcc2m_gobackreadyscenentf.h"
#include "goalawards/ptcg2m_goalawardsserverevent.h"
#include "battlefield/ptck2m_battlefieldreadyscenentf.h"
#include "battlefield/ptck2m_battlefailinfomsntf.h"
#include "goalawards/ptcg2m_goalawardsserverteam.h"
#include "skycity/ptcc2m_mulactivityiconsysreq.h"
#include "kickaccount/ptcg2m_kickaccountjkydg2m.h"
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
#include "mail/rpcc2m_fetchmail.h"
#include "mail/rpcc2m_mailop.h"
#include "qa/rpcc2m_getqadatareq.h"
#include "common/rpcg2m_registergs2ms.h"
#include "common/rpct2m_registergt2ms.h"
#include "login/rpcm2d_readaccountdata.h"
#include "role/rpcm2d_readroledata.h"
#include "gm/rpcp2m_gmhttpcommandnew.h"
#include "gm/rpcm2g_gshttpcommandnew.h"
#include "role/rpcm2d_mssaveroledata.h"
#include "role/rpcm2d_msgetallrole.h"
#include "scene/rpcm2n_createbattlescene.h"
#include "chat/rpcc2m_chat.h"
#include "chat/rpcm2g_chatcansend.h"
#include "battlewatch/rpcg2m_getwatchinfobytype.h"
#include "battlewatch/rpcg2m_enterwatchbattlems.h"
#include "friend/rpcc2m_doaddfriendnew.h"
#include "friend/rpcc2m_removefriendnew.h"
#include "friend/rpcc2m_addblacklistnew.h"
#include "friend/rpcc2m_removeblacklistnew.h"
#include "friend/rpcc2m_friendgiftopnew.h"
#include "friend/rpcc2m_randomfriendwaitlistnew.h"
#include "levelseal/rpcg2m_inclevelsealbosscount.h"
#include "guild/rpcm2g_noticeguildbossdoendtogsn.h"
#include "worldboss/rpcc2m_getworldbossstatenew.h"
#include "worldboss/rpcc2m_getworldbosstimeleft.h"
#include "social/rpcc2m_showflowerpagenew.h"
#include "social/rpcg2m_msgetflwoerrankreward.h"
#include "social/rpcg2m_mssendflower.h"
#include "social/rpcc2m_msgetflowerrewardlist.h"
#include "social/rpcm2g_getflowerrewardstate.h"
#include "team/rpcc2m_fetchteamlistc2m.h"
#include "team/rpcm2g_teamsynm2c.h"
#include "team/rpcc2m_invhistoryc2mreq.h"
#include "team/rpcc2m_teamrequestc2m.h"
#include "team/rpcm2g_teamstartbattlem2g.h"
#include "team/rpcm2g_teamrequestm2g.h"
#include "firstpass/rpcg2m_mscommendfirstpass.h"
#include "firstpass/rpcg2m_getrankinfo.h"
#include "pvp/rpcg2m_pvpallg2mreq.h"
#include "ranklist/rpcc2m_clientqueryranklistntf.h"
#include "common/rpcg2m_gmcommandg2m.h"
#include "team/rpcm2g_msusesummaryreq.h"
#include "team/rpcm2g_teamchangeexpcheck.h"
#include "ranklist/rpcg2m_updatemsranklist.h"
#include "friend/rpcc2m_addfriendnew.h"
#include "pk/rpcg2m_pkrankreq.h"
#include "common/rpco2m_gmtoolcommand2ms.h"
#include "pk/rpcc2m_pkreqc2m.h"
#include "pk/rpcm2g_pkreqm2g.h"
#include "chat/rpcc2m_clearprivatechatlist.h"
#include "guild/rpcc2m_askguildbriefinfo.h"
#include "guild/rpcc2m_createorenterguild.h"
#include "guild/rpcm2g_createguildcost.h"
#include "guild/rpcc2m_leavefromguild.h"
#include "auction/rpcc2m_auctionallreq.h"
#include "auction/rpcm2g_auctionallm2greq.h"
#include "guild/rpcm2g_enterguildpower.h"
#include "guild/rpcc2m_reqguildlist.h"
#include "scene/rpcc2m_derankreq.h"
#include "worldboss/rpcc2m_worldbossend.h"
#include "guild/rpcc2m_reqguildbossinfo.h"
#include "guild/rpcc2m_askguildmembers.h"
#include "guild/rpcc2m_askguildskillinfonew.h"
#include "guild/rpcc2m_studyguildskillnew.h"
#include "guild/rpcg2m_opguildreqdata.h"
#include "arena/rpcm2g_startarenags.h"
#include "role/rpcm2g_reqguilddarerewardrec.h"
#include "guild/rpcg2m_checkguilddarereward.h"
#include "arena/rpcg2m_endareanms.h"
#include "guild/rpcm2g_reqroleguildskill.h"
#include "ranklist/rpcc2m_mtshowtoplist.h"
#include "guild/rpcc2m_getguildbosstimeleft.h"
#include "guild/rpcc2m_fetchguildapp.h"
#include "guild/rpcc2m_guildapprovalnew.h"
#include "guild/rpcg2m_mguildbonusoperatereq.h"
#include "guild/rpcc2m_changeguildsettingnew.h"
#include "guild/rpcm2g_reqguildactivity.h"
#include "guild/rpcc2m_getguildcheckinrecordsnew.h"
#include "guild/rpcc2m_guildcheckinnew.h"
#include "guild/rpcc2m_queryguildcheckinnew.h"
#include "guild/rpcm2g_reqguildcheckin.h"
#include "guild/rpcm2g_noticeguildcheckin.h"
#include "guild/rpcg2m_notifycheckinbox.h"
#include "guild/rpcg2m_guildcardsyncms.h"
#include "guild/rpcm2g_reqrecvfatigue.h"
#include "guild/rpcc2m_guildfatigueopnew.h"
#include "guild/rpcc2m_fetchguildhistorynew.h"
#include "guild/rpcc2m_changememberpositionnew.h"
#include "designation/rpcm2g_designationeventreq.h"
#include "guild/rpcc2m_getotherguildbriefnew.h"
#include "guild/rpcc2m_askguildarenainfonew.h"
#include "guild/rpcc2m_askguildarenateaminfonew.h"
#include "guild/rpcc2m_adjustguildarenaroleposnew.h"
#include "guild/rpcc2m_guildarenasimpledeploynew.h"
#include "common/rpcm2d_dbreload.h"
#include "common/rpcm2g_gsreload.h"
#include "guild/rpcg2m_reqguildladderreward.h"
#include "guild/rpcc2m_reqguildladderinfo.h"
#include "guild/rpcc2m_reqguildladderrnakinfo.h"
#include "garden/rpcc2m_startplant.h"
#include "garden/rpcc2m_fetchplantinfo.h"
#include "garden/rpcc2m_plantcultivation.h"
#include "garden/rpcc2m_plantharvest.h"
#include "garden/rpcc2m_tryfish.h"
#include "common/rpcm2g_msconsumeitems.h"
#include "garden/rpcc2m_friendgardenplantlog.h"
#include "garden/rpcc2m_gardencookingfood.h"
#include "skycity/rpcc2m_skycityenter.h"
#include "qa/rpcg2m_enterqaroom.h"
#include "qa/rpcg2m_createguildqaroom.h"
#include "guild/rpcc2m_reqguildrankinfo.h"
#include "guild/rpcc2m_getguildbindinfo.h"
#include "guild/rpcc2m_guildbindgroup.h"
#include "guild/rpcc2m_guildjoinbindgroup.h"
#include "guild/rpcc2m_guildunbindgroup.h"
#include "guild/rpcc2m_gmfjoinreq.h"
#include "garden/rpcc2m_gardenbanquet.h"
#include "garden/rpcm2g_gardenplayevent.h"
#include "garden/rpcc2m_gardenoverview.h"
#include "garden/rpcm2g_gardenplantchangestage.h"
#include "guild/rpcm2g_getguildladderrewardtime.h"
#include "garden/rpcc2m_gardenbanquetaward.h"
#include "garden/rpcc2m_activecookbook.h"
#include "garden/rpcc2m_gardenfishinfo.h"
#include "pay/rpcc2m_paynotify.h"
#include "garden/rpcc2m_gardensteal.h"
#include "garden/rpcc2m_gardenexpelsprite.h"
#include "platfriend/rpcc2m_reqplatfriendranklist.h"
#include "platfriend/rpcm2s_m2sreqplatuserinfo.h"
#include "platfriend/rpcm2s_m2sloginandoutsync.h"
#include "platfriend/rpcm2s_m2ssendgift2platfriend.h"
#include "platfriend/rpcs2m_s2msendgift2platfriend.h"
#include "platfriend/rpcc2m_sendgift2platfriend.h"
#include "guild/rpcm2g_queryrolecardmatch.h"
#include "guild/rpcc2m_askguildwageinfo.h"
#include "guild/rpcc2m_getguildwagereward.h"
#include "garden/rpcc2m_opengardenfarmland.h"
#include "common/rpcm2r_registerms2router.h"
#include "common/rpcm2k_registerms2world.h"
#include "reswar/rpcc2m_reswarexplore.h"
#include "reswar/rpcc2m_queryreswar.h"
#include "crossscene/rpcm2k_getsceneinfo.h"
#include "login/rpcm2n_kickaccountfromms.h"
#include "scene/rpcm2n_changescenefromms.h"
#include "auction/rpcc2m_guildauctreqall.h"
#include "mail/rpcm2g_givemailprize.h"
#include "auction/rpcm2g_guildauctreqm2c.h"
#include "guild/rpcc2m_getguildbonussendlist.h"
#include "guild/rpcc2m_sendguildbonusinsendlist.h"
#include "reswar/rpcc2m_useguildbuff.h"
#include "reswar/rpcc2m_startreswarpve.h"
#include "guild/rpcc2m_applyguildarena.h"
#include "guild/rpcc2m_getapplyguildlist.h"
#include "guild/rpcc2m_getintegralbattleinfo.h"
#include "auction/rpcm2g_guildauctfailback.h"
#include "designation/rpcg2m_gsdesignationeventreq.h"
#include "guild/rpcc2m_getguildintegralinfo.h"
#include "guildcamp/rpcc2m_guildcampinfo.h"
#include "common/rpcm2i_registerms2login.h"
#include "reswar/rpcc2m_queryreswarrolerank.h"
#include "role/rpcn2m_checkrolename.h"
#include "role/rpcn2m_checkrolebaninfo.h"
#include "guild/rpcc2m_reqguildarenahistory.h"
#include "task/rpcg2m_askguildhelp.h"
#include "task/rpcc2m_getguilddonateinfo.h"
#include "task/rpcc2m_donatememberitem.h"
#include "task/rpcm2g_costdonateitem.h"
#include "task/rpcm2g_querydonateinfo.h"
#include "common/rpcm2n_nsreload.h"
#include "guild/rpcc2m_addguildinherit.h"
#include "guild/rpcm2g_tryaddguildinherit.h"
#include "guild/rpcc2m_aceptguildinherit.h"
#include "guild/rpcc2m_reqguildinheritinfo.h"
#include "guild/rpcc2m_delguildinherit.h"
#include "gm/rpcm2i_getplatnoticeinfo.h"
#include "platnotice/rpcc2m_fetchplatnotice.h"
#include "common/rpcm2g_crashrecoverm2g.h"
#include "role/rpcm2g_fetchroleinfoonms.h"
#include "chat/rpcc2m_joinfmroom.h"
#include "chat/rpcm2f_upjoinfmroom.h"
#include "team/rpcg2m_tryteamcostreward.h"
#include "role/rpcc2m_changenamenew.h"
#include "role/rpcm2g_changenamegs.h"
#include "guild/rpcm2g_reqguildinherittimes.h"
#include "db/rpcm2d_updatepartnerdata.h"
#include "partner/rpcm2g_makepartnerm2g.h"
#include "partner/rpcc2m_getpartnerinfo.h"
#include "mentorship/rpcc2m_getothermentorstatus.h"
#include "mentorship/rpcc2m_getmymentorinfo.h"
#include "mentorship/rpcc2m_getmyapplystudentinfo.h"
#include "mentorship/rpcc2m_getmyapplymasterinfo.h"
#include "mentorship/rpcc2m_mentormybeappliedmsg.h"
#include "mentorship/rpcc2m_updatementorapplystudentinfo.h"
#include "mentorship/rpcc2m_mentorrelationop.h"
#include "partner/rpcg2m_getpartnerinfog2m.h"
#include "partner/rpcm2g_getpartnerrecordm2g.h"
#include "partner/rpcc2m_getpartnerdetailinfo.h"
#include "partner/rpcc2m_getpartnerliveness.h"
#include "partner/rpcc2m_leavepartner.h"
#include "partner/rpcc2m_cancelleavepartner.h"
#include "invfight/rpcc2m_invfightreqall.h"
#include "guildcastle/rpcc2m_gcfcommonreq.h"
#include "guildcastle/rpcc2m_gcfreadysinforeq.h"
#include "photo/rpcg2m_getcharm.h"
#include "invfight/rpcm2g_invfightm2greq.h"
#include "partner/rpcc2m_getpartnershop.h"
#include "partner/rpcc2m_buypartnershopitem.h"
#include "partner/rpcm2g_buypartneritemm2g.h"
#include "pokertournament/rpcm2g_pokertournamentsignupcost.h"
#include "guild/rpcc2m_reqguildterrcityinfo.h"
#include "guild/rpcc2m_reqguildterrchallinfo.h"
#include "guild/rpcc2m_reqguildterrallianceinfo.h"
#include "partner/rpcc2m_getpartnershoprecord.h"
#include "guild/rpcc2m_allianceguildterr.h"
#include "guildcastle/rpcc2m_gcffightinforeqc2m.h"
#include "guild/rpcc2m_recalliance.h"
#include "guild/rpcc2m_tryalliance.h"
#include "guild/rpcc2m_clearguildterralliance.h"
#include "pay/rpcm2d_savepayaccessdata.h"
#include "pay/rpcm2g_paygetbalance.h"
#include "pay/rpcm2g_payconsumeaddgoods.h"
#include "role/rpcc2m_getunitappearancenew.h"
#include "castle/rpcm2g_gcfcommonreqm2g.h"
#include "db/rpcm2d_saveleagueteamdata.h"
#include "league/rpcc2m_getleaguebattleinfo.h"
#include "league/rpcc2m_getleagueteaminfo.h"
#include "garden/rpcg2m_getgardeninfo.h"
#include "league/rpcc2m_leaveleagueteam.h"
#include "league/rpcc2m_leaguebattlereq.h"
#include "subscribe/rpcc2m_getlistsubscribe.h"
#include "subscribe/rpcc2m_setsubscribe.h"
#include "leaguebattle/rpcg2m_leaguerankupdateg2m.h"
#include "role/rpcg2m_changeprofessionms.h"
#include "league/rpcc2m_getleaguebattlerecord.h"
#include "league/rpcc2m_getleagueeleinfo.h"
#include "league/rpcc2m_joinleagueelebattle.h"
#include "common/rpcm2s_registerms2cs.h"
#include "role/rpcg2m_personalcareertotalinfo.h"
#include "platnotice/rpcc2m_clicknewnotice.h"
#include "leaguebattle/rpcm2k_leaguereporttop2world.h"
#include "pk/rpcm2k_pkmatchtoworldreq.h"
#include "common/rpck2m_getrolesummaryfromms.h"
#include "misc/rpcm2g_checknotice.h"
#include "common/rpcm2f_registerms2fm.h"
#include "gm/rpcm2k_gmcommandm2w.h"
#include "guild/rpcc2m_reqguildterrintellinfo.h"
#include "activity/rpcg2m_getmulactopeninfo.h"
#include "militaryrank/rpcg2m_getmilitaryrank.h"
#include "pay/rpcg2m_uppayfrienditem.h"
#include "spactivity/rpcg2m_syncactivityend2ms.h"
#include "common/rpcm2d_mssavedbend.h"
#include "common/rpcm2k_queryrouter.h"
#include "ranklist/rpcc2m_queryranklistlua.h"
#include "invfight/rpcm2s_invfightcross2csreq.h"
#include "invfight/rpcs2m_invfightcross2msreq.h"
#include "scene/rpcm2k_createcrossbattlescene.h"
#include "scene/rpck2m_notifyentercrossbattle2ms.h"
#include "worldboss/rpcc2m_worldbossguildaddattr.h"
#include "worldboss/rpcm2g_worldbossguildaddattrgs.h"
#include "guild/rpcc2m_modifymsguildname.h"
#include "guild/rpcm2g_modifygsguildname.h"
#include "role/rpcm2d_queryrolesummary.h"
#include "garden/rpcm2d_querygardeninfo.h"
#include "ibshop/rpcm2g_giftibitemreq.h"
#include "pay/rpcm2d_savegiftibitem.h"
#include "arenastar/rpcm2d_arenastarroledatasave.h"
#include "arenastar/rpcc2m_arenastarrolereq.h"
#include "db/rpcm2d_skyteamdata2db.h"
#include "skyteam/rpcc2m_leaveskyteam.h"
#include "skycraft/rpcc2m_skycraftmatchreq.h"
#include "custombattle/rpcc2m_custombattleop.h"
#include "custombattle/rpcm2k_custombattleworldop.h"
#include "skycraft/rpcc2m_getskycraftinfo.h"
#include "skycraft/rpcc2m_getskycraftrecord.h"
#include "skycraft/rpcc2m_getskycrafteliinfo.h"
#include "skycraft/rpcc2m_getskycraftteaminfo.h"
#include "pay/rpcm2d_savegiftibhist.h"
#include "pay/rpcc2m_ibgifthistreq.h"
#include "pay/rpcg2m_paydegreereq.h"
#include "custombattle/rpcm2g_custombattlegsop.h"
#include "pay/rpcc2m_giftibreqgoods.h"
#include "pay/rpcm2g_giftibreqgsgoods.h"
#include "custombattle/rpcm2k_custombattlecheckvalid.h"
#include "custombattle/rpcg2m_custombattleresultms.h"
#include "common/rpcm2h_registerms2team.h"
#include "team/rpcm2h_teamtranms2ts.h"
#include "team/rpch2m_teamtrants2ms.h"
#include "team/rpcm2g_teamtranms2gs.h"
#include "team/rpcm2s_teamrequestplatfreindm2s.h"
#include "team/rpcs2m_teamrequestplatfreinds2m.h"
#include "match/rpcc2m_kmatchcommonreq.h"
#include "match/rpcm2k_kmatchreqm2k.h"
#include "match/rpcm2g_kmatchcommonreqm2c.h"
#include "db/rpcm2d_updaterolereportdata.h"
#include "spactivity/rpcc2m_getfloweractivityreward.h"
#include "common/rpcm2n_registerms2ns.h"
#include "guild/rpcm2g_tryteacherinheritnew.h"
#include "mobabattle/rpcc2m_getmobabattlegamerecord.h"
#include "mobabattle/rpcc2m_getmobabattlebriefrecord.h"
#include "mobabattle/rpcm2d_querymobarolerecord.h"
#include "bigmelee/rpcc2m_enterbmreadyscene.h"
#include "bigmelee/rpcc2m_querybigmeleerank.h"
#include "bigmelee/rpcm2k_querybigmeleerank2k.h"
#include "common/rpcm2g_msgiveitem.h"
#include "mobabattle/rpcc2m_getmobabattleinfo.h"
#include "mobabattle/rpcc2m_getmobabattleweekreward.h"
#include "team/rpck2m_teamstartbattlew2m.h"
#include "team/rpcm2g_teamstartbattlem2gcross.h"
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
#include "groupchat/rpcc2m_groupchatclear.h"
#include "wedding/rpcc2m_holdwedding.h"
#include "wedding/rpcc2m_getallweddinginfo.h"
#include "wedding/rpcc2m_getweddinginviteinfo.h"
#include "wedding/rpcc2m_enterweddingscene.h"
#include "marriage/rpcc2m_getmarriagerelation.h"
#include "marriage/rpcc2m_marriagerelationop.h"
#include "wedding/rpcc2m_weddinginviteoperator.h"
#include "groupchat/rpcc2m_groupchatleaderaddrole.h"
#include "marriage/rpcm2g_checkmarriagecouple.h"
#include "groupchat/rpcc2m_groupchatchangename.h"
#include "marriage/rpcc2m_getmarriageliveness.h"
#include "marriage/rpcc2m_takemarriagechest.h"
#include "pay/rpcc2m_checkrolebeforepay.h"
#include "marriage/rpcm2g_givemarriagelvrerewardm2g.h"
#include "wedding/rpcm2g_holdweddingcheckm2g.h"
#include "wedding/rpcc2m_startweddingcar.h"
#include "wedding/rpcm2g_weddingoperm2g.h"
#include "gm/rpcm2n_gmdowhitelist.h"
#include "battlefield/rpcc2m_enterbattlereadyscene.h"
#include "dragonguild/rpcc2m_getdragonguildliveness.h"
#include "dragonguild/rpcm2g_createdragonguildcost.h"
#include "dragonguild/rpcm2g_getrolepower.h"
#include "dragonguild/rpcc2m_createorjoindragonguild.h"
#include "dragonguild/rpcm2g_getdragonguildrecordm2g.h"
#include "dragonguild/rpcg2m_getdragonguildinfog2m.h"
#include "dragonguild/rpcc2m_fetchdgapps.h"
#include "dragonguild/rpcc2m_changedragonguildsetting.h"
#include "dragonguild/rpcc2m_dragonguildapproval.h"
#include "dragonguild/rpcc2m_leavefromdragonguild.h"
#include "dragonguild/rpcm2g_modifydragonguildnamecost.h"
#include "dragonguild/rpcc2m_modifydragonguildname.h"
#include "dragonguild/rpcc2m_askdragonguildmembers.h"
#include "dragonguild/rpcc2m_fetchdragonguildlist.h"
#include "dragonguild/rpcc2m_getdragonguildtaskinfo.h"
#include "dragonguild/rpcg2m_getdragonguildtaskchestg2m.h"
#include "dragonguild/rpcm2g_getdragonguildrecieveinfo.h"
#include "dragonguild/rpcc2m_buydragonguildshopitem.h"
#include "dragonguild/rpcc2m_getdragonguildshop.h"
#include "dragonguild/rpcm2g_buydragonguilditemm2g.h"
#include "dragonguild/rpcc2m_getdragonguildshoprecord.h"
#include "dragonguild/rpcc2m_changedragonguildposition.h"
#include "dragonguild/rpcc2m_getdragonguildbindinfo.h"
#include "dragonguild/rpcc2m_dragonguildbindgroupreq.h"
#include "dragonguild/rpcc2m_dragonguildjoinbindgroup.h"
#include "dragonguild/rpcc2m_dragonguildunbindgroup.h"
#include "dragonguild/rpcg2m_checkdragonguildredpointg2m.h"
#include "goalawards/rpcc2m_goalawardsgetlist.h"
#include "goalawards/rpcc2m_goalawardsgetawards.h"
#include "task/rpcg2m_taskdonateg2m.h"
#include "npcfeeling/rpcc2m_npcflreqc2m.h"
#include "npcfeeling/rpcm2g_npcflreqm2g.h"
#include "task/rpcc2m_getdailytaskrefreshinfo.h"
#include "task/rpcc2m_getdailytaskaskhelp.h"
#include "task/rpcc2m_getdailytaskrefreshrecord.h"
#include "task/rpcc2m_dailytaskrefreshoper.h"
#include "task/rpcm2g_dailytaskrefreshm2g.h"
#include "task/rpcg2m_dailytaskrefreshg2m.h"
#include "task/rpcm2d_updateroletaskdata.h"
#include "survive/rpcg2m_survivesynpointg2m.h"
#include "task/rpcg2m_getdailytaskg2m.h"
#include "activity/rpcg2m_getnewzonebenefitg2m.h"
#include "role/rpcm2i_getzoneroleinfom2i.h"
#include "role/rpci2m_getzoneroleinfoi2m.h"
#include "role/rpcm2d_readaccountdatanew.h"
#include "mail/rpcm2d_readmailfromdb.h"


static BaseLink *FindLink(const char *linkName)
{
	return Config::GetConfig()->FindLinkHandler(linkName);
}

void CProtocolRegister::Regiter() 
{
	CProtocol::Register(new PtcG2M_GMCloseQARoom());
	CProtocol::Register(new PtcG2M_UpdateRoleSummary());
	CProtocol::Register(new PtcG2M_SceneDestroyed());
	CProtocol::Register(new PtcG2M_NoticeForwardNtf());
	CProtocol::Register(new PtcG2M_SyncLiveInfoToMs());
	CProtocol::Register(new PtcG2M_RolesFinishSceneToMaster());
	CProtocol::Register(new PtcG2M_SceneEndToMaster());
	CProtocol::Register(new PtcG2M_UpdateGuildCampResult());
	CProtocol::Register(new PtcC2M_LoadOfflineChatNtf());
	CProtocol::Register(new PtcC2M_FriendQueryReportNew());
	CProtocol::Register(new PtcC2M_BlackListReportNew());
	CProtocol::Register(new PtcG2M_ReportWatchNtf());
	CProtocol::Register(new PtcG2M_GmLevelSealInfo());
	CProtocol::Register(new PtcG2M_AddMailGS());
	CProtocol::Register(new PtcC2M_RoleStateReportNew());
	CProtocol::Register(new PtcG2M_GMSendChat());
	CProtocol::Register(new PtcG2M_AddGuildBossAttr());
	CProtocol::Register(new PtcG2M_FirstEnterSceneNtf());
	CProtocol::Register(new PtcG2M_GmsetGuildBossTime());
	CProtocol::Register(new PtcG2M_SynSceneRewardG2M());
	CProtocol::Register(new PtcG2M_NoticeGuildBossDie());
	CProtocol::Register(new PtcG2M_CreateGuildBossSingle());
	CProtocol::Register(new PtcG2M_WorldBossSyncMS());
	CProtocol::Register(new PtcG2M_ReloadVersionNtf());
	CProtocol::Register(new PtcC2M_TeamInviteAckC2M());
	CProtocol::Register(new PtcC2M_FMBRefuseC2M());
	CProtocol::Register(new PtcG2M_TeamEventNotify());
	CProtocol::Register(new PtcG2M_DEProgressSyn());
	CProtocol::Register(new PtcG2M_GetMaxTitleNtf());
	CProtocol::Register(new PtcG2M_ReqGuildSimpleData());
	CProtocol::Register(new PtcG2M_SynGuildBossHurt());
	CProtocol::Register(new PtcG2M_SynAddGuildActivity());
	CProtocol::Register(new PtcC2M_GmfJoinBattleC2MRes());
	CProtocol::Register(new PtcG2M_GmfUpdateDataG2M());
	CProtocol::Register(new PtcG2M_SynGuildContribute());
	CProtocol::Register(new PtcC2M_GuildCardRankReq());
	CProtocol::Register(new PtcG2M_NotifyScenePkWin());
	CProtocol::Register(new PtcC2M_GuildCardMatchReq());
	CProtocol::Register(new PtcG2M_SkyCityTeamResultNtf());
	CProtocol::Register(new PtcC2M_OpenPrivateChatNtf());
	CProtocol::Register(new PtcC2M_CommitAnswerNtf());
	CProtocol::Register(new PtcC2M_GiveUpQAQuestionNtf());
	CProtocol::Register(new PtcG2M_BroadCast());
	CProtocol::Register(new PtcC2M_SyncPlatFriend2MS());
	CProtocol::Register(new PtcG2M_SceneCreated());
	CProtocol::Register(new PtcG2M_RoleLogin());
	CProtocol::Register(new PtcG2M_RoleLogout());
	CProtocol::Register(new PtcN2M_NewRoleCreated());
	CProtocol::Register(new PtcG2M_RoleChangeSceneNtf());
	CProtocol::Register(new PtcG2M_EnterSceneReqToMs());
	CProtocol::Register(new PtcG2M_ResWarTeamResultNtf());
	CProtocol::Register(new PtcC2M_QueryResWarRequet());
	CProtocol::Register(new PtcC2M_GardenFishStop());
	CProtocol::Register(new PtcK2M_UpdateWorldMatchNum());
	CProtocol::Register(new PtcK2M_CrossZoneStatusNtf());
	CProtocol::Register(new PtcP2M_UpdatePlatNoticeMsNtf());
	CProtocol::Register(new PtcN2M_UpdateFunctionOpen2Ms());
	CProtocol::Register(new PtcG2M_NoticeGuildInheritEnd());
	CProtocol::Register(new PtcG2M_mscrashrecoverg2m());
	CProtocol::Register(new PtcC2M_LeaveLargeRoom());
	CProtocol::Register(new PtcG2M_SyncAllLiveInfoToMs());
	CProtocol::Register(new PtcF2M_FMWhiteRoleNtf());
	CProtocol::Register(new PtcF2M_LargeRoomRadioNtf());
	CProtocol::Register(new PtcG2M_EnterHallNotice());
	CProtocol::Register(new PtcR2M_CrossGsCloseNtf2Ms());
	CProtocol::Register(new PtcC2M_HandleMicphone());
	CProtocol::Register(new PtcF2M_FMLargeRoomCloseParmNtf());
	CProtocol::Register(new PtcG2M_SceneEmptyNotify());
	CProtocol::Register(new PtcG2M_RemoveAskItemNtf());
	CProtocol::Register(new PtcG2M_AddPartnerLivenessG2M());
	CProtocol::Register(new PtcC2M_PokerTournamentReq());
	CProtocol::Register(new PtcG2M_GCFSynG2M());
	CProtocol::Register(new PtcC2M_PayParameterInfoNtf());
	CProtocol::Register(new PtcG2M_PayAddDiamondNtf());
	CProtocol::Register(new PtcG2M_PayConsumeNtf());
	CProtocol::Register(new PtcF2M_AudioChannelToMs());
	CProtocol::Register(new PtcG2M_LeagueBattleUpdateDataG2M());
	CProtocol::Register(new PtcI2M_ServerInfoNtf());
	CProtocol::Register(new PtcG2M_LeagueEliminationUpdateG2M());
	CProtocol::Register(new PtcC2M_CloseLeagueEleNtf());
	CProtocol::Register(new PtcF2M_FMWhiteListReloadNtf());
	CProtocol::Register(new PtcG2M_SynGuildBossAttr());
	CProtocol::Register(new PtcG2M_LevelChangeNtfMs());
	CProtocol::Register(new PtcK2M_PkMatchInWorldNtf());
	CProtocol::Register(new PtcK2M_SyncCrossLeagueTeam2MS());
	CProtocol::Register(new PtcK2M_SyncRoleSummaryToMS());
	CProtocol::Register(new PtcK2M_LeagueMatchWorldResultNtf());
	CProtocol::Register(new PtcK2M_HeroWorldRoomNtf());
	CProtocol::Register(new PtcK2M_SyncServerTime2MS());
	CProtocol::Register(new PtcK2M_LeagueCrossEliNtfMs());
	CProtocol::Register(new PtcK2M_LeagueCrossRewardNtf());
	CProtocol::Register(new PtcG2M_OpenSystemNtfMs());
	CProtocol::Register(new PtcK2M_BroadCastWorld2Ms());
	CProtocol::Register(new PtcF2M_FmMasterInfoNtf());
	CProtocol::Register(new PtcP2M_KickAccountNtf());
	CProtocol::Register(new PtcI2M_BanAccountListNtf());
	CProtocol::Register(new PtcI2M_DelBanAccountNtf());
	CProtocol::Register(new PtcS2M_S2MInvFightMsgNtf());
	CProtocol::Register(new PtcK2M_InvFightCrossNtf());
	CProtocol::Register(new PtcG2M_UpdateGlobalValue());
	CProtocol::Register(new PtcC2M_PayBuyGoodsFailNtf());
	CProtocol::Register(new PtcC2M_IBGiftTipsReq());
	CProtocol::Register(new PtcC2M_CloseSkyCraftEliNtf());
	CProtocol::Register(new PtcK2M_ChangeSceneWorldReq());
	CProtocol::Register(new PtcG2M_ReportCustomBattleResult());
	CProtocol::Register(new PtcS2M_TeamRequestPlatFreindConfirm());
	CProtocol::Register(new PtcK2M_KMatchedk2mNtf());
	CProtocol::Register(new PtcG2M_ReportBadPlayer2Ms());
	CProtocol::Register(new PtcG2M_TarjaInfoNtf());
	CProtocol::Register(new PtcG2M_SceneTeamResultNtf());
	CProtocol::Register(new PtcG2M_TeamCostLeaderResetNtf());
	CProtocol::Register(new PtcG2M_AddMobaBattleRecord());
	CProtocol::Register(new PtcG2M_ClientSessionChangeG2M());
	CProtocol::Register(new PtcG2M_RoleLoginReconnectNtf());
	CProtocol::Register(new PtcK2M_BMCrossNtfDataNtf());
	CProtocol::Register(new PtcK2M_EnterBMFightSceneNtf());
	CProtocol::Register(new PtcK2M_MayhemAwardNtf());
	CProtocol::Register(new PtcK2M_MayhemBattlefieldReport());
	CProtocol::Register(new PtcG2M_TeamTranGs2Ms());
	CProtocol::Register(new PtcH2M_TeamTranTs2Ms());
	CProtocol::Register(new PtcG2M_UpdateWeddingStateG2M());
	CProtocol::Register(new PtcG2M_PrivateChat());
	CProtocol::Register(new PtcG2M_InvfightAgainReqG2M());
	CProtocol::Register(new PtcK2M_EnterBFFightSceneNtf());
	CProtocol::Register(new PtcG2M_reqdragonguildsimpledata());
	CProtocol::Register(new PtcG2M_CostDragonGuildReceiveCount());
	CProtocol::Register(new PtcG2M_CheckDragonGuildTask());
	CProtocol::Register(new PtcC2M_GoBackReadySceneNtf());
	CProtocol::Register(new PtcG2M_GoalAwardsServerEvent());
	CProtocol::Register(new PtcK2M_BattleFieldReadySceneNtf());
	CProtocol::Register(new PtcK2M_BattleFailInfoMsNtf());
	CProtocol::Register(new PtcG2M_GoalAwardsServerTeam());
	CProtocol::Register(new PtcC2M_MulActivityIconSysReq());
	CProtocol::Register(new PtcG2M_KickAccountJkydG2M());
	CRpc::RegistRpc(RpcC2M_FetchMail::CreateRpc());
	CRpc::RegistRpc(RpcC2M_MailOp::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetQADataReq::CreateRpc());
	CRpc::RegistRpc(RpcG2M_RegisterGs2Ms::CreateRpc());
	CRpc::RegistRpc(RpcT2M_RegisterGt2Ms::CreateRpc());
	CRpc::RegistRpc(RpcM2D_ReadAccountData::CreateRpc());
	CRpc::RegistRpc(RpcM2D_ReadRoleData::CreateRpc());
	CRpc::RegistRpc(RpcP2M_GMHttpCommandNew::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GSHttpCommandNew::CreateRpc());
	CRpc::RegistRpc(RpcM2D_MsSaveRoleData::CreateRpc());
	CRpc::RegistRpc(RpcM2D_MsGetAllRole::CreateRpc());
	CRpc::RegistRpc(RpcM2N_CreateBattleScene::CreateRpc());
	CRpc::RegistRpc(RpcC2M_chat::CreateRpc());
	CRpc::RegistRpc(RpcM2G_ChatCanSend::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GetWatchInfoByType::CreateRpc());
	CRpc::RegistRpc(RpcG2M_EnterWatchBattleMs::CreateRpc());
	CRpc::RegistRpc(RpcC2M_DoAddFriendNew::CreateRpc());
	CRpc::RegistRpc(RpcC2M_RemoveFriendNew::CreateRpc());
	CRpc::RegistRpc(RpcC2M_AddBlackListNew::CreateRpc());
	CRpc::RegistRpc(RpcC2M_RemoveBlackListNew::CreateRpc());
	CRpc::RegistRpc(RpcC2M_FriendGiftOpNew::CreateRpc());
	CRpc::RegistRpc(RpcC2M_RandomFriendWaitListNew::CreateRpc());
	CRpc::RegistRpc(RpcG2M_IncLevelSealBossCount::CreateRpc());
	CRpc::RegistRpc(RpcM2G_NoticeGuildBossDoEndToGSN::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetWorldBossStateNew::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetWorldBossTimeLeft::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ShowFlowerPageNew::CreateRpc());
	CRpc::RegistRpc(RpcG2M_MSGetFlwoerRankReward::CreateRpc());
	CRpc::RegistRpc(RpcG2M_MSSendFlower::CreateRpc());
	CRpc::RegistRpc(RpcC2M_MSGetFlowerRewardList::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GetFlowerRewardState::CreateRpc());
	CRpc::RegistRpc(RpcC2M_FetchTeamListC2M::CreateRpc());
	CRpc::RegistRpc(RpcM2G_TeamSynM2C::CreateRpc());
	CRpc::RegistRpc(RpcC2M_InvHistoryC2MReq::CreateRpc());
	CRpc::RegistRpc(RpcC2M_TeamRequestC2M::CreateRpc());
	CRpc::RegistRpc(RpcM2G_TeamStartBattleM2G::CreateRpc());
	CRpc::RegistRpc(RpcM2G_TeamRequestM2G::CreateRpc());
	CRpc::RegistRpc(RpcG2M_MSCommendFirstPass::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GetRankInfo::CreateRpc());
	CRpc::RegistRpc(RpcG2M_PvpAllG2MReq::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ClientQueryRankListNtf::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GMCommandG2M::CreateRpc());
	CRpc::RegistRpc(RpcM2G_MSUseSummaryReq::CreateRpc());
	CRpc::RegistRpc(RpcM2G_TeamChangeExpCheck::CreateRpc());
	CRpc::RegistRpc(RpcG2M_UpdateMSRankList::CreateRpc());
	CRpc::RegistRpc(RpcC2M_AddFriendNew::CreateRpc());
	CRpc::RegistRpc(RpcG2M_PkRankReq::CreateRpc());
	CRpc::RegistRpc(RpcO2M_GMToolCommand2Ms::CreateRpc());
	CRpc::RegistRpc(RpcC2M_PkReqC2M::CreateRpc());
	CRpc::RegistRpc(RpcM2G_PkReqM2G::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ClearPrivateChatList::CreateRpc());
	CRpc::RegistRpc(RpcC2M_AskGuildBriefInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_CreateOrEnterGuild::CreateRpc());
	CRpc::RegistRpc(RpcM2G_CreateGuildCost::CreateRpc());
	CRpc::RegistRpc(RpcC2M_LeaveFromGuild::CreateRpc());
	CRpc::RegistRpc(RpcC2M_AuctionAllReq::CreateRpc());
	CRpc::RegistRpc(RpcM2G_AuctionAllM2GReq::CreateRpc());
	CRpc::RegistRpc(RpcM2G_EnterGuildPower::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ReqGuildList::CreateRpc());
	CRpc::RegistRpc(RpcC2M_DERankReq::CreateRpc());
	CRpc::RegistRpc(RpcC2M_WorldBossEnd::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ReqGuildBossInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_AskGuildMembers::CreateRpc());
	CRpc::RegistRpc(RpcC2M_AskGuildSkillInfoNew::CreateRpc());
	CRpc::RegistRpc(RpcC2M_StudyGuildSkillNew::CreateRpc());
	CRpc::RegistRpc(RpcG2M_OpGuildReqData::CreateRpc());
	CRpc::RegistRpc(RpcM2G_StartArenaGs::CreateRpc());
	CRpc::RegistRpc(RpcM2G_ReqGuildDareRewardRec::CreateRpc());
	CRpc::RegistRpc(RpcG2M_CheckGuildDareReward::CreateRpc());
	CRpc::RegistRpc(RpcG2M_EndAreanMs::CreateRpc());
	CRpc::RegistRpc(RpcM2G_ReqRoleGuildSkill::CreateRpc());
	CRpc::RegistRpc(RpcC2M_MTShowTopList::CreateRpc());
	CRpc::RegistRpc(RpcC2M_getguildbosstimeleft::CreateRpc());
	CRpc::RegistRpc(RpcC2M_FetchGuildApp::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GuildApprovalNew::CreateRpc());
	CRpc::RegistRpc(RpcG2M_MGuildBonusOperateReq::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ChangeGuildSettingNew::CreateRpc());
	CRpc::RegistRpc(RpcM2G_ReqGuildActivity::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetGuildCheckinRecordsNew::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GuildCheckinNew::CreateRpc());
	CRpc::RegistRpc(RpcC2M_QueryGuildCheckinNew::CreateRpc());
	CRpc::RegistRpc(RpcM2G_ReqGuildCheckin::CreateRpc());
	CRpc::RegistRpc(RpcM2G_NoticeGuildCheckin::CreateRpc());
	CRpc::RegistRpc(RpcG2M_NotifyCheckinBox::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GuildCardSyncMs::CreateRpc());
	CRpc::RegistRpc(RpcM2G_ReqRecvFatigue::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GuildFatigueOPNew::CreateRpc());
	CRpc::RegistRpc(RpcC2M_FetchGuildHistoryNew::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ChangeMemberPositionNew::CreateRpc());
	CRpc::RegistRpc(RpcM2G_DesignationEventReq::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetOtherGuildBriefNew::CreateRpc());
	CRpc::RegistRpc(RpcC2M_AskGuildArenaInfoNew::CreateRpc());
	CRpc::RegistRpc(RpcC2M_AskGuildArenaTeamInfoNew::CreateRpc());
	CRpc::RegistRpc(RpcC2M_AdjustGuildArenaRolePosNew::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GuildArenaSimpleDeployNew::CreateRpc());
	CRpc::RegistRpc(RpcM2D_DbReload::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GsReload::CreateRpc());
	CRpc::RegistRpc(RpcG2M_ReqGuildLadderReward::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ReqGuildLadderInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ReqGuildLadderRnakInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_StartPlant::CreateRpc());
	CRpc::RegistRpc(RpcC2M_FetchPlantInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_PlantCultivation::CreateRpc());
	CRpc::RegistRpc(RpcC2M_PlantHarvest::CreateRpc());
	CRpc::RegistRpc(RpcC2M_TryFish::CreateRpc());
	CRpc::RegistRpc(RpcM2G_MSConsumeItems::CreateRpc());
	CRpc::RegistRpc(RpcC2M_FriendGardenPlantLog::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GardenCookingFood::CreateRpc());
	CRpc::RegistRpc(RpcC2M_SkyCityEnter::CreateRpc());
	CRpc::RegistRpc(RpcG2M_EnterQARoom::CreateRpc());
	CRpc::RegistRpc(RpcG2M_CreateGuildQARoom::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ReqGuildRankInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetGuildBindInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GuildBindGroup::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GuildJoinBindGroup::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GuildUnBindGroup::CreateRpc());
	CRpc::RegistRpc(RpcC2M_gmfjoinreq::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GardenBanquet::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GardenPlayEvent::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GardenOverview::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GardenPlantChangeStage::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GetGuildLadderRewardTime::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GardenBanquetAward::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ActiveCookbook::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GardenFishInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_PayNotify::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GardenSteal::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GardenExpelSprite::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ReqPlatFriendRankList::CreateRpc());
	CRpc::RegistRpc(RpcM2S_M2SReqPlatUserInfo::CreateRpc());
	CRpc::RegistRpc(RpcM2S_M2SLoginAndoutSync::CreateRpc());
	CRpc::RegistRpc(RpcM2S_M2SSendGift2PlatFriend::CreateRpc());
	CRpc::RegistRpc(RpcS2M_S2MSendGift2PlatFriend::CreateRpc());
	CRpc::RegistRpc(RpcC2M_SendGift2PlatFriend::CreateRpc());
	CRpc::RegistRpc(RpcM2G_QueryRoleCardMatch::CreateRpc());
	CRpc::RegistRpc(RpcC2M_AskGuildWageInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetGuildWageReward::CreateRpc());
	CRpc::RegistRpc(RpcC2M_OPenGardenFarmland::CreateRpc());
	CRpc::RegistRpc(RpcM2R_RegisterMs2Router::CreateRpc());
	CRpc::RegistRpc(RpcM2K_RegisterMs2World::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ResWarExplore::CreateRpc());
	CRpc::RegistRpc(RpcC2M_QueryResWar::CreateRpc());
	CRpc::RegistRpc(RpcM2K_GetSceneInfo::CreateRpc());
	CRpc::RegistRpc(RpcM2N_KickAccountFromMs::CreateRpc());
	CRpc::RegistRpc(RpcM2N_ChangeSceneFromMs::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GuildAuctReqAll::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GiveMailPrize::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GuildAuctReqM2C::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetGuildBonusSendList::CreateRpc());
	CRpc::RegistRpc(RpcC2M_SendGuildBonusInSendList::CreateRpc());
	CRpc::RegistRpc(RpcC2M_UseGuildBuff::CreateRpc());
	CRpc::RegistRpc(RpcC2M_StartResWarPVE::CreateRpc());
	CRpc::RegistRpc(RpcC2M_applyguildarena::CreateRpc());
	CRpc::RegistRpc(RpcC2M_getapplyguildlist::CreateRpc());
	CRpc::RegistRpc(RpcC2M_getintegralbattleInfo::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GuildAuctFailBack::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GSDesignationEventReq::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetGuildIntegralInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GuildCampInfo::CreateRpc());
	CRpc::RegistRpc(RpcM2I_RegisterMs2Login::CreateRpc());
	CRpc::RegistRpc(RpcC2M_QueryResWarRoleRank::CreateRpc());
	CRpc::RegistRpc(RpcN2M_CheckRoleName::CreateRpc());
	CRpc::RegistRpc(RpcN2M_CheckRoleBanInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ReqGuildArenaHistory::CreateRpc());
	CRpc::RegistRpc(RpcG2M_AskGuildHelp::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetGuildDonateInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_DonateMemberItem::CreateRpc());
	CRpc::RegistRpc(RpcM2G_CostDonateItem::CreateRpc());
	CRpc::RegistRpc(RpcM2G_QueryDonateInfo::CreateRpc());
	CRpc::RegistRpc(RpcM2N_NsReload::CreateRpc());
	CRpc::RegistRpc(RpcC2M_AddGuildInherit::CreateRpc());
	CRpc::RegistRpc(RpcM2G_tryAddGuildInherit::CreateRpc());
	CRpc::RegistRpc(RpcC2M_AceptGuildInherit::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ReqGuildInheritInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_DelGuildInherit::CreateRpc());
	CRpc::RegistRpc(RpcM2I_GetPlatNoticeInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_FetchPlatNotice::CreateRpc());
	CRpc::RegistRpc(RpcM2G_CrashRecoverM2G::CreateRpc());
	CRpc::RegistRpc(RpcM2G_FetchRoleInfoOnMs::CreateRpc());
	CRpc::RegistRpc(RpcC2M_JoinFmRoom::CreateRpc());
	CRpc::RegistRpc(RpcM2F_UPJoinFmRoom::CreateRpc());
	CRpc::RegistRpc(RpcG2M_TryTeamCostReward::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ChangeNameNew::CreateRpc());
	CRpc::RegistRpc(RpcM2G_ChangeNameGs::CreateRpc());
	CRpc::RegistRpc(RpcM2G_ReqGuildInheritTimes::CreateRpc());
	CRpc::RegistRpc(RpcM2D_UpdatePartnerData::CreateRpc());
	CRpc::RegistRpc(RpcM2G_MakePartnerM2G::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetPartnerInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetOtherMentorStatus::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetMyMentorInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetMyApplyStudentInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetMyApplyMasterInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_MentorMyBeAppliedMsg::CreateRpc());
	CRpc::RegistRpc(RpcC2M_UpdateMentorApplyStudentInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_MentorRelationOp::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GetPartnerInfoG2M::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GetPartnerRecordM2G::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetPartnerDetailInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetPartnerLiveness::CreateRpc());
	CRpc::RegistRpc(RpcC2M_LeavePartner::CreateRpc());
	CRpc::RegistRpc(RpcC2M_CancelLeavePartner::CreateRpc());
	CRpc::RegistRpc(RpcC2M_InvFightReqAll::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GCFCommonReq::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GCFReadysInfoReq::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GetCharm::CreateRpc());
	CRpc::RegistRpc(RpcM2G_InvFightM2GReq::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetPartnerShop::CreateRpc());
	CRpc::RegistRpc(RpcC2M_BuyPartnerShopItem::CreateRpc());
	CRpc::RegistRpc(RpcM2G_BuyPartnerItemM2G::CreateRpc());
	CRpc::RegistRpc(RpcM2G_PokerTournamentSignUpCost::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ReqGuildTerrCityInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ReqGuildTerrChallInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ReqGuildTerrAllianceInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetPartnerShopRecord::CreateRpc());
	CRpc::RegistRpc(RpcC2M_AllianceGuildTerr::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GCFFightInfoReqC2M::CreateRpc());
	CRpc::RegistRpc(RpcC2M_RecAlliance::CreateRpc());
	CRpc::RegistRpc(RpcC2M_TryAlliance::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ClearGuildTerrAlliance::CreateRpc());
	CRpc::RegistRpc(RpcM2D_SavePayAccessData::CreateRpc());
	CRpc::RegistRpc(RpcM2G_PayGetBalance::CreateRpc());
	CRpc::RegistRpc(RpcM2G_PayConsumeAddGoods::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetUnitAppearanceNew::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GCFCommonReqM2G::CreateRpc());
	CRpc::RegistRpc(RpcM2D_SaveLeagueTeamData::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetLeagueBattleInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetLeagueTeamInfo::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GetGardenInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_LeaveLeagueTeam::CreateRpc());
	CRpc::RegistRpc(RpcC2M_LeagueBattleReq::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetListSubscribe::CreateRpc());
	CRpc::RegistRpc(RpcC2M_SetSubscribe::CreateRpc());
	CRpc::RegistRpc(RpcG2M_LeagueRankUpdateG2M::CreateRpc());
	CRpc::RegistRpc(RpcG2M_ChangeProfessionMs::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetLeagueBattleRecord::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetLeagueEleInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_JoinLeagueEleBattle::CreateRpc());
	CRpc::RegistRpc(RpcM2S_RegisterMS2CS::CreateRpc());
	CRpc::RegistRpc(RpcG2M_PersonalCareerTotalInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ClickNewNotice::CreateRpc());
	CRpc::RegistRpc(RpcM2K_LeagueReportTop2World::CreateRpc());
	CRpc::RegistRpc(RpcM2K_PkMatchToWorldReq::CreateRpc());
	CRpc::RegistRpc(RpcK2M_GetRoleSummaryFromMS::CreateRpc());
	CRpc::RegistRpc(RpcM2G_CheckNotice::CreateRpc());
	CRpc::RegistRpc(RpcM2F_RegisterMs2Fm::CreateRpc());
	CRpc::RegistRpc(RpcM2K_GMCommandM2W::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ReqGuildTerrIntellInfo::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GetMulActOpenInfo::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GetMilitaryRank::CreateRpc());
	CRpc::RegistRpc(RpcG2M_UpPayFriendItem::CreateRpc());
	CRpc::RegistRpc(RpcG2M_SyncActivityEnd2MS::CreateRpc());
	CRpc::RegistRpc(RpcM2D_MsSaveDBEnd::CreateRpc());
	CRpc::RegistRpc(RpcM2K_QueryRouter::CreateRpc());
	CRpc::RegistRpc(RpcC2M_QueryRankListLua::CreateRpc());
	CRpc::RegistRpc(RpcM2S_InvFightCross2CSReq::CreateRpc());
	CRpc::RegistRpc(RpcS2M_InvFightCross2MSReq::CreateRpc());
	CRpc::RegistRpc(RpcM2K_CreateCrossBattleScene::CreateRpc());
	CRpc::RegistRpc(RpcK2M_NotifyEnterCrossBattle2MS::CreateRpc());
	CRpc::RegistRpc(RpcC2M_WorldBossGuildAddAttr::CreateRpc());
	CRpc::RegistRpc(RpcM2G_WorldBossGuildAddAttrGs::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ModifyMsGuildName::CreateRpc());
	CRpc::RegistRpc(RpcM2G_ModifyGsGuildName::CreateRpc());
	CRpc::RegistRpc(RpcM2D_QueryRolesummary::CreateRpc());
	CRpc::RegistRpc(RpcM2D_QueryGardenInfo::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GiftIbItemReq::CreateRpc());
	CRpc::RegistRpc(RpcM2D_SaveGiftIbItem::CreateRpc());
	CRpc::RegistRpc(RpcM2D_ArenaStarRoleDataSave::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ArenaStarRoleReq::CreateRpc());
	CRpc::RegistRpc(RpcM2D_SkyTeamData2DB::CreateRpc());
	CRpc::RegistRpc(RpcC2M_LeaveSkyTeam::CreateRpc());
	CRpc::RegistRpc(RpcC2M_SkyCraftMatchReq::CreateRpc());
	CRpc::RegistRpc(RpcC2M_CustomBattleOp::CreateRpc());
	CRpc::RegistRpc(RpcM2K_CustomBattleWorldOp::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetSkyCraftInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetSkyCraftRecord::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetSkyCraftEliInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetSkyCraftTeamInfo::CreateRpc());
	CRpc::RegistRpc(RpcM2D_SaveGiftIbHist::CreateRpc());
	CRpc::RegistRpc(RpcC2M_IbGiftHistReq::CreateRpc());
	CRpc::RegistRpc(RpcG2M_PayDegreeReq::CreateRpc());
	CRpc::RegistRpc(RpcM2G_CustomBattleGsOp::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GiftIbReqGoods::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GiftIbReqGSGoods::CreateRpc());
	CRpc::RegistRpc(RpcM2K_CustomBattleCheckValid::CreateRpc());
	CRpc::RegistRpc(RpcG2M_CustomBattleResultMs::CreateRpc());
	CRpc::RegistRpc(RpcM2H_RegisterMs2Team::CreateRpc());
	CRpc::RegistRpc(RpcM2H_TeamTranMs2Ts::CreateRpc());
	CRpc::RegistRpc(RpcH2M_TeamTranTs2Ms::CreateRpc());
	CRpc::RegistRpc(RpcM2G_TeamTranMs2Gs::CreateRpc());
	CRpc::RegistRpc(RpcM2S_TeamRequestPlatFreindM2S::CreateRpc());
	CRpc::RegistRpc(RpcS2M_TeamRequestPlatFreindS2M::CreateRpc());
	CRpc::RegistRpc(RpcC2M_KMatchCommonReq::CreateRpc());
	CRpc::RegistRpc(RpcM2K_KMatchReqm2k::CreateRpc());
	CRpc::RegistRpc(RpcM2G_KMatchCommonReqM2C::CreateRpc());
	CRpc::RegistRpc(RpcM2D_UpdateRoleReportData::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetFlowerActivityReward::CreateRpc());
	CRpc::RegistRpc(RpcM2N_registerms2ns::CreateRpc());
	CRpc::RegistRpc(RpcM2G_TryTeacherInheritNew::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetMobaBattleGameRecord::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetMobaBattleBriefRecord::CreateRpc());
	CRpc::RegistRpc(RpcM2D_QueryMobaRoleRecord::CreateRpc());
	CRpc::RegistRpc(RpcC2M_EnterBMReadyScene::CreateRpc());
	CRpc::RegistRpc(RpcC2M_QueryBigMeleeRank::CreateRpc());
	CRpc::RegistRpc(RpcM2K_QueryBigMeleeRank2k::CreateRpc());
	CRpc::RegistRpc(RpcM2G_MsGiveItem::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetMobaBattleInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetMobaBattleWeekReward::CreateRpc());
	CRpc::RegistRpc(RpcK2M_TeamStartBattleW2M::CreateRpc());
	CRpc::RegistRpc(RpcM2G_TeamStartBattleM2GCross::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GroupChatLeaderReviewList::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GroupChatFindRoleInfoList::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GroupChatFindTeamInfoList::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GroupChatPlayerIssueInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GroupChatLeaderIssueInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GroupChatLeaderReview::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GroupChatPlayerApply::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GroupChatGetGroupInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GroupChatManager::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GroupChatQuit::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GroupChatCreate::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GroupChatClear::CreateRpc());
	CRpc::RegistRpc(RpcC2M_HoldWedding::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetAllWeddingInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetWeddingInviteInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_EnterWeddingScene::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetMarriageRelation::CreateRpc());
	CRpc::RegistRpc(RpcC2M_MarriageRelationOp::CreateRpc());
	CRpc::RegistRpc(RpcC2M_WeddingInviteOperator::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GroupChatLeaderAddRole::CreateRpc());
	CRpc::RegistRpc(RpcM2G_CheckMarriageCouple::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GroupChatChangeName::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetMarriageLiveness::CreateRpc());
	CRpc::RegistRpc(RpcC2M_TakeMarriageChest::CreateRpc());
	CRpc::RegistRpc(RpcC2M_CheckRoleBeforePay::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GiveMarriageLvRerewardM2G::CreateRpc());
	CRpc::RegistRpc(RpcM2G_Holdweddingcheckm2g::CreateRpc());
	CRpc::RegistRpc(RpcC2M_StartWeddingCar::CreateRpc());
	CRpc::RegistRpc(RpcM2G_WeddingOperM2G::CreateRpc());
	CRpc::RegistRpc(RpcM2N_GMDoWhiteList::CreateRpc());
	CRpc::RegistRpc(RpcC2M_EnterBattleReadyScene::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetDragonGuildLiveness::CreateRpc());
	CRpc::RegistRpc(RpcM2G_CreateDragonGuildCost::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GetRolePower::CreateRpc());
	CRpc::RegistRpc(RpcC2M_CreateOrJoinDragonGuild::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GetDragonGuildRecordM2G::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GetDragonGuildInfoG2M::CreateRpc());
	CRpc::RegistRpc(RpcC2M_FetchDGApps::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ChangeDragonGuildSetting::CreateRpc());
	CRpc::RegistRpc(RpcC2M_DragonGuildApproval::CreateRpc());
	CRpc::RegistRpc(RpcC2M_LeaveFromDragonGuild::CreateRpc());
	CRpc::RegistRpc(RpcM2G_ModifyDragonGuildNameCost::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ModifyDragonGuildName::CreateRpc());
	CRpc::RegistRpc(RpcC2M_AskDragonGuildMembers::CreateRpc());
	CRpc::RegistRpc(RpcC2M_FetchDragonGuildList::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetDragonGuildTaskInfo::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GetDragonGuildTaskChestG2M::CreateRpc());
	CRpc::RegistRpc(RpcM2G_GetDragonGuildRecieveInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_BuyDragonGuildShopItem::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetDragonGuildShop::CreateRpc());
	CRpc::RegistRpc(RpcM2G_BuyDragonGuildItemM2G::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetDragonGuildShopRecord::CreateRpc());
	CRpc::RegistRpc(RpcC2M_ChangeDragonGuildPosition::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetDragonGuildBindInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_DragonGuildBindGroupReq::CreateRpc());
	CRpc::RegistRpc(RpcC2M_DragonGuildJoinBindGroup::CreateRpc());
	CRpc::RegistRpc(RpcC2M_DragonGuildUnBindGroup::CreateRpc());
	CRpc::RegistRpc(RpcG2M_CheckDragonGuildRedPointG2M::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GoalAwardsGetList::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GoalAwardsGetAwards::CreateRpc());
	CRpc::RegistRpc(RpcG2M_TaskDonateG2M::CreateRpc());
	CRpc::RegistRpc(RpcC2M_NpcFlReqC2M::CreateRpc());
	CRpc::RegistRpc(RpcM2G_NpcFlReqM2G::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetDailyTaskRefreshInfo::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetDailyTaskAskHelp::CreateRpc());
	CRpc::RegistRpc(RpcC2M_GetDailyTaskRefreshRecord::CreateRpc());
	CRpc::RegistRpc(RpcC2M_DailyTaskRefreshOper::CreateRpc());
	CRpc::RegistRpc(RpcM2G_DailyTaskRefreshM2G::CreateRpc());
	CRpc::RegistRpc(RpcG2M_DailyTaskRefreshG2M::CreateRpc());
	CRpc::RegistRpc(RpcM2D_UpdateRoleTaskData::CreateRpc());
	CRpc::RegistRpc(RpcG2M_SurviveSynPointG2M::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GetDailyTaskG2M::CreateRpc());
	CRpc::RegistRpc(RpcG2M_GetNewZoneBenefitG2M::CreateRpc());
	CRpc::RegistRpc(RpcM2I_GetZoneRoleInfoM2I::CreateRpc());
	CRpc::RegistRpc(RpcI2M_GetZoneRoleInfoI2M::CreateRpc());
	CRpc::RegistRpc(RpcM2D_ReadAccountDataNew::CreateRpc());
	CRpc::RegistRpc(RpcM2D_ReadMailFromDB::CreateRpc());


	CProtocol::RegistProtoName(PTCG2M_GMCLOSEQAROOM_TYPE, "PTCG2M_GMCLOSEQAROOM");
	CProtocol::RegistProtoName(PTCG2M_UPDATEROLESUMMARY_TYPE, "PTCG2M_UPDATEROLESUMMARY");
	CProtocol::RegistProtoName(PTCG2M_SCENEDESTROYED_TYPE, "PTCG2M_SCENEDESTROYED");
	CProtocol::RegistProtoName(PTCG2M_NOTICEFORWARDNTF_TYPE, "PTCG2M_NOTICEFORWARDNTF");
	CProtocol::RegistProtoName(PTCG2M_SYNCLIVEINFOTOMS_TYPE, "PTCG2M_SYNCLIVEINFOTOMS");
	CProtocol::RegistProtoName(PTCG2M_ROLESFINISHSCENETOMASTER_TYPE, "PTCG2M_ROLESFINISHSCENETOMASTER");
	CProtocol::RegistProtoName(PTCG2M_SCENEENDTOMASTER_TYPE, "PTCG2M_SCENEENDTOMASTER");
	CProtocol::RegistProtoName(PTCG2M_UPDATEGUILDCAMPRESULT_TYPE, "PTCG2M_UPDATEGUILDCAMPRESULT");
	CProtocol::RegistProtoName(PTCC2M_LOADOFFLINECHATNTF_TYPE, "PTCC2M_LOADOFFLINECHATNTF");
	CProtocol::RegistProtoName(PTCC2M_FRIENDQUERYREPORTNEW_TYPE, "PTCC2M_FRIENDQUERYREPORTNEW");
	CProtocol::RegistProtoName(PTCC2M_BLACKLISTREPORTNEW_TYPE, "PTCC2M_BLACKLISTREPORTNEW");
	CProtocol::RegistProtoName(PTCG2M_REPORTWATCHNTF_TYPE, "PTCG2M_REPORTWATCHNTF");
	CProtocol::RegistProtoName(PTCG2M_GMLEVELSEALINFO_TYPE, "PTCG2M_GMLEVELSEALINFO");
	CProtocol::RegistProtoName(PTCG2M_ADDMAILGS_TYPE, "PTCG2M_ADDMAILGS");
	CProtocol::RegistProtoName(PTCC2M_ROLESTATEREPORTNEW_TYPE, "PTCC2M_ROLESTATEREPORTNEW");
	CProtocol::RegistProtoName(PTCG2M_GMSENDCHAT_TYPE, "PTCG2M_GMSENDCHAT");
	CProtocol::RegistProtoName(PTCG2M_ADDGUILDBOSSATTR_TYPE, "PTCG2M_ADDGUILDBOSSATTR");
	CProtocol::RegistProtoName(PTCG2M_FIRSTENTERSCENENTF_TYPE, "PTCG2M_FIRSTENTERSCENENTF");
	CProtocol::RegistProtoName(PTCG2M_GMSETGUILDBOSSTIME_TYPE, "PTCG2M_GMSETGUILDBOSSTIME");
	CProtocol::RegistProtoName(PTCG2M_SYNSCENEREWARDG2M_TYPE, "PTCG2M_SYNSCENEREWARDG2M");
	CProtocol::RegistProtoName(PTCG2M_NOTICEGUILDBOSSDIE_TYPE, "PTCG2M_NOTICEGUILDBOSSDIE");
	CProtocol::RegistProtoName(PTCG2M_CREATEGUILDBOSSSINGLE_TYPE, "PTCG2M_CREATEGUILDBOSSSINGLE");
	CProtocol::RegistProtoName(PTCG2M_WORLDBOSSSYNCMS_TYPE, "PTCG2M_WORLDBOSSSYNCMS");
	CProtocol::RegistProtoName(PTCG2M_RELOADVERSIONNTF_TYPE, "PTCG2M_RELOADVERSIONNTF");
	CProtocol::RegistProtoName(PTCC2M_TEAMINVITEACKC2M_TYPE, "PTCC2M_TEAMINVITEACKC2M");
	CProtocol::RegistProtoName(PTCC2M_FMBREFUSEC2M_TYPE, "PTCC2M_FMBREFUSEC2M");
	CProtocol::RegistProtoName(PTCG2M_TEAMEVENTNOTIFY_TYPE, "PTCG2M_TEAMEVENTNOTIFY");
	CProtocol::RegistProtoName(PTCG2M_DEPROGRESSSYN_TYPE, "PTCG2M_DEPROGRESSSYN");
	CProtocol::RegistProtoName(PTCG2M_GETMAXTITLENTF_TYPE, "PTCG2M_GETMAXTITLENTF");
	CProtocol::RegistProtoName(PTCG2M_REQGUILDSIMPLEDATA_TYPE, "PTCG2M_REQGUILDSIMPLEDATA");
	CProtocol::RegistProtoName(PTCG2M_SYNGUILDBOSSHURT_TYPE, "PTCG2M_SYNGUILDBOSSHURT");
	CProtocol::RegistProtoName(PTCG2M_SYNADDGUILDACTIVITY_TYPE, "PTCG2M_SYNADDGUILDACTIVITY");
	CProtocol::RegistProtoName(PTCC2M_GMFJOINBATTLEC2MRES_TYPE, "PTCC2M_GMFJOINBATTLEC2MRES");
	CProtocol::RegistProtoName(PTCG2M_GMFUPDATEDATAG2M_TYPE, "PTCG2M_GMFUPDATEDATAG2M");
	CProtocol::RegistProtoName(PTCG2M_SYNGUILDCONTRIBUTE_TYPE, "PTCG2M_SYNGUILDCONTRIBUTE");
	CProtocol::RegistProtoName(PTCC2M_GUILDCARDRANKREQ_TYPE, "PTCC2M_GUILDCARDRANKREQ");
	CProtocol::RegistProtoName(PTCG2M_NOTIFYSCENEPKWIN_TYPE, "PTCG2M_NOTIFYSCENEPKWIN");
	CProtocol::RegistProtoName(PTCC2M_GUILDCARDMATCHREQ_TYPE, "PTCC2M_GUILDCARDMATCHREQ");
	CProtocol::RegistProtoName(PTCG2M_SKYCITYTEAMRESULTNTF_TYPE, "PTCG2M_SKYCITYTEAMRESULTNTF");
	CProtocol::RegistProtoName(PTCC2M_OPENPRIVATECHATNTF_TYPE, "PTCC2M_OPENPRIVATECHATNTF");
	CProtocol::RegistProtoName(PTCC2M_COMMITANSWERNTF_TYPE, "PTCC2M_COMMITANSWERNTF");
	CProtocol::RegistProtoName(PTCC2M_GIVEUPQAQUESTIONNTF_TYPE, "PTCC2M_GIVEUPQAQUESTIONNTF");
	CProtocol::RegistProtoName(PTCG2M_BROADCAST_TYPE, "PTCG2M_BROADCAST");
	CProtocol::RegistProtoName(PTCC2M_SYNCPLATFRIEND2MS_TYPE, "PTCC2M_SYNCPLATFRIEND2MS");
	CProtocol::RegistProtoName(PTCG2M_SCENECREATED_TYPE, "PTCG2M_SCENECREATED");
	CProtocol::RegistProtoName(PTCG2M_ROLELOGIN_TYPE, "PTCG2M_ROLELOGIN");
	CProtocol::RegistProtoName(PTCG2M_ROLELOGOUT_TYPE, "PTCG2M_ROLELOGOUT");
	CProtocol::RegistProtoName(PTCN2M_NEWROLECREATED_TYPE, "PTCN2M_NEWROLECREATED");
	CProtocol::RegistProtoName(PTCG2M_ROLECHANGESCENENTF_TYPE, "PTCG2M_ROLECHANGESCENENTF");
	CProtocol::RegistProtoName(PTCG2M_ENTERSCENEREQTOMS_TYPE, "PTCG2M_ENTERSCENEREQTOMS");
	CProtocol::RegistProtoName(PTCG2M_RESWARTEAMRESULTNTF_TYPE, "PTCG2M_RESWARTEAMRESULTNTF");
	CProtocol::RegistProtoName(PTCC2M_QUERYRESWARREQUET_TYPE, "PTCC2M_QUERYRESWARREQUET");
	CProtocol::RegistProtoName(PTCC2M_GARDENFISHSTOP_TYPE, "PTCC2M_GARDENFISHSTOP");
	CProtocol::RegistProtoName(PTCK2M_UPDATEWORLDMATCHNUM_TYPE, "PTCK2M_UPDATEWORLDMATCHNUM");
	CProtocol::RegistProtoName(PTCK2M_CROSSZONESTATUSNTF_TYPE, "PTCK2M_CROSSZONESTATUSNTF");
	CProtocol::RegistProtoName(PTCP2M_UPDATEPLATNOTICEMSNTF_TYPE, "PTCP2M_UPDATEPLATNOTICEMSNTF");
	CProtocol::RegistProtoName(PTCN2M_UPDATEFUNCTIONOPEN2MS_TYPE, "PTCN2M_UPDATEFUNCTIONOPEN2MS");
	CProtocol::RegistProtoName(PTCG2M_NOTICEGUILDINHERITEND_TYPE, "PTCG2M_NOTICEGUILDINHERITEND");
	CProtocol::RegistProtoName(PTCG2M_MSCRASHRECOVERG2M_TYPE, "PTCG2M_MSCRASHRECOVERG2M");
	CProtocol::RegistProtoName(PTCC2M_LEAVELARGEROOM_TYPE, "PTCC2M_LEAVELARGEROOM");
	CProtocol::RegistProtoName(PTCG2M_SYNCALLLIVEINFOTOMS_TYPE, "PTCG2M_SYNCALLLIVEINFOTOMS");
	CProtocol::RegistProtoName(PTCF2M_FMWHITEROLENTF_TYPE, "PTCF2M_FMWHITEROLENTF");
	CProtocol::RegistProtoName(PTCF2M_LARGEROOMRADIONTF_TYPE, "PTCF2M_LARGEROOMRADIONTF");
	CProtocol::RegistProtoName(PTCG2M_ENTERHALLNOTICE_TYPE, "PTCG2M_ENTERHALLNOTICE");
	CProtocol::RegistProtoName(PTCR2M_CROSSGSCLOSENTF2MS_TYPE, "PTCR2M_CROSSGSCLOSENTF2MS");
	CProtocol::RegistProtoName(PTCC2M_HANDLEMICPHONE_TYPE, "PTCC2M_HANDLEMICPHONE");
	CProtocol::RegistProtoName(PTCF2M_FMLARGEROOMCLOSEPARMNTF_TYPE, "PTCF2M_FMLARGEROOMCLOSEPARMNTF");
	CProtocol::RegistProtoName(PTCG2M_SCENEEMPTYNOTIFY_TYPE, "PTCG2M_SCENEEMPTYNOTIFY");
	CProtocol::RegistProtoName(PTCG2M_REMOVEASKITEMNTF_TYPE, "PTCG2M_REMOVEASKITEMNTF");
	CProtocol::RegistProtoName(PTCG2M_ADDPARTNERLIVENESSG2M_TYPE, "PTCG2M_ADDPARTNERLIVENESSG2M");
	CProtocol::RegistProtoName(PTCC2M_POKERTOURNAMENTREQ_TYPE, "PTCC2M_POKERTOURNAMENTREQ");
	CProtocol::RegistProtoName(PTCG2M_GCFSYNG2M_TYPE, "PTCG2M_GCFSYNG2M");
	CProtocol::RegistProtoName(PTCC2M_PAYPARAMETERINFONTF_TYPE, "PTCC2M_PAYPARAMETERINFONTF");
	CProtocol::RegistProtoName(PTCG2M_PAYADDDIAMONDNTF_TYPE, "PTCG2M_PAYADDDIAMONDNTF");
	CProtocol::RegistProtoName(PTCG2M_PAYCONSUMENTF_TYPE, "PTCG2M_PAYCONSUMENTF");
	CProtocol::RegistProtoName(PTCF2M_AUDIOCHANNELTOMS_TYPE, "PTCF2M_AUDIOCHANNELTOMS");
	CProtocol::RegistProtoName(PTCG2M_LEAGUEBATTLEUPDATEDATAG2M_TYPE, "PTCG2M_LEAGUEBATTLEUPDATEDATAG2M");
	CProtocol::RegistProtoName(PTCI2M_SERVERINFONTF_TYPE, "PTCI2M_SERVERINFONTF");
	CProtocol::RegistProtoName(PTCG2M_LEAGUEELIMINATIONUPDATEG2M_TYPE, "PTCG2M_LEAGUEELIMINATIONUPDATEG2M");
	CProtocol::RegistProtoName(PTCC2M_CLOSELEAGUEELENTF_TYPE, "PTCC2M_CLOSELEAGUEELENTF");
	CProtocol::RegistProtoName(PTCF2M_FMWHITELISTRELOADNTF_TYPE, "PTCF2M_FMWHITELISTRELOADNTF");
	CProtocol::RegistProtoName(PTCG2M_SYNGUILDBOSSATTR_TYPE, "PTCG2M_SYNGUILDBOSSATTR");
	CProtocol::RegistProtoName(PTCG2M_LEVELCHANGENTFMS_TYPE, "PTCG2M_LEVELCHANGENTFMS");
	CProtocol::RegistProtoName(PTCK2M_PKMATCHINWORLDNTF_TYPE, "PTCK2M_PKMATCHINWORLDNTF");
	CProtocol::RegistProtoName(PTCK2M_SYNCCROSSLEAGUETEAM2MS_TYPE, "PTCK2M_SYNCCROSSLEAGUETEAM2MS");
	CProtocol::RegistProtoName(PTCK2M_SYNCROLESUMMARYTOMS_TYPE, "PTCK2M_SYNCROLESUMMARYTOMS");
	CProtocol::RegistProtoName(PTCK2M_LEAGUEMATCHWORLDRESULTNTF_TYPE, "PTCK2M_LEAGUEMATCHWORLDRESULTNTF");
	CProtocol::RegistProtoName(PTCK2M_HEROWORLDROOMNTF_TYPE, "PTCK2M_HEROWORLDROOMNTF");
	CProtocol::RegistProtoName(PTCK2M_SYNCSERVERTIME2MS_TYPE, "PTCK2M_SYNCSERVERTIME2MS");
	CProtocol::RegistProtoName(PTCK2M_LEAGUECROSSELINTFMS_TYPE, "PTCK2M_LEAGUECROSSELINTFMS");
	CProtocol::RegistProtoName(PTCK2M_LEAGUECROSSREWARDNTF_TYPE, "PTCK2M_LEAGUECROSSREWARDNTF");
	CProtocol::RegistProtoName(PTCG2M_OPENSYSTEMNTFMS_TYPE, "PTCG2M_OPENSYSTEMNTFMS");
	CProtocol::RegistProtoName(PTCK2M_BROADCASTWORLD2MS_TYPE, "PTCK2M_BROADCASTWORLD2MS");
	CProtocol::RegistProtoName(PTCF2M_FMMASTERINFONTF_TYPE, "PTCF2M_FMMASTERINFONTF");
	CProtocol::RegistProtoName(PTCP2M_KICKACCOUNTNTF_TYPE, "PTCP2M_KICKACCOUNTNTF");
	CProtocol::RegistProtoName(PTCI2M_BANACCOUNTLISTNTF_TYPE, "PTCI2M_BANACCOUNTLISTNTF");
	CProtocol::RegistProtoName(PTCI2M_DELBANACCOUNTNTF_TYPE, "PTCI2M_DELBANACCOUNTNTF");
	CProtocol::RegistProtoName(PTCS2M_S2MINVFIGHTMSGNTF_TYPE, "PTCS2M_S2MINVFIGHTMSGNTF");
	CProtocol::RegistProtoName(PTCK2M_INVFIGHTCROSSNTF_TYPE, "PTCK2M_INVFIGHTCROSSNTF");
	CProtocol::RegistProtoName(PTCG2M_UPDATEGLOBALVALUE_TYPE, "PTCG2M_UPDATEGLOBALVALUE");
	CProtocol::RegistProtoName(PTCC2M_PAYBUYGOODSFAILNTF_TYPE, "PTCC2M_PAYBUYGOODSFAILNTF");
	CProtocol::RegistProtoName(PTCC2M_IBGIFTTIPSREQ_TYPE, "PTCC2M_IBGIFTTIPSREQ");
	CProtocol::RegistProtoName(PTCC2M_CLOSESKYCRAFTELINTF_TYPE, "PTCC2M_CLOSESKYCRAFTELINTF");
	CProtocol::RegistProtoName(PTCK2M_CHANGESCENEWORLDREQ_TYPE, "PTCK2M_CHANGESCENEWORLDREQ");
	CProtocol::RegistProtoName(PTCG2M_REPORTCUSTOMBATTLERESULT_TYPE, "PTCG2M_REPORTCUSTOMBATTLERESULT");
	CProtocol::RegistProtoName(PTCS2M_TEAMREQUESTPLATFREINDCONFIRM_TYPE, "PTCS2M_TEAMREQUESTPLATFREINDCONFIRM");
	CProtocol::RegistProtoName(PTCK2M_KMATCHEDK2MNTF_TYPE, "PTCK2M_KMATCHEDK2MNTF");
	CProtocol::RegistProtoName(PTCG2M_REPORTBADPLAYER2MS_TYPE, "PTCG2M_REPORTBADPLAYER2MS");
	CProtocol::RegistProtoName(PTCG2M_TARJAINFONTF_TYPE, "PTCG2M_TARJAINFONTF");
	CProtocol::RegistProtoName(PTCG2M_SCENETEAMRESULTNTF_TYPE, "PTCG2M_SCENETEAMRESULTNTF");
	CProtocol::RegistProtoName(PTCG2M_TEAMCOSTLEADERRESETNTF_TYPE, "PTCG2M_TEAMCOSTLEADERRESETNTF");
	CProtocol::RegistProtoName(PTCG2M_ADDMOBABATTLERECORD_TYPE, "PTCG2M_ADDMOBABATTLERECORD");
	CProtocol::RegistProtoName(PTCG2M_CLIENTSESSIONCHANGEG2M_TYPE, "PTCG2M_CLIENTSESSIONCHANGEG2M");
	CProtocol::RegistProtoName(PTCG2M_ROLELOGINRECONNECTNTF_TYPE, "PTCG2M_ROLELOGINRECONNECTNTF");
	CProtocol::RegistProtoName(PTCK2M_BMCROSSNTFDATANTF_TYPE, "PTCK2M_BMCROSSNTFDATANTF");
	CProtocol::RegistProtoName(PTCK2M_ENTERBMFIGHTSCENENTF_TYPE, "PTCK2M_ENTERBMFIGHTSCENENTF");
	CProtocol::RegistProtoName(PTCK2M_MAYHEMAWARDNTF_TYPE, "PTCK2M_MAYHEMAWARDNTF");
	CProtocol::RegistProtoName(PTCK2M_MAYHEMBATTLEFIELDREPORT_TYPE, "PTCK2M_MAYHEMBATTLEFIELDREPORT");
	CProtocol::RegistProtoName(PTCG2M_TEAMTRANGS2MS_TYPE, "PTCG2M_TEAMTRANGS2MS");
	CProtocol::RegistProtoName(PTCH2M_TEAMTRANTS2MS_TYPE, "PTCH2M_TEAMTRANTS2MS");
	CProtocol::RegistProtoName(PTCG2M_UPDATEWEDDINGSTATEG2M_TYPE, "PTCG2M_UPDATEWEDDINGSTATEG2M");
	CProtocol::RegistProtoName(PTCG2M_PRIVATECHAT_TYPE, "PTCG2M_PRIVATECHAT");
	CProtocol::RegistProtoName(PTCG2M_INVFIGHTAGAINREQG2M_TYPE, "PTCG2M_INVFIGHTAGAINREQG2M");
	CProtocol::RegistProtoName(PTCK2M_ENTERBFFIGHTSCENENTF_TYPE, "PTCK2M_ENTERBFFIGHTSCENENTF");
	CProtocol::RegistProtoName(PTCG2M_REQDRAGONGUILDSIMPLEDATA_TYPE, "PTCG2M_REQDRAGONGUILDSIMPLEDATA");
	CProtocol::RegistProtoName(PTCG2M_COSTDRAGONGUILDRECEIVECOUNT_TYPE, "PTCG2M_COSTDRAGONGUILDRECEIVECOUNT");
	CProtocol::RegistProtoName(PTCG2M_CHECKDRAGONGUILDTASK_TYPE, "PTCG2M_CHECKDRAGONGUILDTASK");
	CProtocol::RegistProtoName(PTCC2M_GOBACKREADYSCENENTF_TYPE, "PTCC2M_GOBACKREADYSCENENTF");
	CProtocol::RegistProtoName(PTCG2M_GOALAWARDSSERVEREVENT_TYPE, "PTCG2M_GOALAWARDSSERVEREVENT");
	CProtocol::RegistProtoName(PTCK2M_BATTLEFIELDREADYSCENENTF_TYPE, "PTCK2M_BATTLEFIELDREADYSCENENTF");
	CProtocol::RegistProtoName(PTCK2M_BATTLEFAILINFOMSNTF_TYPE, "PTCK2M_BATTLEFAILINFOMSNTF");
	CProtocol::RegistProtoName(PTCG2M_GOALAWARDSSERVERTEAM_TYPE, "PTCG2M_GOALAWARDSSERVERTEAM");
	CProtocol::RegistProtoName(PTCC2M_MULACTIVITYICONSYSREQ_TYPE, "PTCC2M_MULACTIVITYICONSYSREQ");
	CProtocol::RegistProtoName(PTCG2M_KICKACCOUNTJKYDG2M_TYPE, "PTCG2M_KICKACCOUNTJKYDG2M");
	CProtocol::RegistProtoName(PTCM2C_ANSWERACKNTF_TYPE, "PTCM2C_ANSWERACKNTF");
	CProtocol::RegistProtoName(PTCM2C_PUSHQUESTIONNTF_TYPE, "PTCM2C_PUSHQUESTIONNTF");
	CProtocol::RegistProtoName(PTCM2C_QAOVERNTF_TYPE, "PTCM2C_QAOVERNTF");
	CProtocol::RegistProtoName(PTCM2C_QAROOMRANKNTF_TYPE, "PTCM2C_QAROOMRANKNTF");
	CProtocol::RegistProtoName(PTCM2G_UPDATEGUILDQAROOM_TYPE, "PTCM2G_UPDATEGUILDQAROOM");
	CProtocol::RegistProtoName(PTCM2C_QAENTERROOMNTF_TYPE, "PTCM2C_QAENTERROOMNTF");
	CProtocol::RegistProtoName(PTCM2P_REGISTMSSERVER_TYPE, "PTCM2P_REGISTMSSERVER");
	CProtocol::RegistProtoName(PTCM2D_ADDTEMPLATEMAIL_TYPE, "PTCM2D_ADDTEMPLATEMAIL");
	CProtocol::RegistProtoName(PTCM2D_MAILOPERATION_TYPE, "PTCM2D_MAILOPERATION");
	CProtocol::RegistProtoName(PTCM2T_CHATFORWARD_TYPE, "PTCM2T_CHATFORWARD");
	CProtocol::RegistProtoName(PTCM2D_MDUPLOADCHATNTF_TYPE, "PTCM2D_MDUPLOADCHATNTF");
	CProtocol::RegistProtoName(PTCM2T_VERSIONNOTIFY_TYPE, "PTCM2T_VERSIONNOTIFY");
	CProtocol::RegistProtoName(PTCM2C_MCCHATOFFLINENOTIFY_TYPE, "PTCM2C_MCCHATOFFLINENOTIFY");
	CProtocol::RegistProtoName(PTCM2D_MSUPDATEGLOBALCONFIG_TYPE, "PTCM2D_MSUPDATEGLOBALCONFIG");
	CProtocol::RegistProtoName(PTCM2G_MSLEVELSEALNTF_TYPE, "PTCM2G_MSLEVELSEALNTF");
	CProtocol::RegistProtoName(PTCM2C_FRIENDOPNTFNEW_TYPE, "PTCM2C_FRIENDOPNTFNEW");
	CProtocol::RegistProtoName(PTCM2C_FRIENDDEGREEUPNTFNEW_TYPE, "PTCM2C_FRIENDDEGREEUPNTFNEW");
	CProtocol::RegistProtoName(PTCM2C_BLACKLISTNTFNEW_TYPE, "PTCM2C_BLACKLISTNTFNEW");
	CProtocol::RegistProtoName(PTCM2G_NOTICEGUILDBOSSENDTOGSN_TYPE, "PTCM2G_NOTICEGUILDBOSSENDTOGSN");
	CProtocol::RegistProtoName(PTCM2D_UPDATEFRIENDONDB_TYPE, "PTCM2D_UPDATEFRIENDONDB");
	CProtocol::RegistProtoName(PTCM2G_UPDATEFRIENDONGS_TYPE, "PTCM2G_UPDATEFRIENDONGS");
	CProtocol::RegistProtoName(PTCM2G_NOTIFYWATCHICONNUM_TYPE, "PTCM2G_NOTIFYWATCHICONNUM");
	CProtocol::RegistProtoName(PTCM2C_ROLESTATENTFNEW_TYPE, "PTCM2C_ROLESTATENTFNEW");
	CProtocol::RegistProtoName(PTCM2C_HINTNOTIFYMS_TYPE, "PTCM2C_HINTNOTIFYMS");
	CProtocol::RegistProtoName(PTCM2G_SYNSCENEREWARDM2G_TYPE, "PTCM2G_SYNSCENEREWARDM2G");
	CProtocol::RegistProtoName(PTCM2G_CHANGEGUILDBOSSTIME_TYPE, "PTCM2G_CHANGEGUILDBOSSTIME");
	CProtocol::RegistProtoName(PTCM2G_WORLDBOSSSYNCGS_TYPE, "PTCM2G_WORLDBOSSSYNCGS");
	CProtocol::RegistProtoName(PTCM2L_MSLOGNOTIFY_TYPE, "PTCM2L_MSLOGNOTIFY");
	CProtocol::RegistProtoName(PTCM2D_MSSAVESOCIALINFONTF_TYPE, "PTCM2D_MSSAVESOCIALINFONTF");
	CProtocol::RegistProtoName(PTCM2C_MSRECEIVEFLOWERNTF_TYPE, "PTCM2C_MSRECEIVEFLOWERNTF");
	CProtocol::RegistProtoName(PTCM2C_MSFLOWERRAINNTF_TYPE, "PTCM2C_MSFLOWERRAINNTF");
	CProtocol::RegistProtoName(PTCM2C_TEAMCHANGEM2CNTF_TYPE, "PTCM2C_TEAMCHANGEM2CNTF");
	CProtocol::RegistProtoName(PTCM2C_LEAVETEAMM2CNTF_TYPE, "PTCM2C_LEAVETEAMM2CNTF");
	CProtocol::RegistProtoName(PTCM2C_TEAMFULLDATAM2CNTF_TYPE, "PTCM2C_TEAMFULLDATAM2CNTF");
	CProtocol::RegistProtoName(PTCM2C_TEAMINVITEM2CNOTIFY_TYPE, "PTCM2C_TEAMINVITEM2CNOTIFY");
	CProtocol::RegistProtoName(PTCM2C_MSERRORNOTIFY_TYPE, "PTCM2C_MSERRORNOTIFY");
	CProtocol::RegistProtoName(PTCM2C_INVITEREFUSEM2CNTF_TYPE, "PTCM2C_INVITEREFUSEM2CNTF");
	CProtocol::RegistProtoName(PTCM2C_ROLEMATCHSTATEM2CNOTIFY_TYPE, "PTCM2C_ROLEMATCHSTATEM2CNOTIFY");
	CProtocol::RegistProtoName(PTCM2C_FASTMBCONFIRMM2CNTF_TYPE, "PTCM2C_FASTMBCONFIRMM2CNTF");
	CProtocol::RegistProtoName(PTCM2C_FASTMBDISMISSM2CNTF_TYPE, "PTCM2C_FASTMBDISMISSM2CNTF");
	CProtocol::RegistProtoName(PTCM2I_UPDATESELFSERVER_TYPE, "PTCM2I_UPDATESELFSERVER");
	CProtocol::RegistProtoName(PTCM2D_SAVERANKLISTCHANGETODB_TYPE, "PTCM2D_SAVERANKLISTCHANGETODB");
	CProtocol::RegistProtoName(PTCM2D_SAVERANKLISTTODB_TYPE, "PTCM2D_SAVERANKLISTTODB");
	CProtocol::RegistProtoName(PTCM2D_SAVEROLESUMMARYDATA_TYPE, "PTCM2D_SAVEROLESUMMARYDATA");
	CProtocol::RegistProtoName(PTCM2C_QAIDNAMENTF_TYPE, "PTCM2C_QAIDNAMENTF");
	CProtocol::RegistProtoName(PTCM2G_FIRSTPASSNEWCOMMENDREWARDNTF_TYPE, "PTCM2G_FIRSTPASSNEWCOMMENDREWARDNTF");
	CProtocol::RegistProtoName(PTCM2C_PKTIMEOUTM2CNTF_TYPE, "PTCM2C_PKTIMEOUTM2CNTF");
	CProtocol::RegistProtoName(PTCM2D_SAVEGUILDDATA_TYPE, "PTCM2D_SAVEGUILDDATA");
	CProtocol::RegistProtoName(PTCM2C_LOGINGUILDINFO_TYPE, "PTCM2C_LOGINGUILDINFO");
	CProtocol::RegistProtoName(PTCM2C_SENDGUILDSKILLINFO_TYPE, "PTCM2C_SENDGUILDSKILLINFO");
	CProtocol::RegistProtoName(PTCM2D_UPDATEPRIVATELISTNTF_TYPE, "PTCM2D_UPDATEPRIVATELISTNTF");
	CProtocol::RegistProtoName(PTCM2G_SYNROLEINGUILD_TYPE, "PTCM2G_SYNROLEINGUILD");
	CProtocol::RegistProtoName(PTCM2D_SAVEPUNISHDATA_TYPE, "PTCM2D_SAVEPUNISHDATA");
	CProtocol::RegistProtoName(PTCM2C_MSEVENTNOTIFY_TYPE, "PTCM2C_MSEVENTNOTIFY");
	CProtocol::RegistProtoName(PTCM2G_SYNGUILDINFO_TYPE, "PTCM2G_SYNGUILDINFO");
	CProtocol::RegistProtoName(PTCM2C_SYNGUILDARENAFIGHTUNITNEW_TYPE, "PTCM2C_SYNGUILDARENAFIGHTUNITNEW");
	CProtocol::RegistProtoName(PTCM2C_SYNGUILDARENABATTLEINFONEW_TYPE, "PTCM2C_SYNGUILDARENABATTLEINFONEW");
	CProtocol::RegistProtoName(PTCM2C_NOTICEGUILDARENABEGINNEW_TYPE, "PTCM2C_NOTICEGUILDARENABEGINNEW");
	CProtocol::RegistProtoName(PTCM2C_SYNGUILDARENAROLEONLINENEW_TYPE, "PTCM2C_SYNGUILDARENAROLEONLINENEW");
	CProtocol::RegistProtoName(PTCM2C_SYNGUILDARENADISPLACENEW_TYPE, "PTCM2C_SYNGUILDARENADISPLACENEW");
	CProtocol::RegistProtoName(PTCM2C_NOTIFYGUILDSKILLDATA_TYPE, "PTCM2C_NOTIFYGUILDSKILLDATA");
	CProtocol::RegistProtoName(PTCM2G_SYNCARENAGS_TYPE, "PTCM2G_SYNCARENAGS");
	CProtocol::RegistProtoName(PTCM2G_SYNGUILDSIMPLEDATA_TYPE, "PTCM2G_SYNGUILDSIMPLEDATA");
	CProtocol::RegistProtoName(PTCM2G_MNEWGUILDBONUSNTF_TYPE, "PTCM2G_MNEWGUILDBONUSNTF");
	CProtocol::RegistProtoName(PTCM2C_GUILDSETINGNOTIFY_TYPE, "PTCM2C_GUILDSETINGNOTIFY");
	CProtocol::RegistProtoName(PTCM2D_AUCTSALEITEMDBNTF_TYPE, "PTCM2D_AUCTSALEITEMDBNTF");
	CProtocol::RegistProtoName(PTCM2D_AUCTTRADERECORDDBNTF_TYPE, "PTCM2D_AUCTTRADERECORDDBNTF");
	CProtocol::RegistProtoName(PTCM2G_NOTIFYGUILDCHECKINPROCESS_TYPE, "PTCM2G_NOTIFYGUILDCHECKINPROCESS");
	CProtocol::RegistProtoName(PTCM2C_GUILDBESTCARDSNTFMS_TYPE, "PTCM2C_GUILDBESTCARDSNTFMS");
	CProtocol::RegistProtoName(PTCM2C_DERANKCHANGNTF_TYPE, "PTCM2C_DERANKCHANGNTF");
	CProtocol::RegistProtoName(PTCM2G_SYNFATIGUERESULT_TYPE, "PTCM2G_SYNFATIGUERESULT");
	CProtocol::RegistProtoName(PTCM2C_GMFJOINBATTLEM2CREQ_TYPE, "PTCM2C_GMFJOINBATTLEM2CREQ");
	CProtocol::RegistProtoName(PTCM2G_SYNGUILDCHECKINHINT_TYPE, "PTCM2G_SYNGUILDCHECKINHINT");
	CProtocol::RegistProtoName(PTCM2D_UPDATEDESIGNATION_TYPE, "PTCM2D_UPDATEDESIGNATION");
	CProtocol::RegistProtoName(PTCM2C_SKYCITYFINALRES_TYPE, "PTCM2C_SKYCITYFINALRES");
	CProtocol::RegistProtoName(PTCM2C_NOTIFYIDIPMESSAGEMS_TYPE, "PTCM2C_NOTIFYIDIPMESSAGEMS");
	CProtocol::RegistProtoName(PTCM2C_IDIPPUNISHINFOMSNTF_TYPE, "PTCM2C_IDIPPUNISHINFOMSNTF");
	CProtocol::RegistProtoName(PTCM2C_IDIPCLEARCHATNTF_TYPE, "PTCM2C_IDIPCLEARCHATNTF");
	CProtocol::RegistProtoName(PTCM2D_SAVEGARDENINFO_TYPE, "PTCM2D_SAVEGARDENINFO");
	CProtocol::RegistProtoName(PTCM2C_GUILDCARDRANKNTF_TYPE, "PTCM2C_GUILDCARDRANKNTF");
	CProtocol::RegistProtoName(PTCM2C_NOTICEGUILDLADDERSTART_TYPE, "PTCM2C_NOTICEGUILDLADDERSTART");
	CProtocol::RegistProtoName(PTCM2C_GUILDCARDMATCHNTF_TYPE, "PTCM2C_GUILDCARDMATCHNTF");
	CProtocol::RegistProtoName(PTCM2G_GIVEANSERQAREWARDNTF_TYPE, "PTCM2G_GIVEANSERQAREWARDNTF");
	CProtocol::RegistProtoName(PTCM2G_QAOVERTOGS_TYPE, "PTCM2G_QAOVERTOGS");
	CProtocol::RegistProtoName(PTCM2T_BROADCASTM2T_TYPE, "PTCM2T_BROADCASTM2T");
	CProtocol::RegistProtoName(PTCM2C_SPRITESTATECHANGENTF_TYPE, "PTCM2C_SPRITESTATECHANGENTF");
	CProtocol::RegistProtoName(PTCM2G_GARDENEVENTNTF_TYPE, "PTCM2G_GARDENEVENTNTF");
	CProtocol::RegistProtoName(PTCM2C_INVUNFSTATEM2CNTF_TYPE, "PTCM2C_INVUNFSTATEM2CNTF");
	CProtocol::RegistProtoName(PTCM2G_ROLEEVENTSYNCGS_TYPE, "PTCM2G_ROLEEVENTSYNCGS");
	CProtocol::RegistProtoName(PTCM2D_DELROLENTF_TYPE, "PTCM2D_DELROLENTF");
	CProtocol::RegistProtoName(PTCM2C_NOTICEGUILDARENANEXTTIME_TYPE, "PTCM2C_NOTICEGUILDARENANEXTTIME");
	CProtocol::RegistProtoName(PTCM2C_GARDENBANQUETNTF_TYPE, "PTCM2C_GARDENBANQUETNTF");
	CProtocol::RegistProtoName(PTCM2D_SAVEPAYLIST_TYPE, "PTCM2D_SAVEPAYLIST");
	CProtocol::RegistProtoName(PTCM2C_NOTICEGUILDWAGEREWARD_TYPE, "PTCM2C_NOTICEGUILDWAGEREWARD");
	CProtocol::RegistProtoName(PTCM2D_SAVEPLATFRIENDINFO_TYPE, "PTCM2D_SAVEPLATFRIENDINFO");
	CProtocol::RegistProtoName(PTCM2C_SENDGUILDBONUSNTF_TYPE, "PTCM2C_SENDGUILDBONUSNTF");
	CProtocol::RegistProtoName(PTCM2D_GUILDAUCTUPDATENTF_TYPE, "PTCM2D_GUILDAUCTUPDATENTF");
	CProtocol::RegistProtoName(PTCM2C_RESWARGUILDBRIEFNTF_TYPE, "PTCM2C_RESWARGUILDBRIEFNTF");
	CProtocol::RegistProtoName(PTCM2C_SYNGUILINTEGRALSTATE_TYPE, "PTCM2C_SYNGUILINTEGRALSTATE");
	CProtocol::RegistProtoName(PTCM2C_RESWARSTATENTF_TYPE, "PTCM2C_RESWARSTATENTF");
	CProtocol::RegistProtoName(PTCM2C_SYNGUILDINTEGRALSTATE_TYPE, "PTCM2C_SYNGUILDINTEGRALSTATE");
	CProtocol::RegistProtoName(PTCM2G_SYNCSERVERTIME_TYPE, "PTCM2G_SYNCSERVERTIME");
	CProtocol::RegistProtoName(PTCM2C_RESWARRANKSIMPLEINFONTF_TYPE, "PTCM2C_RESWARRANKSIMPLEINFONTF");
	CProtocol::RegistProtoName(PTCM2C_GUILDBUFFSIMPLEINFONTF_TYPE, "PTCM2C_GUILDBUFFSIMPLEINFONTF");
	CProtocol::RegistProtoName(PTCM2C_GUILDBUFFSIMPLEITEMNTF_TYPE, "PTCM2C_GUILDBUFFSIMPLEITEMNTF");
	CProtocol::RegistProtoName(PTCM2G_WORLDLEVELNTF_TYPE, "PTCM2G_WORLDLEVELNTF");
	CProtocol::RegistProtoName(PTCM2C_HALLICONMNTF_TYPE, "PTCM2C_HALLICONMNTF");
	CProtocol::RegistProtoName(PTCM2D_SAVEPLATNOTICEPTF_TYPE, "PTCM2D_SAVEPLATNOTICEPTF");
	CProtocol::RegistProtoName(PTCM2C_RESWARMINEDATANTF_TYPE, "PTCM2C_RESWARMINEDATANTF");
	CProtocol::RegistProtoName(PTCM2G_NOTICEGUILDINHERITNUMINFO_TYPE, "PTCM2G_NOTICEGUILDINHERITNUMINFO");
	CProtocol::RegistProtoName(PTCM2C_LARGEROOMROLENTF_TYPE, "PTCM2C_LARGEROOMROLENTF");
	CProtocol::RegistProtoName(PTCM2C_LARGEROOMLOGINPARAMNTF_TYPE, "PTCM2C_LARGEROOMLOGINPARAMNTF");
	CProtocol::RegistProtoName(PTCM2F_LEAVEFMLARGEROOM_TYPE, "PTCM2F_LEAVEFMLARGEROOM");
	CProtocol::RegistProtoName(PTCM2C_RESWARTIMENTF_TYPE, "PTCM2C_RESWARTIMENTF");
	CProtocol::RegistProtoName(PTCM2F_FMSHOWNTF_TYPE, "PTCM2F_FMSHOWNTF");
	CProtocol::RegistProtoName(PTCM2C_GUILDBUFFCDPARAMNTF_TYPE, "PTCM2C_GUILDBUFFCDPARAMNTF");
	CProtocol::RegistProtoName(PTCM2G_MSDESTORYSCENE_TYPE, "PTCM2G_MSDESTORYSCENE");
	CProtocol::RegistProtoName(PTCM2C_NOTIFYMENTORAPPLY_TYPE, "PTCM2C_NOTIFYMENTORAPPLY");
	CProtocol::RegistProtoName(PTCM2C_UPDATEPARTNERTOCLIENT_TYPE, "PTCM2C_UPDATEPARTNERTOCLIENT");
	CProtocol::RegistProtoName(PTCM2G_UPDATEPARTNERTOGS_TYPE, "PTCM2G_UPDATEPARTNERTOGS");
	CProtocol::RegistProtoName(PTCM2C_MAKEPARTNERRESULTNTF_TYPE, "PTCM2C_MAKEPARTNERRESULTNTF");
	CProtocol::RegistProtoName(PTCM2C_INVFIGHTNOTIFY_TYPE, "PTCM2C_INVFIGHTNOTIFY");
	CProtocol::RegistProtoName(PTCM2D_MDDELCHATNTF_TYPE, "PTCM2D_MDDELCHATNTF");
	CProtocol::RegistProtoName(PTCM2D_SAVEMENTORINFONTF_TYPE, "PTCM2D_SAVEMENTORINFONTF");
	CProtocol::RegistProtoName(PTCM2G_GCFSYNM2G_TYPE, "PTCM2G_GCFSYNM2G");
	CProtocol::RegistProtoName(PTCM2C_PAYPARAMETERINFOINVALIDNTF_TYPE, "PTCM2C_PAYPARAMETERINFOINVALIDNTF");
	CProtocol::RegistProtoName(PTCM2C_NOTICEGUILDTERRWAR_TYPE, "PTCM2C_NOTICEGUILDTERRWAR");
	CProtocol::RegistProtoName(PTCM2C_NOTICEGUILDTERRALL_TYPE, "PTCM2C_NOTICEGUILDTERRALL");
	CProtocol::RegistProtoName(PTCM2G_TRYTEACHERINHERIT_TYPE, "PTCM2G_TRYTEACHERINHERIT");
	CProtocol::RegistProtoName(PTCM2G_BALANCEINFONTF_TYPE, "PTCM2G_BALANCEINFONTF");
	CProtocol::RegistProtoName(PTCM2C_GUILDBONUSGETALL_TYPE, "PTCM2C_GUILDBONUSGETALL");
	CProtocol::RegistProtoName(PTCM2N_UPDATEREGISTERCOUNT2NS_TYPE, "PTCM2N_UPDATEREGISTERCOUNT2NS");
	CProtocol::RegistProtoName(PTCM2C_RESWARENEMYTIMENTF_TYPE, "PTCM2C_RESWARENEMYTIMENTF");
	CProtocol::RegistProtoName(PTCM2F_AUDIOCHANNELTOFM_TYPE, "PTCM2F_AUDIOCHANNELTOFM");
	CProtocol::RegistProtoName(PTCM2C_UPDATELEAGUEBATTLESEASONINFO_TYPE, "PTCM2C_UPDATELEAGUEBATTLESEASONINFO");
	CProtocol::RegistProtoName(PTCM2C_NOTIFYLEAGUETEAMCREATE_TYPE, "PTCM2C_NOTIFYLEAGUETEAMCREATE");
	CProtocol::RegistProtoName(PTCM2C_NOTIFYLEAGUETEAMDISSOLVE_TYPE, "PTCM2C_NOTIFYLEAGUETEAMDISSOLVE");
	CProtocol::RegistProtoName(PTCM2C_UPDATELEAGUETEAMSTATE_TYPE, "PTCM2C_UPDATELEAGUETEAMSTATE");
	CProtocol::RegistProtoName(PTCM2C_LEAGUEBATTLESTARTMATCHNTF_TYPE, "PTCM2C_LEAGUEBATTLESTARTMATCHNTF");
	CProtocol::RegistProtoName(PTCM2C_LEAGUEBATTLESTOPMATCHNTF_TYPE, "PTCM2C_LEAGUEBATTLESTOPMATCHNTF");
	CProtocol::RegistProtoName(PTCM2C_LEAGUEBATTLEMATCHTIMEOUTNTF_TYPE, "PTCM2C_LEAGUEBATTLEMATCHTIMEOUTNTF");
	CProtocol::RegistProtoName(PTCM2C_MIDASEXCEPTIONNTF_TYPE, "PTCM2C_MIDASEXCEPTIONNTF");
	CProtocol::RegistProtoName(PTCM2C_UPDATELEAGUEELEROOMSTATENTF_TYPE, "PTCM2C_UPDATELEAGUEELEROOMSTATENTF");
	CProtocol::RegistProtoName(PTCM2G_SYNGUILDBOSSHP_TYPE, "PTCM2G_SYNGUILDBOSSHP");
	CProtocol::RegistProtoName(PTCM2G_SYNGUILDBOSSDIE_TYPE, "PTCM2G_SYNGUILDBOSSDIE");
	CProtocol::RegistProtoName(PTCM2G_SYNGUILDBOSSDOEND_TYPE, "PTCM2G_SYNGUILDBOSSDOEND");
	CProtocol::RegistProtoName(PTCM2C_KICKFMAUCHOR_TYPE, "PTCM2C_KICKFMAUCHOR");
	CProtocol::RegistProtoName(PTCM2G_NOTICEGUILDBOSSATTR_TYPE, "PTCM2G_NOTICEGUILDBOSSATTR");
	CProtocol::RegistProtoName(PTCM2G_CONTROLFUNCTIONNTF_TYPE, "PTCM2G_CONTROLFUNCTIONNTF");
	CProtocol::RegistProtoName(PTCM2C_GUILDAUCTITEMTIMEFRESH_TYPE, "PTCM2C_GUILDAUCTITEMTIMEFRESH");
	CProtocol::RegistProtoName(PTCM2K_LEAGUEMATCHWORLDNTF_TYPE, "PTCM2K_LEAGUEMATCHWORLDNTF");
	CProtocol::RegistProtoName(PTCM2C_POKERTOURNAMENTENDREFUND_TYPE, "PTCM2C_POKERTOURNAMENTENDREFUND");
	CProtocol::RegistProtoName(PTCM2G_CLICKNOTICENTF_TYPE, "PTCM2G_CLICKNOTICENTF");
	CProtocol::RegistProtoName(PTCM2K_HEROBATTLEWORLDMATCHNTF_TYPE, "PTCM2K_HEROBATTLEWORLDMATCHNTF");
	CProtocol::RegistProtoName(PTCM2D_DELETECROSSROLESUMMARY_TYPE, "PTCM2D_DELETECROSSROLESUMMARY");
	CProtocol::RegistProtoName(PTCM2C_NOTICEGUILDTERREND_TYPE, "PTCM2C_NOTICEGUILDTERREND");
	CProtocol::RegistProtoName(PTCM2C_NOTICEGUILDTERRBATTLEWIN_TYPE, "PTCM2C_NOTICEGUILDTERRBATTLEWIN");
	CProtocol::RegistProtoName(PTCM2C_NOTICEGUILDTERRBIGICON_TYPE, "PTCM2C_NOTICEGUILDTERRBIGICON");
	CProtocol::RegistProtoName(PTCM2K_REPORTROLESUMMARY2WORLD_TYPE, "PTCM2K_REPORTROLESUMMARY2WORLD");
	CProtocol::RegistProtoName(PTCM2K_GETROLESUMMARYFROMWORLD_TYPE, "PTCM2K_GETROLESUMMARYFROMWORLD");
	CProtocol::RegistProtoName(PTCM2G_MILITARYRANKCHANGE_TYPE, "PTCM2G_MILITARYRANKCHANGE");
	CProtocol::RegistProtoName(PTCM2T_NOTIFYROUTERINFO2GATE_TYPE, "PTCM2T_NOTIFYROUTERINFO2GATE");
	CProtocol::RegistProtoName(PTCM2D_NOTIFYROUTERINFO2DB_TYPE, "PTCM2D_NOTIFYROUTERINFO2DB");
	CProtocol::RegistProtoName(PTCM2N_NOTIFYROUTERINFO2CTRL_TYPE, "PTCM2N_NOTIFYROUTERINFO2CTRL");
	CProtocol::RegistProtoName(PTCM2G_BUYPAYAILEENNTF_TYPE, "PTCM2G_BUYPAYAILEENNTF");
	CProtocol::RegistProtoName(PTCM2C_MODIFYGUILDNAMENTF_TYPE, "PTCM2C_MODIFYGUILDNAMENTF");
	CProtocol::RegistProtoName(PTCM2G_WORLDBOSSGUILDADDATTRSYNCGSNTF_TYPE, "PTCM2G_WORLDBOSSGUILDADDATTRSYNCGSNTF");
	CProtocol::RegistProtoName(PTCM2S_M2SUSERSTATUSSYNC_TYPE, "PTCM2S_M2SUSERSTATUSSYNC");
	CProtocol::RegistProtoName(PTCM2G_ENTERSCENEFROMMS_TYPE, "PTCM2G_ENTERSCENEFROMMS");
	CProtocol::RegistProtoName(PTCM2G_BROADCASTGLOBALVALUE_TYPE, "PTCM2G_BROADCASTGLOBALVALUE");
	CProtocol::RegistProtoName(PTCM2D_SAVEPAYIBITEM_TYPE, "PTCM2D_SAVEPAYIBITEM");
	CProtocol::RegistProtoName(PTCM2C_ARENASTARDATANTF_TYPE, "PTCM2C_ARENASTARDATANTF");
	CProtocol::RegistProtoName(PTCM2C_NOTIFYSKYTEAMCREATE_TYPE, "PTCM2C_NOTIFYSKYTEAMCREATE");
	CProtocol::RegistProtoName(PTCM2C_SKYCRAFTMATCHNTF_TYPE, "PTCM2C_SKYCRAFTMATCHNTF");
	CProtocol::RegistProtoName(PTCM2C_SKYCRAFTELIROOMNTF_TYPE, "PTCM2C_SKYCRAFTELIROOMNTF");
	CProtocol::RegistProtoName(PTCM2C_IBGIFTICONNTF_TYPE, "PTCM2C_IBGIFTICONNTF");
	CProtocol::RegistProtoName(PTCM2C_GIFTIBSUCESSNTF_TYPE, "PTCM2C_GIFTIBSUCESSNTF");
	CProtocol::RegistProtoName(PTCM2D_UPDATECUSTOMBATTLEONDB_TYPE, "PTCM2D_UPDATECUSTOMBATTLEONDB");
	CProtocol::RegistProtoName(PTCM2D_UPDATECUSTOMBATTLEROLEONDB_TYPE, "PTCM2D_UPDATECUSTOMBATTLEROLEONDB");
	CProtocol::RegistProtoName(PTCM2K_REPORTCUSTOMBATTLERESULTWORLD_TYPE, "PTCM2K_REPORTCUSTOMBATTLERESULTWORLD");
	CProtocol::RegistProtoName(PTCM2K_CUSTOMBATTLEDELMATCH_TYPE, "PTCM2K_CUSTOMBATTLEDELMATCH");
	CProtocol::RegistProtoName(PTCM2K_TRANSCUSTOMBATTLEMATCH_TYPE, "PTCM2K_TRANSCUSTOMBATTLEMATCH");
	CProtocol::RegistProtoName(PTCM2C_TEAMREQUESTPLATFREIND2CLIENT_TYPE, "PTCM2C_TEAMREQUESTPLATFREIND2CLIENT");
	CProtocol::RegistProtoName(PTCM2C_CUSTOMBATTLEGMNOTIFY_TYPE, "PTCM2C_CUSTOMBATTLEGMNOTIFY");
	CProtocol::RegistProtoName(PTCM2C_TARJABRIEFNTF_TYPE, "PTCM2C_TARJABRIEFNTF");
	CProtocol::RegistProtoName(PTCM2G_PKUPDATESYNNTF_TYPE, "PTCM2G_PKUPDATESYNNTF");
	CProtocol::RegistProtoName(PTCM2D_SAVEMOBAROLERECORD_TYPE, "PTCM2D_SAVEMOBAROLERECORD");
	CProtocol::RegistProtoName(PTCM2K_BMENTERREADYSCENENTF_TYPE, "PTCM2K_BMENTERREADYSCENENTF");
	CProtocol::RegistProtoName(PTCM2G_ADDMILITARYRANK_TYPE, "PTCM2G_ADDMILITARYRANK");
	CProtocol::RegistProtoName(PTCM2K_SERVERCROSSDATAM2W_TYPE, "PTCM2K_SERVERCROSSDATAM2W");
	CProtocol::RegistProtoName(PTCM2H_TEAMTRANMS2TS_TYPE, "PTCM2H_TEAMTRANMS2TS");
	CProtocol::RegistProtoName(PTCM2G_TEAMTRANMS2GS_TYPE, "PTCM2G_TEAMTRANMS2GS");
	CProtocol::RegistProtoName(PTCM2N_M2NVERSOINNOTIFY_TYPE, "PTCM2N_M2NVERSOINNOTIFY");
	CProtocol::RegistProtoName(PTCM2C_GROUPCHATMANAGER_TYPE, "PTCM2C_GROUPCHATMANAGER");
	CProtocol::RegistProtoName(PTCM2C_GROUPCHATQUIT_TYPE, "PTCM2C_GROUPCHATQUIT");
	CProtocol::RegistProtoName(PTCM2C_GROUPCHATDISMISS_TYPE, "PTCM2C_GROUPCHATDISMISS");
	CProtocol::RegistProtoName(PTCM2C_WEDDINGINVITENTF_TYPE, "PTCM2C_WEDDINGINVITENTF");
	CProtocol::RegistProtoName(PTCM2D_SAVEMARRIAGEINFONTF_TYPE, "PTCM2D_SAVEMARRIAGEINFONTF");
	CProtocol::RegistProtoName(PTCM2C_NOTIFYMARRIAGEAPPLY_TYPE, "PTCM2C_NOTIFYMARRIAGEAPPLY");
	CProtocol::RegistProtoName(PTCM2C_NOTIFYMARRIAGEDIVORCEAPPLY_TYPE, "PTCM2C_NOTIFYMARRIAGEDIVORCEAPPLY");
	CProtocol::RegistProtoName(PTCM2C_GROUPCHATISSUECOUNT_TYPE, "PTCM2C_GROUPCHATISSUECOUNT");
	CProtocol::RegistProtoName(PTCM2C_GROUPCHATAPPLY_TYPE, "PTCM2C_GROUPCHATAPPLY");
	CProtocol::RegistProtoName(PTCM2D_DBGROUPCHATSAVEGROUPISSUENTF_TYPE, "PTCM2D_DBGROUPCHATSAVEGROUPISSUENTF");
	CProtocol::RegistProtoName(PTCM2D_DBGROUPCHATDELGROUPISSUENTF_TYPE, "PTCM2D_DBGROUPCHATDELGROUPISSUENTF");
	CProtocol::RegistProtoName(PTCM2D_DBGROUPCHATSAVEROLEISSUENTF_TYPE, "PTCM2D_DBGROUPCHATSAVEROLEISSUENTF");
	CProtocol::RegistProtoName(PTCM2D_DBGROUPCHATDELROLEISSUENTF_TYPE, "PTCM2D_DBGROUPCHATDELROLEISSUENTF");
	CProtocol::RegistProtoName(PTCM2D_DBGROUPCHATSAVEGROUPNTF_TYPE, "PTCM2D_DBGROUPCHATSAVEGROUPNTF");
	CProtocol::RegistProtoName(PTCM2D_DBGROUPCHATDELGROUPNTF_TYPE, "PTCM2D_DBGROUPCHATDELGROUPNTF");
	CProtocol::RegistProtoName(PTCM2D_DBGROUPCHATDELROLENTF_TYPE, "PTCM2D_DBGROUPCHATDELROLENTF");
	CProtocol::RegistProtoName(PTCM2D_DBGROUPCHATSAVEROLENTF_TYPE, "PTCM2D_DBGROUPCHATSAVEROLENTF");
	CProtocol::RegistProtoName(PTCM2G_WEDDINGCARENDM2G_TYPE, "PTCM2G_WEDDINGCARENDM2G");
	CProtocol::RegistProtoName(PTCM2C_STARTBATTLEFAILEDM2CNTF_TYPE, "PTCM2C_STARTBATTLEFAILEDM2CNTF");
	CProtocol::RegistProtoName(PTCM2K_BATTLEFIELDEVENTREQ_TYPE, "PTCM2K_BATTLEFIELDEVENTREQ");
	CProtocol::RegistProtoName(PTCM2G_SYNDRAGONGUILDSIMPLEDATA_TYPE, "PTCM2G_SYNDRAGONGUILDSIMPLEDATA");
	CProtocol::RegistProtoName(PTCM2C_LOGINDRAGONGUILDINFO_TYPE, "PTCM2C_LOGINDRAGONGUILDINFO");
	CProtocol::RegistProtoName(PTCM2C_DRAGONGUILDSETTINGCHANGED_TYPE, "PTCM2C_DRAGONGUILDSETTINGCHANGED");
	CProtocol::RegistProtoName(PTCM2D_UPDATEDRAGONGUILDDATA_TYPE, "PTCM2D_UPDATEDRAGONGUILDDATA");
	CProtocol::RegistProtoName(PTCM2C_DRAGONGUILDNAMENTF_TYPE, "PTCM2C_DRAGONGUILDNAMENTF");
	CProtocol::RegistProtoName(PTCM2G_NOTIFYDRAGONGUILDREDPOINT_TYPE, "PTCM2G_NOTIFYDRAGONGUILDREDPOINT");
	CProtocol::RegistProtoName(PTCM2C_GOALAWARDSREDPOINT_TYPE, "PTCM2C_GOALAWARDSREDPOINT");
	CProtocol::RegistProtoName(PTCM2G_GOALAWARDSSERVERAWARDS_TYPE, "PTCM2G_GOALAWARDSSERVERAWARDS");
	CProtocol::RegistProtoName(PTCM2G_BATTLEFIEDLREADYSCENEGSNTF_TYPE, "PTCM2G_BATTLEFIEDLREADYSCENEGSNTF");
	CProtocol::RegistProtoName(PTCM2D_DBGOALAWARDSSAVENTF_TYPE, "PTCM2D_DBGOALAWARDSSAVENTF");
	CProtocol::RegistProtoName(PTCM2G_NPCFLNOTIFYM2G_TYPE, "PTCM2G_NPCFLNOTIFYM2G");
	CProtocol::RegistProtoName(PTCM2G_BATTLEFAILINFOGSNTF_TYPE, "PTCM2G_BATTLEFAILINFOGSNTF");
	CProtocol::RegistProtoName(PTCM2C_DAILYTASKEVENTNTF_TYPE, "PTCM2C_DAILYTASKEVENTNTF");
	CProtocol::RegistProtoName(PTCM2G_CONSUMEREBATENTF_TYPE, "PTCM2G_CONSUMEREBATENTF");
	CProtocol::RegistProtoName(PTCM2C_TASKREFRESHNTF_TYPE, "PTCM2C_TASKREFRESHNTF");
	CProtocol::RegistProtoName(PTCM2C_GUILDCASTFEATSNTF_TYPE, "PTCM2C_GUILDCASTFEATSNTF");
	CProtocol::RegistProtoName(PTCM2G_PAYTOTALNTF_TYPE, "PTCM2G_PAYTOTALNTF");
	CProtocol::RegistProtoName(RPCC2M_FETCHMAIL_TYPE, "RPCC2M_FETCHMAIL");
	CProtocol::RegistProtoName(RPCC2M_MAILOP_TYPE, "RPCC2M_MAILOP");
	CProtocol::RegistProtoName(RPCC2M_GETQADATAREQ_TYPE, "RPCC2M_GETQADATAREQ");
	CProtocol::RegistProtoName(RPCG2M_REGISTERGS2MS_TYPE, "RPCG2M_REGISTERGS2MS");
	CProtocol::RegistProtoName(RPCT2M_REGISTERGT2MS_TYPE, "RPCT2M_REGISTERGT2MS");
	CProtocol::RegistProtoName(RPCM2D_READACCOUNTDATA_TYPE, "RPCM2D_READACCOUNTDATA");
	CProtocol::RegistProtoName(RPCM2D_READROLEDATA_TYPE, "RPCM2D_READROLEDATA");
	CProtocol::RegistProtoName(RPCP2M_GMHTTPCOMMANDNEW_TYPE, "RPCP2M_GMHTTPCOMMANDNEW");
	CProtocol::RegistProtoName(RPCM2G_GSHTTPCOMMANDNEW_TYPE, "RPCM2G_GSHTTPCOMMANDNEW");
	CProtocol::RegistProtoName(RPCM2D_MSSAVEROLEDATA_TYPE, "RPCM2D_MSSAVEROLEDATA");
	CProtocol::RegistProtoName(RPCM2D_MSGETALLROLE_TYPE, "RPCM2D_MSGETALLROLE");
	CProtocol::RegistProtoName(RPCM2N_CREATEBATTLESCENE_TYPE, "RPCM2N_CREATEBATTLESCENE");
	CProtocol::RegistProtoName(RPCC2M_CHAT_TYPE, "RPCC2M_CHAT");
	CProtocol::RegistProtoName(RPCM2G_CHATCANSEND_TYPE, "RPCM2G_CHATCANSEND");
	CProtocol::RegistProtoName(RPCG2M_GETWATCHINFOBYTYPE_TYPE, "RPCG2M_GETWATCHINFOBYTYPE");
	CProtocol::RegistProtoName(RPCG2M_ENTERWATCHBATTLEMS_TYPE, "RPCG2M_ENTERWATCHBATTLEMS");
	CProtocol::RegistProtoName(RPCC2M_DOADDFRIENDNEW_TYPE, "RPCC2M_DOADDFRIENDNEW");
	CProtocol::RegistProtoName(RPCC2M_REMOVEFRIENDNEW_TYPE, "RPCC2M_REMOVEFRIENDNEW");
	CProtocol::RegistProtoName(RPCC2M_ADDBLACKLISTNEW_TYPE, "RPCC2M_ADDBLACKLISTNEW");
	CProtocol::RegistProtoName(RPCC2M_REMOVEBLACKLISTNEW_TYPE, "RPCC2M_REMOVEBLACKLISTNEW");
	CProtocol::RegistProtoName(RPCC2M_FRIENDGIFTOPNEW_TYPE, "RPCC2M_FRIENDGIFTOPNEW");
	CProtocol::RegistProtoName(RPCC2M_RANDOMFRIENDWAITLISTNEW_TYPE, "RPCC2M_RANDOMFRIENDWAITLISTNEW");
	CProtocol::RegistProtoName(RPCG2M_INCLEVELSEALBOSSCOUNT_TYPE, "RPCG2M_INCLEVELSEALBOSSCOUNT");
	CProtocol::RegistProtoName(RPCM2G_NOTICEGUILDBOSSDOENDTOGSN_TYPE, "RPCM2G_NOTICEGUILDBOSSDOENDTOGSN");
	CProtocol::RegistProtoName(RPCC2M_GETWORLDBOSSSTATENEW_TYPE, "RPCC2M_GETWORLDBOSSSTATENEW");
	CProtocol::RegistProtoName(RPCC2M_GETWORLDBOSSTIMELEFT_TYPE, "RPCC2M_GETWORLDBOSSTIMELEFT");
	CProtocol::RegistProtoName(RPCC2M_SHOWFLOWERPAGENEW_TYPE, "RPCC2M_SHOWFLOWERPAGENEW");
	CProtocol::RegistProtoName(RPCG2M_MSGETFLWOERRANKREWARD_TYPE, "RPCG2M_MSGETFLWOERRANKREWARD");
	CProtocol::RegistProtoName(RPCG2M_MSSENDFLOWER_TYPE, "RPCG2M_MSSENDFLOWER");
	CProtocol::RegistProtoName(RPCC2M_MSGETFLOWERREWARDLIST_TYPE, "RPCC2M_MSGETFLOWERREWARDLIST");
	CProtocol::RegistProtoName(RPCM2G_GETFLOWERREWARDSTATE_TYPE, "RPCM2G_GETFLOWERREWARDSTATE");
	CProtocol::RegistProtoName(RPCC2M_FETCHTEAMLISTC2M_TYPE, "RPCC2M_FETCHTEAMLISTC2M");
	CProtocol::RegistProtoName(RPCM2G_TEAMSYNM2C_TYPE, "RPCM2G_TEAMSYNM2C");
	CProtocol::RegistProtoName(RPCC2M_INVHISTORYC2MREQ_TYPE, "RPCC2M_INVHISTORYC2MREQ");
	CProtocol::RegistProtoName(RPCC2M_TEAMREQUESTC2M_TYPE, "RPCC2M_TEAMREQUESTC2M");
	CProtocol::RegistProtoName(RPCM2G_TEAMSTARTBATTLEM2G_TYPE, "RPCM2G_TEAMSTARTBATTLEM2G");
	CProtocol::RegistProtoName(RPCM2G_TEAMREQUESTM2G_TYPE, "RPCM2G_TEAMREQUESTM2G");
	CProtocol::RegistProtoName(RPCG2M_MSCOMMENDFIRSTPASS_TYPE, "RPCG2M_MSCOMMENDFIRSTPASS");
	CProtocol::RegistProtoName(RPCG2M_GETRANKINFO_TYPE, "RPCG2M_GETRANKINFO");
	CProtocol::RegistProtoName(RPCG2M_PVPALLG2MREQ_TYPE, "RPCG2M_PVPALLG2MREQ");
	CProtocol::RegistProtoName(RPCC2M_CLIENTQUERYRANKLISTNTF_TYPE, "RPCC2M_CLIENTQUERYRANKLISTNTF");
	CProtocol::RegistProtoName(RPCG2M_GMCOMMANDG2M_TYPE, "RPCG2M_GMCOMMANDG2M");
	CProtocol::RegistProtoName(RPCM2G_MSUSESUMMARYREQ_TYPE, "RPCM2G_MSUSESUMMARYREQ");
	CProtocol::RegistProtoName(RPCM2G_TEAMCHANGEEXPCHECK_TYPE, "RPCM2G_TEAMCHANGEEXPCHECK");
	CProtocol::RegistProtoName(RPCG2M_UPDATEMSRANKLIST_TYPE, "RPCG2M_UPDATEMSRANKLIST");
	CProtocol::RegistProtoName(RPCC2M_ADDFRIENDNEW_TYPE, "RPCC2M_ADDFRIENDNEW");
	CProtocol::RegistProtoName(RPCG2M_PKRANKREQ_TYPE, "RPCG2M_PKRANKREQ");
	CProtocol::RegistProtoName(RPCO2M_GMTOOLCOMMAND2MS_TYPE, "RPCO2M_GMTOOLCOMMAND2MS");
	CProtocol::RegistProtoName(RPCC2M_PKREQC2M_TYPE, "RPCC2M_PKREQC2M");
	CProtocol::RegistProtoName(RPCM2G_PKREQM2G_TYPE, "RPCM2G_PKREQM2G");
	CProtocol::RegistProtoName(RPCC2M_CLEARPRIVATECHATLIST_TYPE, "RPCC2M_CLEARPRIVATECHATLIST");
	CProtocol::RegistProtoName(RPCC2M_ASKGUILDBRIEFINFO_TYPE, "RPCC2M_ASKGUILDBRIEFINFO");
	CProtocol::RegistProtoName(RPCC2M_CREATEORENTERGUILD_TYPE, "RPCC2M_CREATEORENTERGUILD");
	CProtocol::RegistProtoName(RPCM2G_CREATEGUILDCOST_TYPE, "RPCM2G_CREATEGUILDCOST");
	CProtocol::RegistProtoName(RPCC2M_LEAVEFROMGUILD_TYPE, "RPCC2M_LEAVEFROMGUILD");
	CProtocol::RegistProtoName(RPCC2M_AUCTIONALLREQ_TYPE, "RPCC2M_AUCTIONALLREQ");
	CProtocol::RegistProtoName(RPCM2G_AUCTIONALLM2GREQ_TYPE, "RPCM2G_AUCTIONALLM2GREQ");
	CProtocol::RegistProtoName(RPCM2G_ENTERGUILDPOWER_TYPE, "RPCM2G_ENTERGUILDPOWER");
	CProtocol::RegistProtoName(RPCC2M_REQGUILDLIST_TYPE, "RPCC2M_REQGUILDLIST");
	CProtocol::RegistProtoName(RPCC2M_DERANKREQ_TYPE, "RPCC2M_DERANKREQ");
	CProtocol::RegistProtoName(RPCC2M_WORLDBOSSEND_TYPE, "RPCC2M_WORLDBOSSEND");
	CProtocol::RegistProtoName(RPCC2M_REQGUILDBOSSINFO_TYPE, "RPCC2M_REQGUILDBOSSINFO");
	CProtocol::RegistProtoName(RPCC2M_ASKGUILDMEMBERS_TYPE, "RPCC2M_ASKGUILDMEMBERS");
	CProtocol::RegistProtoName(RPCC2M_ASKGUILDSKILLINFONEW_TYPE, "RPCC2M_ASKGUILDSKILLINFONEW");
	CProtocol::RegistProtoName(RPCC2M_STUDYGUILDSKILLNEW_TYPE, "RPCC2M_STUDYGUILDSKILLNEW");
	CProtocol::RegistProtoName(RPCG2M_OPGUILDREQDATA_TYPE, "RPCG2M_OPGUILDREQDATA");
	CProtocol::RegistProtoName(RPCM2G_STARTARENAGS_TYPE, "RPCM2G_STARTARENAGS");
	CProtocol::RegistProtoName(RPCM2G_REQGUILDDAREREWARDREC_TYPE, "RPCM2G_REQGUILDDAREREWARDREC");
	CProtocol::RegistProtoName(RPCG2M_CHECKGUILDDAREREWARD_TYPE, "RPCG2M_CHECKGUILDDAREREWARD");
	CProtocol::RegistProtoName(RPCG2M_ENDAREANMS_TYPE, "RPCG2M_ENDAREANMS");
	CProtocol::RegistProtoName(RPCM2G_REQROLEGUILDSKILL_TYPE, "RPCM2G_REQROLEGUILDSKILL");
	CProtocol::RegistProtoName(RPCC2M_MTSHOWTOPLIST_TYPE, "RPCC2M_MTSHOWTOPLIST");
	CProtocol::RegistProtoName(RPCC2M_GETGUILDBOSSTIMELEFT_TYPE, "RPCC2M_GETGUILDBOSSTIMELEFT");
	CProtocol::RegistProtoName(RPCC2M_FETCHGUILDAPP_TYPE, "RPCC2M_FETCHGUILDAPP");
	CProtocol::RegistProtoName(RPCC2M_GUILDAPPROVALNEW_TYPE, "RPCC2M_GUILDAPPROVALNEW");
	CProtocol::RegistProtoName(RPCG2M_MGUILDBONUSOPERATEREQ_TYPE, "RPCG2M_MGUILDBONUSOPERATEREQ");
	CProtocol::RegistProtoName(RPCC2M_CHANGEGUILDSETTINGNEW_TYPE, "RPCC2M_CHANGEGUILDSETTINGNEW");
	CProtocol::RegistProtoName(RPCM2G_REQGUILDACTIVITY_TYPE, "RPCM2G_REQGUILDACTIVITY");
	CProtocol::RegistProtoName(RPCC2M_GETGUILDCHECKINRECORDSNEW_TYPE, "RPCC2M_GETGUILDCHECKINRECORDSNEW");
	CProtocol::RegistProtoName(RPCC2M_GUILDCHECKINNEW_TYPE, "RPCC2M_GUILDCHECKINNEW");
	CProtocol::RegistProtoName(RPCC2M_QUERYGUILDCHECKINNEW_TYPE, "RPCC2M_QUERYGUILDCHECKINNEW");
	CProtocol::RegistProtoName(RPCM2G_REQGUILDCHECKIN_TYPE, "RPCM2G_REQGUILDCHECKIN");
	CProtocol::RegistProtoName(RPCM2G_NOTICEGUILDCHECKIN_TYPE, "RPCM2G_NOTICEGUILDCHECKIN");
	CProtocol::RegistProtoName(RPCG2M_NOTIFYCHECKINBOX_TYPE, "RPCG2M_NOTIFYCHECKINBOX");
	CProtocol::RegistProtoName(RPCG2M_GUILDCARDSYNCMS_TYPE, "RPCG2M_GUILDCARDSYNCMS");
	CProtocol::RegistProtoName(RPCM2G_REQRECVFATIGUE_TYPE, "RPCM2G_REQRECVFATIGUE");
	CProtocol::RegistProtoName(RPCC2M_GUILDFATIGUEOPNEW_TYPE, "RPCC2M_GUILDFATIGUEOPNEW");
	CProtocol::RegistProtoName(RPCC2M_FETCHGUILDHISTORYNEW_TYPE, "RPCC2M_FETCHGUILDHISTORYNEW");
	CProtocol::RegistProtoName(RPCC2M_CHANGEMEMBERPOSITIONNEW_TYPE, "RPCC2M_CHANGEMEMBERPOSITIONNEW");
	CProtocol::RegistProtoName(RPCM2G_DESIGNATIONEVENTREQ_TYPE, "RPCM2G_DESIGNATIONEVENTREQ");
	CProtocol::RegistProtoName(RPCC2M_GETOTHERGUILDBRIEFNEW_TYPE, "RPCC2M_GETOTHERGUILDBRIEFNEW");
	CProtocol::RegistProtoName(RPCC2M_ASKGUILDARENAINFONEW_TYPE, "RPCC2M_ASKGUILDARENAINFONEW");
	CProtocol::RegistProtoName(RPCC2M_ASKGUILDARENATEAMINFONEW_TYPE, "RPCC2M_ASKGUILDARENATEAMINFONEW");
	CProtocol::RegistProtoName(RPCC2M_ADJUSTGUILDARENAROLEPOSNEW_TYPE, "RPCC2M_ADJUSTGUILDARENAROLEPOSNEW");
	CProtocol::RegistProtoName(RPCC2M_GUILDARENASIMPLEDEPLOYNEW_TYPE, "RPCC2M_GUILDARENASIMPLEDEPLOYNEW");
	CProtocol::RegistProtoName(RPCM2D_DBRELOAD_TYPE, "RPCM2D_DBRELOAD");
	CProtocol::RegistProtoName(RPCM2G_GSRELOAD_TYPE, "RPCM2G_GSRELOAD");
	CProtocol::RegistProtoName(RPCG2M_REQGUILDLADDERREWARD_TYPE, "RPCG2M_REQGUILDLADDERREWARD");
	CProtocol::RegistProtoName(RPCC2M_REQGUILDLADDERINFO_TYPE, "RPCC2M_REQGUILDLADDERINFO");
	CProtocol::RegistProtoName(RPCC2M_REQGUILDLADDERRNAKINFO_TYPE, "RPCC2M_REQGUILDLADDERRNAKINFO");
	CProtocol::RegistProtoName(RPCC2M_STARTPLANT_TYPE, "RPCC2M_STARTPLANT");
	CProtocol::RegistProtoName(RPCC2M_FETCHPLANTINFO_TYPE, "RPCC2M_FETCHPLANTINFO");
	CProtocol::RegistProtoName(RPCC2M_PLANTCULTIVATION_TYPE, "RPCC2M_PLANTCULTIVATION");
	CProtocol::RegistProtoName(RPCC2M_PLANTHARVEST_TYPE, "RPCC2M_PLANTHARVEST");
	CProtocol::RegistProtoName(RPCC2M_TRYFISH_TYPE, "RPCC2M_TRYFISH");
	CProtocol::RegistProtoName(RPCM2G_MSCONSUMEITEMS_TYPE, "RPCM2G_MSCONSUMEITEMS");
	CProtocol::RegistProtoName(RPCC2M_FRIENDGARDENPLANTLOG_TYPE, "RPCC2M_FRIENDGARDENPLANTLOG");
	CProtocol::RegistProtoName(RPCC2M_GARDENCOOKINGFOOD_TYPE, "RPCC2M_GARDENCOOKINGFOOD");
	CProtocol::RegistProtoName(RPCC2M_SKYCITYENTER_TYPE, "RPCC2M_SKYCITYENTER");
	CProtocol::RegistProtoName(RPCG2M_ENTERQAROOM_TYPE, "RPCG2M_ENTERQAROOM");
	CProtocol::RegistProtoName(RPCG2M_CREATEGUILDQAROOM_TYPE, "RPCG2M_CREATEGUILDQAROOM");
	CProtocol::RegistProtoName(RPCC2M_REQGUILDRANKINFO_TYPE, "RPCC2M_REQGUILDRANKINFO");
	CProtocol::RegistProtoName(RPCC2M_GETGUILDBINDINFO_TYPE, "RPCC2M_GETGUILDBINDINFO");
	CProtocol::RegistProtoName(RPCC2M_GUILDBINDGROUP_TYPE, "RPCC2M_GUILDBINDGROUP");
	CProtocol::RegistProtoName(RPCC2M_GUILDJOINBINDGROUP_TYPE, "RPCC2M_GUILDJOINBINDGROUP");
	CProtocol::RegistProtoName(RPCC2M_GUILDUNBINDGROUP_TYPE, "RPCC2M_GUILDUNBINDGROUP");
	CProtocol::RegistProtoName(RPCC2M_GMFJOINREQ_TYPE, "RPCC2M_GMFJOINREQ");
	CProtocol::RegistProtoName(RPCC2M_GARDENBANQUET_TYPE, "RPCC2M_GARDENBANQUET");
	CProtocol::RegistProtoName(RPCM2G_GARDENPLAYEVENT_TYPE, "RPCM2G_GARDENPLAYEVENT");
	CProtocol::RegistProtoName(RPCC2M_GARDENOVERVIEW_TYPE, "RPCC2M_GARDENOVERVIEW");
	CProtocol::RegistProtoName(RPCM2G_GARDENPLANTCHANGESTAGE_TYPE, "RPCM2G_GARDENPLANTCHANGESTAGE");
	CProtocol::RegistProtoName(RPCM2G_GETGUILDLADDERREWARDTIME_TYPE, "RPCM2G_GETGUILDLADDERREWARDTIME");
	CProtocol::RegistProtoName(RPCC2M_GARDENBANQUETAWARD_TYPE, "RPCC2M_GARDENBANQUETAWARD");
	CProtocol::RegistProtoName(RPCC2M_ACTIVECOOKBOOK_TYPE, "RPCC2M_ACTIVECOOKBOOK");
	CProtocol::RegistProtoName(RPCC2M_GARDENFISHINFO_TYPE, "RPCC2M_GARDENFISHINFO");
	CProtocol::RegistProtoName(RPCC2M_PAYNOTIFY_TYPE, "RPCC2M_PAYNOTIFY");
	CProtocol::RegistProtoName(RPCC2M_GARDENSTEAL_TYPE, "RPCC2M_GARDENSTEAL");
	CProtocol::RegistProtoName(RPCC2M_GARDENEXPELSPRITE_TYPE, "RPCC2M_GARDENEXPELSPRITE");
	CProtocol::RegistProtoName(RPCC2M_REQPLATFRIENDRANKLIST_TYPE, "RPCC2M_REQPLATFRIENDRANKLIST");
	CProtocol::RegistProtoName(RPCM2S_M2SREQPLATUSERINFO_TYPE, "RPCM2S_M2SREQPLATUSERINFO");
	CProtocol::RegistProtoName(RPCM2S_M2SLOGINANDOUTSYNC_TYPE, "RPCM2S_M2SLOGINANDOUTSYNC");
	CProtocol::RegistProtoName(RPCM2S_M2SSENDGIFT2PLATFRIEND_TYPE, "RPCM2S_M2SSENDGIFT2PLATFRIEND");
	CProtocol::RegistProtoName(RPCS2M_S2MSENDGIFT2PLATFRIEND_TYPE, "RPCS2M_S2MSENDGIFT2PLATFRIEND");
	CProtocol::RegistProtoName(RPCC2M_SENDGIFT2PLATFRIEND_TYPE, "RPCC2M_SENDGIFT2PLATFRIEND");
	CProtocol::RegistProtoName(RPCM2G_QUERYROLECARDMATCH_TYPE, "RPCM2G_QUERYROLECARDMATCH");
	CProtocol::RegistProtoName(RPCC2M_ASKGUILDWAGEINFO_TYPE, "RPCC2M_ASKGUILDWAGEINFO");
	CProtocol::RegistProtoName(RPCC2M_GETGUILDWAGEREWARD_TYPE, "RPCC2M_GETGUILDWAGEREWARD");
	CProtocol::RegistProtoName(RPCC2M_OPENGARDENFARMLAND_TYPE, "RPCC2M_OPENGARDENFARMLAND");
	CProtocol::RegistProtoName(RPCM2R_REGISTERMS2ROUTER_TYPE, "RPCM2R_REGISTERMS2ROUTER");
	CProtocol::RegistProtoName(RPCM2K_REGISTERMS2WORLD_TYPE, "RPCM2K_REGISTERMS2WORLD");
	CProtocol::RegistProtoName(RPCC2M_RESWAREXPLORE_TYPE, "RPCC2M_RESWAREXPLORE");
	CProtocol::RegistProtoName(RPCC2M_QUERYRESWAR_TYPE, "RPCC2M_QUERYRESWAR");
	CProtocol::RegistProtoName(RPCM2K_GETSCENEINFO_TYPE, "RPCM2K_GETSCENEINFO");
	CProtocol::RegistProtoName(RPCM2N_KICKACCOUNTFROMMS_TYPE, "RPCM2N_KICKACCOUNTFROMMS");
	CProtocol::RegistProtoName(RPCM2N_CHANGESCENEFROMMS_TYPE, "RPCM2N_CHANGESCENEFROMMS");
	CProtocol::RegistProtoName(RPCC2M_GUILDAUCTREQALL_TYPE, "RPCC2M_GUILDAUCTREQALL");
	CProtocol::RegistProtoName(RPCM2G_GIVEMAILPRIZE_TYPE, "RPCM2G_GIVEMAILPRIZE");
	CProtocol::RegistProtoName(RPCM2G_GUILDAUCTREQM2C_TYPE, "RPCM2G_GUILDAUCTREQM2C");
	CProtocol::RegistProtoName(RPCC2M_GETGUILDBONUSSENDLIST_TYPE, "RPCC2M_GETGUILDBONUSSENDLIST");
	CProtocol::RegistProtoName(RPCC2M_SENDGUILDBONUSINSENDLIST_TYPE, "RPCC2M_SENDGUILDBONUSINSENDLIST");
	CProtocol::RegistProtoName(RPCC2M_USEGUILDBUFF_TYPE, "RPCC2M_USEGUILDBUFF");
	CProtocol::RegistProtoName(RPCC2M_STARTRESWARPVE_TYPE, "RPCC2M_STARTRESWARPVE");
	CProtocol::RegistProtoName(RPCC2M_APPLYGUILDARENA_TYPE, "RPCC2M_APPLYGUILDARENA");
	CProtocol::RegistProtoName(RPCC2M_GETAPPLYGUILDLIST_TYPE, "RPCC2M_GETAPPLYGUILDLIST");
	CProtocol::RegistProtoName(RPCC2M_GETINTEGRALBATTLEINFO_TYPE, "RPCC2M_GETINTEGRALBATTLEINFO");
	CProtocol::RegistProtoName(RPCM2G_GUILDAUCTFAILBACK_TYPE, "RPCM2G_GUILDAUCTFAILBACK");
	CProtocol::RegistProtoName(RPCG2M_GSDESIGNATIONEVENTREQ_TYPE, "RPCG2M_GSDESIGNATIONEVENTREQ");
	CProtocol::RegistProtoName(RPCC2M_GETGUILDINTEGRALINFO_TYPE, "RPCC2M_GETGUILDINTEGRALINFO");
	CProtocol::RegistProtoName(RPCC2M_GUILDCAMPINFO_TYPE, "RPCC2M_GUILDCAMPINFO");
	CProtocol::RegistProtoName(RPCM2I_REGISTERMS2LOGIN_TYPE, "RPCM2I_REGISTERMS2LOGIN");
	CProtocol::RegistProtoName(RPCC2M_QUERYRESWARROLERANK_TYPE, "RPCC2M_QUERYRESWARROLERANK");
	CProtocol::RegistProtoName(RPCN2M_CHECKROLENAME_TYPE, "RPCN2M_CHECKROLENAME");
	CProtocol::RegistProtoName(RPCN2M_CHECKROLEBANINFO_TYPE, "RPCN2M_CHECKROLEBANINFO");
	CProtocol::RegistProtoName(RPCC2M_REQGUILDARENAHISTORY_TYPE, "RPCC2M_REQGUILDARENAHISTORY");
	CProtocol::RegistProtoName(RPCG2M_ASKGUILDHELP_TYPE, "RPCG2M_ASKGUILDHELP");
	CProtocol::RegistProtoName(RPCC2M_GETGUILDDONATEINFO_TYPE, "RPCC2M_GETGUILDDONATEINFO");
	CProtocol::RegistProtoName(RPCC2M_DONATEMEMBERITEM_TYPE, "RPCC2M_DONATEMEMBERITEM");
	CProtocol::RegistProtoName(RPCM2G_COSTDONATEITEM_TYPE, "RPCM2G_COSTDONATEITEM");
	CProtocol::RegistProtoName(RPCM2G_QUERYDONATEINFO_TYPE, "RPCM2G_QUERYDONATEINFO");
	CProtocol::RegistProtoName(RPCM2N_NSRELOAD_TYPE, "RPCM2N_NSRELOAD");
	CProtocol::RegistProtoName(RPCC2M_ADDGUILDINHERIT_TYPE, "RPCC2M_ADDGUILDINHERIT");
	CProtocol::RegistProtoName(RPCM2G_TRYADDGUILDINHERIT_TYPE, "RPCM2G_TRYADDGUILDINHERIT");
	CProtocol::RegistProtoName(RPCC2M_ACEPTGUILDINHERIT_TYPE, "RPCC2M_ACEPTGUILDINHERIT");
	CProtocol::RegistProtoName(RPCC2M_REQGUILDINHERITINFO_TYPE, "RPCC2M_REQGUILDINHERITINFO");
	CProtocol::RegistProtoName(RPCC2M_DELGUILDINHERIT_TYPE, "RPCC2M_DELGUILDINHERIT");
	CProtocol::RegistProtoName(RPCM2I_GETPLATNOTICEINFO_TYPE, "RPCM2I_GETPLATNOTICEINFO");
	CProtocol::RegistProtoName(RPCC2M_FETCHPLATNOTICE_TYPE, "RPCC2M_FETCHPLATNOTICE");
	CProtocol::RegistProtoName(RPCM2G_CRASHRECOVERM2G_TYPE, "RPCM2G_CRASHRECOVERM2G");
	CProtocol::RegistProtoName(RPCM2G_FETCHROLEINFOONMS_TYPE, "RPCM2G_FETCHROLEINFOONMS");
	CProtocol::RegistProtoName(RPCC2M_JOINFMROOM_TYPE, "RPCC2M_JOINFMROOM");
	CProtocol::RegistProtoName(RPCM2F_UPJOINFMROOM_TYPE, "RPCM2F_UPJOINFMROOM");
	CProtocol::RegistProtoName(RPCG2M_TRYTEAMCOSTREWARD_TYPE, "RPCG2M_TRYTEAMCOSTREWARD");
	CProtocol::RegistProtoName(RPCC2M_CHANGENAMENEW_TYPE, "RPCC2M_CHANGENAMENEW");
	CProtocol::RegistProtoName(RPCM2G_CHANGENAMEGS_TYPE, "RPCM2G_CHANGENAMEGS");
	CProtocol::RegistProtoName(RPCM2G_REQGUILDINHERITTIMES_TYPE, "RPCM2G_REQGUILDINHERITTIMES");
	CProtocol::RegistProtoName(RPCM2D_UPDATEPARTNERDATA_TYPE, "RPCM2D_UPDATEPARTNERDATA");
	CProtocol::RegistProtoName(RPCM2G_MAKEPARTNERM2G_TYPE, "RPCM2G_MAKEPARTNERM2G");
	CProtocol::RegistProtoName(RPCC2M_GETPARTNERINFO_TYPE, "RPCC2M_GETPARTNERINFO");
	CProtocol::RegistProtoName(RPCC2M_GETOTHERMENTORSTATUS_TYPE, "RPCC2M_GETOTHERMENTORSTATUS");
	CProtocol::RegistProtoName(RPCC2M_GETMYMENTORINFO_TYPE, "RPCC2M_GETMYMENTORINFO");
	CProtocol::RegistProtoName(RPCC2M_GETMYAPPLYSTUDENTINFO_TYPE, "RPCC2M_GETMYAPPLYSTUDENTINFO");
	CProtocol::RegistProtoName(RPCC2M_GETMYAPPLYMASTERINFO_TYPE, "RPCC2M_GETMYAPPLYMASTERINFO");
	CProtocol::RegistProtoName(RPCC2M_MENTORMYBEAPPLIEDMSG_TYPE, "RPCC2M_MENTORMYBEAPPLIEDMSG");
	CProtocol::RegistProtoName(RPCC2M_UPDATEMENTORAPPLYSTUDENTINFO_TYPE, "RPCC2M_UPDATEMENTORAPPLYSTUDENTINFO");
	CProtocol::RegistProtoName(RPCC2M_MENTORRELATIONOP_TYPE, "RPCC2M_MENTORRELATIONOP");
	CProtocol::RegistProtoName(RPCG2M_GETPARTNERINFOG2M_TYPE, "RPCG2M_GETPARTNERINFOG2M");
	CProtocol::RegistProtoName(RPCM2G_GETPARTNERRECORDM2G_TYPE, "RPCM2G_GETPARTNERRECORDM2G");
	CProtocol::RegistProtoName(RPCC2M_GETPARTNERDETAILINFO_TYPE, "RPCC2M_GETPARTNERDETAILINFO");
	CProtocol::RegistProtoName(RPCC2M_GETPARTNERLIVENESS_TYPE, "RPCC2M_GETPARTNERLIVENESS");
	CProtocol::RegistProtoName(RPCC2M_LEAVEPARTNER_TYPE, "RPCC2M_LEAVEPARTNER");
	CProtocol::RegistProtoName(RPCC2M_CANCELLEAVEPARTNER_TYPE, "RPCC2M_CANCELLEAVEPARTNER");
	CProtocol::RegistProtoName(RPCC2M_INVFIGHTREQALL_TYPE, "RPCC2M_INVFIGHTREQALL");
	CProtocol::RegistProtoName(RPCC2M_GCFCOMMONREQ_TYPE, "RPCC2M_GCFCOMMONREQ");
	CProtocol::RegistProtoName(RPCC2M_GCFREADYSINFOREQ_TYPE, "RPCC2M_GCFREADYSINFOREQ");
	CProtocol::RegistProtoName(RPCG2M_GETCHARM_TYPE, "RPCG2M_GETCHARM");
	CProtocol::RegistProtoName(RPCM2G_INVFIGHTM2GREQ_TYPE, "RPCM2G_INVFIGHTM2GREQ");
	CProtocol::RegistProtoName(RPCC2M_GETPARTNERSHOP_TYPE, "RPCC2M_GETPARTNERSHOP");
	CProtocol::RegistProtoName(RPCC2M_BUYPARTNERSHOPITEM_TYPE, "RPCC2M_BUYPARTNERSHOPITEM");
	CProtocol::RegistProtoName(RPCM2G_BUYPARTNERITEMM2G_TYPE, "RPCM2G_BUYPARTNERITEMM2G");
	CProtocol::RegistProtoName(RPCM2G_POKERTOURNAMENTSIGNUPCOST_TYPE, "RPCM2G_POKERTOURNAMENTSIGNUPCOST");
	CProtocol::RegistProtoName(RPCC2M_REQGUILDTERRCITYINFO_TYPE, "RPCC2M_REQGUILDTERRCITYINFO");
	CProtocol::RegistProtoName(RPCC2M_REQGUILDTERRCHALLINFO_TYPE, "RPCC2M_REQGUILDTERRCHALLINFO");
	CProtocol::RegistProtoName(RPCC2M_REQGUILDTERRALLIANCEINFO_TYPE, "RPCC2M_REQGUILDTERRALLIANCEINFO");
	CProtocol::RegistProtoName(RPCC2M_GETPARTNERSHOPRECORD_TYPE, "RPCC2M_GETPARTNERSHOPRECORD");
	CProtocol::RegistProtoName(RPCC2M_ALLIANCEGUILDTERR_TYPE, "RPCC2M_ALLIANCEGUILDTERR");
	CProtocol::RegistProtoName(RPCC2M_GCFFIGHTINFOREQC2M_TYPE, "RPCC2M_GCFFIGHTINFOREQC2M");
	CProtocol::RegistProtoName(RPCC2M_RECALLIANCE_TYPE, "RPCC2M_RECALLIANCE");
	CProtocol::RegistProtoName(RPCC2M_TRYALLIANCE_TYPE, "RPCC2M_TRYALLIANCE");
	CProtocol::RegistProtoName(RPCC2M_CLEARGUILDTERRALLIANCE_TYPE, "RPCC2M_CLEARGUILDTERRALLIANCE");
	CProtocol::RegistProtoName(RPCM2D_SAVEPAYACCESSDATA_TYPE, "RPCM2D_SAVEPAYACCESSDATA");
	CProtocol::RegistProtoName(RPCM2G_PAYGETBALANCE_TYPE, "RPCM2G_PAYGETBALANCE");
	CProtocol::RegistProtoName(RPCM2G_PAYCONSUMEADDGOODS_TYPE, "RPCM2G_PAYCONSUMEADDGOODS");
	CProtocol::RegistProtoName(RPCC2M_GETUNITAPPEARANCENEW_TYPE, "RPCC2M_GETUNITAPPEARANCENEW");
	CProtocol::RegistProtoName(RPCM2G_GCFCOMMONREQM2G_TYPE, "RPCM2G_GCFCOMMONREQM2G");
	CProtocol::RegistProtoName(RPCM2D_SAVELEAGUETEAMDATA_TYPE, "RPCM2D_SAVELEAGUETEAMDATA");
	CProtocol::RegistProtoName(RPCC2M_GETLEAGUEBATTLEINFO_TYPE, "RPCC2M_GETLEAGUEBATTLEINFO");
	CProtocol::RegistProtoName(RPCC2M_GETLEAGUETEAMINFO_TYPE, "RPCC2M_GETLEAGUETEAMINFO");
	CProtocol::RegistProtoName(RPCG2M_GETGARDENINFO_TYPE, "RPCG2M_GETGARDENINFO");
	CProtocol::RegistProtoName(RPCC2M_LEAVELEAGUETEAM_TYPE, "RPCC2M_LEAVELEAGUETEAM");
	CProtocol::RegistProtoName(RPCC2M_LEAGUEBATTLEREQ_TYPE, "RPCC2M_LEAGUEBATTLEREQ");
	CProtocol::RegistProtoName(RPCC2M_GETLISTSUBSCRIBE_TYPE, "RPCC2M_GETLISTSUBSCRIBE");
	CProtocol::RegistProtoName(RPCC2M_SETSUBSCRIBE_TYPE, "RPCC2M_SETSUBSCRIBE");
	CProtocol::RegistProtoName(RPCG2M_LEAGUERANKUPDATEG2M_TYPE, "RPCG2M_LEAGUERANKUPDATEG2M");
	CProtocol::RegistProtoName(RPCG2M_CHANGEPROFESSIONMS_TYPE, "RPCG2M_CHANGEPROFESSIONMS");
	CProtocol::RegistProtoName(RPCC2M_GETLEAGUEBATTLERECORD_TYPE, "RPCC2M_GETLEAGUEBATTLERECORD");
	CProtocol::RegistProtoName(RPCC2M_GETLEAGUEELEINFO_TYPE, "RPCC2M_GETLEAGUEELEINFO");
	CProtocol::RegistProtoName(RPCC2M_JOINLEAGUEELEBATTLE_TYPE, "RPCC2M_JOINLEAGUEELEBATTLE");
	CProtocol::RegistProtoName(RPCM2S_REGISTERMS2CS_TYPE, "RPCM2S_REGISTERMS2CS");
	CProtocol::RegistProtoName(RPCG2M_PERSONALCAREERTOTALINFO_TYPE, "RPCG2M_PERSONALCAREERTOTALINFO");
	CProtocol::RegistProtoName(RPCC2M_CLICKNEWNOTICE_TYPE, "RPCC2M_CLICKNEWNOTICE");
	CProtocol::RegistProtoName(RPCM2K_LEAGUEREPORTTOP2WORLD_TYPE, "RPCM2K_LEAGUEREPORTTOP2WORLD");
	CProtocol::RegistProtoName(RPCM2K_PKMATCHTOWORLDREQ_TYPE, "RPCM2K_PKMATCHTOWORLDREQ");
	CProtocol::RegistProtoName(RPCK2M_GETROLESUMMARYFROMMS_TYPE, "RPCK2M_GETROLESUMMARYFROMMS");
	CProtocol::RegistProtoName(RPCM2G_CHECKNOTICE_TYPE, "RPCM2G_CHECKNOTICE");
	CProtocol::RegistProtoName(RPCM2F_REGISTERMS2FM_TYPE, "RPCM2F_REGISTERMS2FM");
	CProtocol::RegistProtoName(RPCM2K_GMCOMMANDM2W_TYPE, "RPCM2K_GMCOMMANDM2W");
	CProtocol::RegistProtoName(RPCC2M_REQGUILDTERRINTELLINFO_TYPE, "RPCC2M_REQGUILDTERRINTELLINFO");
	CProtocol::RegistProtoName(RPCG2M_GETMULACTOPENINFO_TYPE, "RPCG2M_GETMULACTOPENINFO");
	CProtocol::RegistProtoName(RPCG2M_GETMILITARYRANK_TYPE, "RPCG2M_GETMILITARYRANK");
	CProtocol::RegistProtoName(RPCG2M_UPPAYFRIENDITEM_TYPE, "RPCG2M_UPPAYFRIENDITEM");
	CProtocol::RegistProtoName(RPCG2M_SYNCACTIVITYEND2MS_TYPE, "RPCG2M_SYNCACTIVITYEND2MS");
	CProtocol::RegistProtoName(RPCM2D_MSSAVEDBEND_TYPE, "RPCM2D_MSSAVEDBEND");
	CProtocol::RegistProtoName(RPCM2K_QUERYROUTER_TYPE, "RPCM2K_QUERYROUTER");
	CProtocol::RegistProtoName(RPCC2M_QUERYRANKLISTLUA_TYPE, "RPCC2M_QUERYRANKLISTLUA");
	CProtocol::RegistProtoName(RPCM2S_INVFIGHTCROSS2CSREQ_TYPE, "RPCM2S_INVFIGHTCROSS2CSREQ");
	CProtocol::RegistProtoName(RPCS2M_INVFIGHTCROSS2MSREQ_TYPE, "RPCS2M_INVFIGHTCROSS2MSREQ");
	CProtocol::RegistProtoName(RPCM2K_CREATECROSSBATTLESCENE_TYPE, "RPCM2K_CREATECROSSBATTLESCENE");
	CProtocol::RegistProtoName(RPCK2M_NOTIFYENTERCROSSBATTLE2MS_TYPE, "RPCK2M_NOTIFYENTERCROSSBATTLE2MS");
	CProtocol::RegistProtoName(RPCC2M_WORLDBOSSGUILDADDATTR_TYPE, "RPCC2M_WORLDBOSSGUILDADDATTR");
	CProtocol::RegistProtoName(RPCM2G_WORLDBOSSGUILDADDATTRGS_TYPE, "RPCM2G_WORLDBOSSGUILDADDATTRGS");
	CProtocol::RegistProtoName(RPCC2M_MODIFYMSGUILDNAME_TYPE, "RPCC2M_MODIFYMSGUILDNAME");
	CProtocol::RegistProtoName(RPCM2G_MODIFYGSGUILDNAME_TYPE, "RPCM2G_MODIFYGSGUILDNAME");
	CProtocol::RegistProtoName(RPCM2D_QUERYROLESUMMARY_TYPE, "RPCM2D_QUERYROLESUMMARY");
	CProtocol::RegistProtoName(RPCM2D_QUERYGARDENINFO_TYPE, "RPCM2D_QUERYGARDENINFO");
	CProtocol::RegistProtoName(RPCM2G_GIFTIBITEMREQ_TYPE, "RPCM2G_GIFTIBITEMREQ");
	CProtocol::RegistProtoName(RPCM2D_SAVEGIFTIBITEM_TYPE, "RPCM2D_SAVEGIFTIBITEM");
	CProtocol::RegistProtoName(RPCM2D_ARENASTARROLEDATASAVE_TYPE, "RPCM2D_ARENASTARROLEDATASAVE");
	CProtocol::RegistProtoName(RPCC2M_ARENASTARROLEREQ_TYPE, "RPCC2M_ARENASTARROLEREQ");
	CProtocol::RegistProtoName(RPCM2D_SKYTEAMDATA2DB_TYPE, "RPCM2D_SKYTEAMDATA2DB");
	CProtocol::RegistProtoName(RPCC2M_LEAVESKYTEAM_TYPE, "RPCC2M_LEAVESKYTEAM");
	CProtocol::RegistProtoName(RPCC2M_SKYCRAFTMATCHREQ_TYPE, "RPCC2M_SKYCRAFTMATCHREQ");
	CProtocol::RegistProtoName(RPCC2M_CUSTOMBATTLEOP_TYPE, "RPCC2M_CUSTOMBATTLEOP");
	CProtocol::RegistProtoName(RPCM2K_CUSTOMBATTLEWORLDOP_TYPE, "RPCM2K_CUSTOMBATTLEWORLDOP");
	CProtocol::RegistProtoName(RPCC2M_GETSKYCRAFTINFO_TYPE, "RPCC2M_GETSKYCRAFTINFO");
	CProtocol::RegistProtoName(RPCC2M_GETSKYCRAFTRECORD_TYPE, "RPCC2M_GETSKYCRAFTRECORD");
	CProtocol::RegistProtoName(RPCC2M_GETSKYCRAFTELIINFO_TYPE, "RPCC2M_GETSKYCRAFTELIINFO");
	CProtocol::RegistProtoName(RPCC2M_GETSKYCRAFTTEAMINFO_TYPE, "RPCC2M_GETSKYCRAFTTEAMINFO");
	CProtocol::RegistProtoName(RPCM2D_SAVEGIFTIBHIST_TYPE, "RPCM2D_SAVEGIFTIBHIST");
	CProtocol::RegistProtoName(RPCC2M_IBGIFTHISTREQ_TYPE, "RPCC2M_IBGIFTHISTREQ");
	CProtocol::RegistProtoName(RPCG2M_PAYDEGREEREQ_TYPE, "RPCG2M_PAYDEGREEREQ");
	CProtocol::RegistProtoName(RPCM2G_CUSTOMBATTLEGSOP_TYPE, "RPCM2G_CUSTOMBATTLEGSOP");
	CProtocol::RegistProtoName(RPCC2M_GIFTIBREQGOODS_TYPE, "RPCC2M_GIFTIBREQGOODS");
	CProtocol::RegistProtoName(RPCM2G_GIFTIBREQGSGOODS_TYPE, "RPCM2G_GIFTIBREQGSGOODS");
	CProtocol::RegistProtoName(RPCM2K_CUSTOMBATTLECHECKVALID_TYPE, "RPCM2K_CUSTOMBATTLECHECKVALID");
	CProtocol::RegistProtoName(RPCG2M_CUSTOMBATTLERESULTMS_TYPE, "RPCG2M_CUSTOMBATTLERESULTMS");
	CProtocol::RegistProtoName(RPCM2H_REGISTERMS2TEAM_TYPE, "RPCM2H_REGISTERMS2TEAM");
	CProtocol::RegistProtoName(RPCM2H_TEAMTRANMS2TS_TYPE, "RPCM2H_TEAMTRANMS2TS");
	CProtocol::RegistProtoName(RPCH2M_TEAMTRANTS2MS_TYPE, "RPCH2M_TEAMTRANTS2MS");
	CProtocol::RegistProtoName(RPCM2G_TEAMTRANMS2GS_TYPE, "RPCM2G_TEAMTRANMS2GS");
	CProtocol::RegistProtoName(RPCM2S_TEAMREQUESTPLATFREINDM2S_TYPE, "RPCM2S_TEAMREQUESTPLATFREINDM2S");
	CProtocol::RegistProtoName(RPCS2M_TEAMREQUESTPLATFREINDS2M_TYPE, "RPCS2M_TEAMREQUESTPLATFREINDS2M");
	CProtocol::RegistProtoName(RPCC2M_KMATCHCOMMONREQ_TYPE, "RPCC2M_KMATCHCOMMONREQ");
	CProtocol::RegistProtoName(RPCM2K_KMATCHREQM2K_TYPE, "RPCM2K_KMATCHREQM2K");
	CProtocol::RegistProtoName(RPCM2G_KMATCHCOMMONREQM2C_TYPE, "RPCM2G_KMATCHCOMMONREQM2C");
	CProtocol::RegistProtoName(RPCM2D_UPDATEROLEREPORTDATA_TYPE, "RPCM2D_UPDATEROLEREPORTDATA");
	CProtocol::RegistProtoName(RPCC2M_GETFLOWERACTIVITYREWARD_TYPE, "RPCC2M_GETFLOWERACTIVITYREWARD");
	CProtocol::RegistProtoName(RPCM2N_REGISTERMS2NS_TYPE, "RPCM2N_REGISTERMS2NS");
	CProtocol::RegistProtoName(RPCM2G_TRYTEACHERINHERITNEW_TYPE, "RPCM2G_TRYTEACHERINHERITNEW");
	CProtocol::RegistProtoName(RPCC2M_GETMOBABATTLEGAMERECORD_TYPE, "RPCC2M_GETMOBABATTLEGAMERECORD");
	CProtocol::RegistProtoName(RPCC2M_GETMOBABATTLEBRIEFRECORD_TYPE, "RPCC2M_GETMOBABATTLEBRIEFRECORD");
	CProtocol::RegistProtoName(RPCM2D_QUERYMOBAROLERECORD_TYPE, "RPCM2D_QUERYMOBAROLERECORD");
	CProtocol::RegistProtoName(RPCC2M_ENTERBMREADYSCENE_TYPE, "RPCC2M_ENTERBMREADYSCENE");
	CProtocol::RegistProtoName(RPCC2M_QUERYBIGMELEERANK_TYPE, "RPCC2M_QUERYBIGMELEERANK");
	CProtocol::RegistProtoName(RPCM2K_QUERYBIGMELEERANK2K_TYPE, "RPCM2K_QUERYBIGMELEERANK2K");
	CProtocol::RegistProtoName(RPCM2G_MSGIVEITEM_TYPE, "RPCM2G_MSGIVEITEM");
	CProtocol::RegistProtoName(RPCC2M_GETMOBABATTLEINFO_TYPE, "RPCC2M_GETMOBABATTLEINFO");
	CProtocol::RegistProtoName(RPCC2M_GETMOBABATTLEWEEKREWARD_TYPE, "RPCC2M_GETMOBABATTLEWEEKREWARD");
	CProtocol::RegistProtoName(RPCK2M_TEAMSTARTBATTLEW2M_TYPE, "RPCK2M_TEAMSTARTBATTLEW2M");
	CProtocol::RegistProtoName(RPCM2G_TEAMSTARTBATTLEM2GCROSS_TYPE, "RPCM2G_TEAMSTARTBATTLEM2GCROSS");
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
	CProtocol::RegistProtoName(RPCC2M_GROUPCHATCLEAR_TYPE, "RPCC2M_GROUPCHATCLEAR");
	CProtocol::RegistProtoName(RPCC2M_HOLDWEDDING_TYPE, "RPCC2M_HOLDWEDDING");
	CProtocol::RegistProtoName(RPCC2M_GETALLWEDDINGINFO_TYPE, "RPCC2M_GETALLWEDDINGINFO");
	CProtocol::RegistProtoName(RPCC2M_GETWEDDINGINVITEINFO_TYPE, "RPCC2M_GETWEDDINGINVITEINFO");
	CProtocol::RegistProtoName(RPCC2M_ENTERWEDDINGSCENE_TYPE, "RPCC2M_ENTERWEDDINGSCENE");
	CProtocol::RegistProtoName(RPCC2M_GETMARRIAGERELATION_TYPE, "RPCC2M_GETMARRIAGERELATION");
	CProtocol::RegistProtoName(RPCC2M_MARRIAGERELATIONOP_TYPE, "RPCC2M_MARRIAGERELATIONOP");
	CProtocol::RegistProtoName(RPCC2M_WEDDINGINVITEOPERATOR_TYPE, "RPCC2M_WEDDINGINVITEOPERATOR");
	CProtocol::RegistProtoName(RPCC2M_GROUPCHATLEADERADDROLE_TYPE, "RPCC2M_GROUPCHATLEADERADDROLE");
	CProtocol::RegistProtoName(RPCM2G_CHECKMARRIAGECOUPLE_TYPE, "RPCM2G_CHECKMARRIAGECOUPLE");
	CProtocol::RegistProtoName(RPCC2M_GROUPCHATCHANGENAME_TYPE, "RPCC2M_GROUPCHATCHANGENAME");
	CProtocol::RegistProtoName(RPCC2M_GETMARRIAGELIVENESS_TYPE, "RPCC2M_GETMARRIAGELIVENESS");
	CProtocol::RegistProtoName(RPCC2M_TAKEMARRIAGECHEST_TYPE, "RPCC2M_TAKEMARRIAGECHEST");
	CProtocol::RegistProtoName(RPCC2M_CHECKROLEBEFOREPAY_TYPE, "RPCC2M_CHECKROLEBEFOREPAY");
	CProtocol::RegistProtoName(RPCM2G_GIVEMARRIAGELVREREWARDM2G_TYPE, "RPCM2G_GIVEMARRIAGELVREREWARDM2G");
	CProtocol::RegistProtoName(RPCM2G_HOLDWEDDINGCHECKM2G_TYPE, "RPCM2G_HOLDWEDDINGCHECKM2G");
	CProtocol::RegistProtoName(RPCC2M_STARTWEDDINGCAR_TYPE, "RPCC2M_STARTWEDDINGCAR");
	CProtocol::RegistProtoName(RPCM2G_WEDDINGOPERM2G_TYPE, "RPCM2G_WEDDINGOPERM2G");
	CProtocol::RegistProtoName(RPCM2N_GMDOWHITELIST_TYPE, "RPCM2N_GMDOWHITELIST");
	CProtocol::RegistProtoName(RPCC2M_ENTERBATTLEREADYSCENE_TYPE, "RPCC2M_ENTERBATTLEREADYSCENE");
	CProtocol::RegistProtoName(RPCC2M_GETDRAGONGUILDLIVENESS_TYPE, "RPCC2M_GETDRAGONGUILDLIVENESS");
	CProtocol::RegistProtoName(RPCM2G_CREATEDRAGONGUILDCOST_TYPE, "RPCM2G_CREATEDRAGONGUILDCOST");
	CProtocol::RegistProtoName(RPCM2G_GETROLEPOWER_TYPE, "RPCM2G_GETROLEPOWER");
	CProtocol::RegistProtoName(RPCC2M_CREATEORJOINDRAGONGUILD_TYPE, "RPCC2M_CREATEORJOINDRAGONGUILD");
	CProtocol::RegistProtoName(RPCM2G_GETDRAGONGUILDRECORDM2G_TYPE, "RPCM2G_GETDRAGONGUILDRECORDM2G");
	CProtocol::RegistProtoName(RPCG2M_GETDRAGONGUILDINFOG2M_TYPE, "RPCG2M_GETDRAGONGUILDINFOG2M");
	CProtocol::RegistProtoName(RPCC2M_FETCHDGAPPS_TYPE, "RPCC2M_FETCHDGAPPS");
	CProtocol::RegistProtoName(RPCC2M_CHANGEDRAGONGUILDSETTING_TYPE, "RPCC2M_CHANGEDRAGONGUILDSETTING");
	CProtocol::RegistProtoName(RPCC2M_DRAGONGUILDAPPROVAL_TYPE, "RPCC2M_DRAGONGUILDAPPROVAL");
	CProtocol::RegistProtoName(RPCC2M_LEAVEFROMDRAGONGUILD_TYPE, "RPCC2M_LEAVEFROMDRAGONGUILD");
	CProtocol::RegistProtoName(RPCM2G_MODIFYDRAGONGUILDNAMECOST_TYPE, "RPCM2G_MODIFYDRAGONGUILDNAMECOST");
	CProtocol::RegistProtoName(RPCC2M_MODIFYDRAGONGUILDNAME_TYPE, "RPCC2M_MODIFYDRAGONGUILDNAME");
	CProtocol::RegistProtoName(RPCC2M_ASKDRAGONGUILDMEMBERS_TYPE, "RPCC2M_ASKDRAGONGUILDMEMBERS");
	CProtocol::RegistProtoName(RPCC2M_FETCHDRAGONGUILDLIST_TYPE, "RPCC2M_FETCHDRAGONGUILDLIST");
	CProtocol::RegistProtoName(RPCC2M_GETDRAGONGUILDTASKINFO_TYPE, "RPCC2M_GETDRAGONGUILDTASKINFO");
	CProtocol::RegistProtoName(RPCG2M_GETDRAGONGUILDTASKCHESTG2M_TYPE, "RPCG2M_GETDRAGONGUILDTASKCHESTG2M");
	CProtocol::RegistProtoName(RPCM2G_GETDRAGONGUILDRECIEVEINFO_TYPE, "RPCM2G_GETDRAGONGUILDRECIEVEINFO");
	CProtocol::RegistProtoName(RPCC2M_BUYDRAGONGUILDSHOPITEM_TYPE, "RPCC2M_BUYDRAGONGUILDSHOPITEM");
	CProtocol::RegistProtoName(RPCC2M_GETDRAGONGUILDSHOP_TYPE, "RPCC2M_GETDRAGONGUILDSHOP");
	CProtocol::RegistProtoName(RPCM2G_BUYDRAGONGUILDITEMM2G_TYPE, "RPCM2G_BUYDRAGONGUILDITEMM2G");
	CProtocol::RegistProtoName(RPCC2M_GETDRAGONGUILDSHOPRECORD_TYPE, "RPCC2M_GETDRAGONGUILDSHOPRECORD");
	CProtocol::RegistProtoName(RPCC2M_CHANGEDRAGONGUILDPOSITION_TYPE, "RPCC2M_CHANGEDRAGONGUILDPOSITION");
	CProtocol::RegistProtoName(RPCC2M_GETDRAGONGUILDBINDINFO_TYPE, "RPCC2M_GETDRAGONGUILDBINDINFO");
	CProtocol::RegistProtoName(RPCC2M_DRAGONGUILDBINDGROUPREQ_TYPE, "RPCC2M_DRAGONGUILDBINDGROUPREQ");
	CProtocol::RegistProtoName(RPCC2M_DRAGONGUILDJOINBINDGROUP_TYPE, "RPCC2M_DRAGONGUILDJOINBINDGROUP");
	CProtocol::RegistProtoName(RPCC2M_DRAGONGUILDUNBINDGROUP_TYPE, "RPCC2M_DRAGONGUILDUNBINDGROUP");
	CProtocol::RegistProtoName(RPCG2M_CHECKDRAGONGUILDREDPOINTG2M_TYPE, "RPCG2M_CHECKDRAGONGUILDREDPOINTG2M");
	CProtocol::RegistProtoName(RPCC2M_GOALAWARDSGETLIST_TYPE, "RPCC2M_GOALAWARDSGETLIST");
	CProtocol::RegistProtoName(RPCC2M_GOALAWARDSGETAWARDS_TYPE, "RPCC2M_GOALAWARDSGETAWARDS");
	CProtocol::RegistProtoName(RPCG2M_TASKDONATEG2M_TYPE, "RPCG2M_TASKDONATEG2M");
	CProtocol::RegistProtoName(RPCC2M_NPCFLREQC2M_TYPE, "RPCC2M_NPCFLREQC2M");
	CProtocol::RegistProtoName(RPCM2G_NPCFLREQM2G_TYPE, "RPCM2G_NPCFLREQM2G");
	CProtocol::RegistProtoName(RPCC2M_GETDAILYTASKREFRESHINFO_TYPE, "RPCC2M_GETDAILYTASKREFRESHINFO");
	CProtocol::RegistProtoName(RPCC2M_GETDAILYTASKASKHELP_TYPE, "RPCC2M_GETDAILYTASKASKHELP");
	CProtocol::RegistProtoName(RPCC2M_GETDAILYTASKREFRESHRECORD_TYPE, "RPCC2M_GETDAILYTASKREFRESHRECORD");
	CProtocol::RegistProtoName(RPCC2M_DAILYTASKREFRESHOPER_TYPE, "RPCC2M_DAILYTASKREFRESHOPER");
	CProtocol::RegistProtoName(RPCM2G_DAILYTASKREFRESHM2G_TYPE, "RPCM2G_DAILYTASKREFRESHM2G");
	CProtocol::RegistProtoName(RPCG2M_DAILYTASKREFRESHG2M_TYPE, "RPCG2M_DAILYTASKREFRESHG2M");
	CProtocol::RegistProtoName(RPCM2D_UPDATEROLETASKDATA_TYPE, "RPCM2D_UPDATEROLETASKDATA");
	CProtocol::RegistProtoName(RPCG2M_SURVIVESYNPOINTG2M_TYPE, "RPCG2M_SURVIVESYNPOINTG2M");
	CProtocol::RegistProtoName(RPCG2M_GETDAILYTASKG2M_TYPE, "RPCG2M_GETDAILYTASKG2M");
	CProtocol::RegistProtoName(RPCG2M_GETNEWZONEBENEFITG2M_TYPE, "RPCG2M_GETNEWZONEBENEFITG2M");
	CProtocol::RegistProtoName(RPCM2I_GETZONEROLEINFOM2I_TYPE, "RPCM2I_GETZONEROLEINFOM2I");
	CProtocol::RegistProtoName(RPCI2M_GETZONEROLEINFOI2M_TYPE, "RPCI2M_GETZONEROLEINFOI2M");
	CProtocol::RegistProtoName(RPCM2D_READACCOUNTDATANEW_TYPE, "RPCM2D_READACCOUNTDATANEW");
	CProtocol::RegistProtoName(RPCM2D_READMAILFROMDB_TYPE, "RPCM2D_READMAILFROMDB");

}