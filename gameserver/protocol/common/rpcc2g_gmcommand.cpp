#include "pch.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "common/rpcc2g_gmcommand.h"
#include "cmdline.h"
#include "util.h"
#include "gamelogic/rolenethandler.h"
#include "gamelogic/bagtransition.h"
#include "role/ptcg2c_opensystemntf.h"
#include "foreach.h"
#include "scene/scenewedding.h"
#include "gamelogic/activityrecord.h"
#include "gamelogic/activitymgr.h"
#include "gamelogic/arenarecord.h"
#include "gamelogic/arenamgr.h"
#include "gamelogic/opensystemhandler.h"
#include "scene/scenemanager.h"
#include "unit/enemymanager.h"
#include "entity/XRole.h"
#include "scene/scene.h"
#include "gamelogic/rewardmgr.h"
#include "gamelogic/stagemgr.h"
#include "gamelogic/chatmgr.h"
#include "gamelogic/noticemgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "unit/attributewatcher.h"
#include "scene/positiondebugmgr.h"
#include "entity/XObject.h"
#include "component/XBuffComponent.h"
#include "scene/grid.h"
#include "scene/scenepk.h"
#include "gamelogic/pkmgr.h"
#include "gamelogic/pkrecord.h"
#include "gamelogic/rolefashion.h"
#include "gamelogic/itemconfig.h"
#include "component/XNavigationComponent.h"
#include "weblink.h"
#include "gatelink.h"
#include "unit/enemy.h"
#include "unit/enemymanager.h"
#include "gamelogic/rolesummarymgr.h"
#include "unit/dummyrole.h"
#include "gamelogic/lotteryrecord.h"
#include "unit/dummyrolemanager.h"
#include "table/PlayerLevelManager.h"
#include "gamelogic/tshowrecord.h"
#include "gamelogic/teamrequest.h"
#include "camp/camprecord.h"
#include "gamelogic/robotconfig.h"
#include "gamelogic/teammgr.h"
#include "gamelogic/team.h"
#include "gamelogic/robotgenerator.h"
#include "findback/findbackrecord.h"
#include "gamelogic/buyexchangemgr.h"
#include "forbid/forbidwordmgr.h"
#include "gamelogic/designationMgr.h"
#include "gamelogic/designationRecord.h"
#include "gamelogic/achievev2Record.h"
#include "gamelogic/achievev2Mgr.h"
#include "gamelogic/levelsealMgr.h"
#include "gamelogic/loginrecord.h"
#include "gamelogic/pvpmgr.h"
#include "gamelogic/pvprecord.h"
#include "gamelogic/pvprequest.h"
#include "unit/unit.h"
#include "component/XAIComponent.h"
#include "gamelogic/QAMgr.h"
#include "gamelogic/QARecord.h"
#include "gamelogic/teaminvitemgr.h"
#include "gamelogic/MultActivityMgr.h"
#include "gamelogic/dragonrecord.h"
#include "guild/guildrecord.h"
#include "gamelogic/bossrushmgr.h"
#include "guild/guildboss.h"
#include "unit/roleoutlook.h"
#include "scene/sceneswitch.h"
#include "utility/uidhelper.h"
#include "gamelogic/enhanceequipmgr.h"
#include "gamelogic/itemsuffix.h"
#include "gamelogic/item.h"
#include "gamelogic/chatrecord.h"
#include "scene/scenefinder.h"
#include "levelseal/ptcg2m_gmlevelsealinfo.h"
#include "network/mslink.h"
#include "mail/mailsender.h"
#include "guild/ptcg2m_gmsetguildbosstime.h"
#include "table/XSkillEffectMgr.h"
#include "table/expeditionconfigmgr.h"
#include "scene/attributepk.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "table/OpenSystemMgr.h"
#include "gamelogic/shop.h"
#include "apollovoice/ApolloVoiceMgr.h"
#include "gamelogic/payv2Mgr.h"
#include "common/ptcg2m_reloadversionntf.h"
#include "ai/aionlinedebugmgr.h"
#include "buff/XBuff.h"
#include "common/rpcg2m_gmcommandg2m.h"
#include "gamelogic/firstpassrecord.h"
#include "worldboss/ptcg2m_worldbosssyncms.h"
#include "gamelogic/ibshopmgr.h"
#include "gamelogic/ibshopdef.h"
#include "gamelogic/spriteconfig.h"
#include "gamelogic/spritetransition.h"
#include "scene/DragonExpConfig.h"
#include "gamelogic/superriskrecord.h"
#include "gamelogic/titleconfig.h"
#include "gamelogic/titlerecord.h"
#include "gamelogic/towerrecord.h"
#include "config/itemdropconfig.h"
#include "idip/idiprecord.h"
#include "arena/rpcg2m_endareanms.h"
#include "gamelogic/stagemgr.h"
#include "live/livemanager.h"
#include "gamelogic/taskrecord.h"
#include "scene/scenegmf.h"
#include "scene/sceneinspire.h"
#include "gamelogic/MultActivityMgr.h"
#include "spactivity/spactivityrecord.h"
#include "spactivity/spactivitymgr.h"
#include "component/XSkillComponent.h"
#include "define/systemiddef.h"
#include "gamelogic/worldlevelmgr.h"
#include "unit/pet.h"
#include "buff/buffrecord.h"
#include "guild/guildinherit.h"
#include "scene/scenebossrush.h"
#include "component/XMoveComponent.h"
#include "component/XBeHitComponent.h"
#include "state/IXStateTransform.hpp"
#include "component/XFreezeComponent.h"
#include "action/ptcg2c_syncstepnotify.h"
#include "component/XActionSender.h"
#include "gamelogic/qqviprecord.h"
#include "event/eventmgr.h"
#include "common/ptcg2c_antiaddictionremindntf.h"
#include "gamelogic/teamrecord.h"
#include "scene/scenehorserace.h"
#include "scene/scenecastle.h"
#include "util/gametime.h"
#include "scene/sceneherobattle.h"
#include "httptask/reportdata.h"
#include "httptask/httptaskmgr.h"
#include "gamelogic/atlas.h"
#include "scene/sceneleague.h"
#include "gamelogic/popwindows.h"
#include "pay/ptcg2m_payconsumentf.h"
#include "utility/tlogmgr.h"
#include "ai/aitreemgr.h"
#include "gamelogic/herobattlerecord.h"
#include "table/ChannelCloseSys.h"
#include "gamelogic/military_rank_record.h"
#include "lottery/ptcg2c_sharerandomgiftntf.h"
#include "gamelogic/LotteryMgr.h"
#include "gamelogic/rolemisc.h"
#include "gamelogic/ibshoprecord.h"
#include "script/scriptmgr.h"
#include "guild/guildcampparty.h"
#include "gamelogic/platformshare_record.h"
#include "gamelogic/effectconfig.h"
#include "scene/scenemobabattle.h"
#include "gamelogic/dragonhelpconfig.h"
#include "gamelogic/weekend4v4mgr.h"
#include "gamelogic/weekend4v4record.h"
#include "gamelogic/dragongroupmgr.h"
#include "gamelogic/dragongrouprecord.h"
#include "scene/bigmeleefight.h"
#include "gamelogic/XPatrol.h"
#include "scene/sceneinvfight.h"
#include "scene/scene.h"
#include "npcfeeling/npcfeelingrecord.h"
#include "gamelogic/competedragonrecord.h"
#include "npcfeeling/npcfeelingmgr.h"
#include "gamelogic/battlefieldrecord.h"
#include "gamelogic/goalaward.h"
#include "scene/battlefieldfight.h"
#include "gamelogic/battlefieldrecord.h"
#include "gamelogic/helldoordropmgr.h"
#include "gamelogic/payconsumemgr.h"
#include "gamelogic/antiaddictionmgr.h"
// generate by ProtoGen at date: 2015/6/1 14:41:05

RPC_SERVER_IMPLEMETION(RpcC2G_GMCommand, GMCmdArg, GMCmdRes)

typedef bool (*GMHandler)(Role *pRole, std::vector<std::string> &args, std::string &outputMessage);

static bool DebugAddExp(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = args[0] + " expValue";
		return false;
	}

	UINT32 exp = convert<UINT32>(args.at(1));

	BagGiveItemTransition transition(pRole);
	transition.SetReason(ItemFlow_GM, ItemFlow_GM_Give);
	transition.GiveItem(EXP, exp);
	transition.NotifyClient();
	outputMessage = "add exp ok";
	return true;
}
static bool DebugSetLevelSealType(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = args[0] + " LevelSealType";
		return false;
	}
	int count = convert<int>(args.at(1));
	PtcG2M_GmLevelSealInfo ptc;
	ptc.m_Data.set_type(UINT32(count));
	pRole->SendToMS(ptc);
	outputMessage = "set levelseal type ok";
	return true;
}
static bool DebugShowLevelSeal(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	const stServerLevelSealInfo& info = CLevelSealMgr::Instance()->GetLevelSealInfo();
	std::ostringstream oss;
	oss << "Type: " << info.m_uType << "\n StartTime: " << TimeUtil::GetDateTimeStr(info.m_uStartTime) << "\n"
		<< "EndTime: " << TimeUtil::GetDateTimeStr(info.m_uEndTime) << "\n";
	outputMessage = oss.str();
	return true;
}
static bool DebugSetLevelSealTime(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 1)
	{
		outputMessage = args[0] + " LevelSealTime";
		return false;
	}
	PtcG2M_GmLevelSealInfo ptc;
	ptc.m_Data.set_endtime(UINT32(time(NULL)));
	pRole->SendToMS(ptc);
	outputMessage = "set levelseal time ok";
	return true;
}
static bool DebugSetLevelSealBossCount(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = args[0] + " LevelSealBossCount";
		return false;
	}
	int count = convert<int>(args.at(1));
	outputMessage = "set levelseal boss count ok";
	return true;
}

static bool DebugSetLevelSealCollectCount(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = args[0] + " LevelSealCollectCount";
		return false;
	}
	int count = convert<int>(args.at(1));
	CLevelSealMgr::Instance()->SetCollectCount(UINT32(count));
	outputMessage = "set levelseal collect count ok";
	return true;
}
static bool DebugSetPayV2(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 4)
	{
		outputMessage = args[0] + " paramid price days";
		return false;
	}
	
	std::string szParamID = args.at(1);
	INT32 nAmount = convert<INT32>(args.at(2));
	INT32 nDays = convert<INT32>(args.at(3));
	KKSG::PaytssInfo oData;
	oData.set_begintime(INT32(GameTime::GetTime()));
	oData.set_endtime(INT32(GameTime::GetTime() + nDays * XCommon::GetOneDayTime()));
	oData.set_lastgetawardtime(0);
	INT32 nResult = PayV2Mgr::Instance()->Pay(pRole, szParamID, oData);
	if (nResult == KKSG::ERR_SUCCESS)
	{
		outputMessage = "set pay ok";
	}
	else
	{
		outputMessage = "set pay fail";
	}
	return true;
}
static bool DebugSetPayCardTime(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 3)
	{
		outputMessage = args[0] + " paycard 1 20160824";
		return false;
	}
	INT32 nType = convert<INT32>(args.at(1));
	if (nType != KKSG::WEEK_CARD && nType != KKSG::MONTH_CARD)
	{
		outputMessage = "invalid card type(week:1 month:2)";
		return false;
	}
	std::string szTime = args.at(2);
	if (szTime.length() != 8)
	{
		outputMessage = "error time format.";
		return false;
	}
	struct tm stTime;

	memset(&stTime, 0, sizeof(stTime));
	stTime.tm_isdst = -1;
	stTime.tm_year = convert<INT32>(szTime.substr(0,4)) - 1900;
	stTime.tm_mon = convert<INT32>(szTime.substr(4,2)) - 1;
	stTime.tm_mday = convert<INT32>(szTime.substr(6,2));
	stTime.tm_hour = 12;
	time_t tTime = mktime(&stTime);
	if ((time_t)(-1) == tTime || tTime <  INT32(GameTime::GetTime()))
	{
		outputMessage = "invalid time.";
		return false;
	}
	outputMessage = "set paycard ok";
	PayV2Mgr::Instance()->SetPayCard(pRole, nType, (INT32)tTime);
	return true;
}

static bool DebugSetPayMemExpireTime(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 3)
	{
		outputMessage = args[0] + "paymemberexpire ID time";
		return false;
	}
	INT32 iID = convert<INT32>(args.at(1));
	INT32 iTime = convert<INT32>(args.at(2));
	if (PayV2Mgr::Instance()->SetPayMemberExpireTime(pRole, iID, iTime))
	{
		outputMessage = "set ok";
	}
	else
	{
		outputMessage = "set fail";
	}
	return true;
}

static bool DebugAddScore(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() == 2)
	{
		INT32 score = convert<INT32>(args.at(1));
		pRole->Get<CPayV2Record>()->AddScore(score);
	}
	outputMessage = "set ok";
	return true;
}

static bool DebugSetQQVip(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 3)
	{
		outputMessage = args[0] + "qqvip type day";
		return false;
	}
	INT32 type = convert<INT32>(args.at(1));
	INT32 days = convert<INT32>(args.at(2));
	UINT32 uStart = UINT32(time(NULL));
	UINT32 uEnd = uStart + UINT32(days * XCommon::GetOneDayTime());
	if (type == 1)
	{
		pRole->Get<QQVipRecord>()->SetVipTime(uStart, uEnd);
		pRole->Get<QQVipRecord>()->SetIsVip(true);

	}
	else if (type == 2)
	{
		pRole->Get<QQVipRecord>()->SetSVipTime(uStart, uEnd);
		pRole->Get<QQVipRecord>()->SetIsSvip(true);
	}
	else 
	{
		outputMessage = "set fail";
		return false;
	}
	outputMessage = "set ok";
	return true;
}

static bool DebugPayMemberDragonFlowerTime(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 3)
	{
		outputMessage = args[0] + " paymemberdragon 1 20161102";
		return false;
	}
	INT32 iID = convert<INT32>(args.at(1));
	std::string szTime = args.at(2);
	if (szTime.length() != 8)
	{
		outputMessage = "error time format.";
		return false;
	}
	struct tm stTime;
	memset(&stTime, 0, sizeof(stTime));
	stTime.tm_isdst = -1;
	stTime.tm_year = convert<INT32>(szTime.substr(0,4)) - 1900;
	stTime.tm_mon = convert<INT32>(szTime.substr(4,2)) - 1;
	stTime.tm_mday = convert<INT32>(szTime.substr(6,2));
	stTime.tm_hour = 12;
	time_t tTime = mktime(&stTime);
	if ((time_t)(-1) == tTime || tTime > time(NULL))
	{
		outputMessage = "invalid time.";
		return false;
	}
	if (PayV2Mgr::Instance()->SetPayMemberDragonFlowerTime(pRole, iID, (INT32)tTime))
	{
		outputMessage = "set ok";
	}
	else
	{
		outputMessage = "set fail";
	}
	return true;
}

static bool DebugPayVip(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = args[0] + " payvip";
		return false;
	}
	INT32 nAmount = convert<INT32>(args.at(1));
	PayV2Mgr::Instance()->CalcVipLevel(pRole, nAmount, KKSG::PAY_PARAM_NONE);
	outputMessage = "pay vip ok";
	return true;
}

static bool DebugSetAdult(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = args[0] + " setadult";
		return false;
	}
	INT32 type = convert<INT32>(args.at(1));
	pRole->Get<CIdipRecord>()->SetAdultType(type);
	outputMessage = "set ok";
	return true;
}

static bool DebugPayTotal(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() == 2)
	{
		UINT32 amt = convert<INT32>(args.at(1));
		pRole->Get<CPayV2Record>()->SetGMTotalAmt(amt);
	}
	outputMessage = "totalamt=" + ToString(pRole->Get<CPayV2Record>()->GetSaveAmt());
	return true;
}

static bool DebugGetTodayConsume(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	UINT32 consumenum = pRole->Get<CPayV2Record>()->GetTodayConsume();
	CPayConsumeMgr::Instance()->GetRebatInfo(pRole, outputMessage);
	return true;
}

static bool DebugAddDiamond(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = args[0] + " diamond count";
		return false;
	}
	INT32 nAmount = convert<INT32>(args.at(1));
	LogInfo("roleid=%llu add diamond=%d", pRole->GetID(), nAmount);
	if (nAmount > 100000)
	{
		nAmount = 100000;
	}
	if (nAmount && !pRole->AddDiamond(nAmount, ItemFlow_GM, ItemFlow_GM_Give))
	{
		outputMessage = "set fail";
	}
	outputMessage = "set ok";
	return true;
}

static bool DebugConsumeDiamond(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2 && args.size() != 3)
	{
		outputMessage = args[0] + " consume count";
		return false;
	}
	INT32 nAmount = convert<INT32>(args.at(1));
	std::string billno;
	if (args.size() == 3)
	{
		billno = args.at(2);
	}
	else
	{
		billno = TTag::Instance()->GetTagString();
	}

	PtcG2M_PayConsumeNtf ptc;
	ptc.m_Data.set_roleid(pRole->GetID());
	KKSG::Payconsumelist oInfo;
	oInfo.set_amt(nAmount);
	oInfo.set_reason(ItemFlow_GM);
	oInfo.set_subreason(ItemFlow_GM);
	oInfo.set_billno(billno);
	oInfo.set_ts(INT32(GameTime::GetTime()));
	ptc.m_Data.mutable_list()->CopyFrom(oInfo);
	MSLink::Instance()->SendTo(ptc);

	outputMessage = "set ok";
	return true;
}


static bool DebugSetIdipPunish(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 3)
	{
		outputMessage = args[0] + " idippunish type bantime";
		return false;
	}
	INT32 iType = convert<INT32>(args.at(1));
	INT32 iBanTime = convert<INT32>(args.at(2));
	KKSG::IdipPunishData oData;
	oData.set_type(iType);
	oData.set_bantime(iBanTime);
	oData.set_punishtime(INT32(time(NULL)));
	oData.set_reason("gm test");
	pRole->Get<CIdipRecord>()->SetIdipPunishData(oData);
	outputMessage = "set ok";
	return true;
}
static bool DebugAntiAddictionRemind(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = args[0] + " remind type";
		return false;
	}
	INT32 iType = convert<INT32>(args.at(1));
	PtcG2C_AntiAddictionRemindNtf ptc;
	ptc.m_Data.set_type(AntiAddictionReportType(iType));
	pRole->Send(ptc);
	outputMessage = "set ok";
	return true;
}

static bool DebugReportData(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	ReportDataTask* poTask = new ReportDataTask(pRole);
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
	return true;
}

static bool DebugUpdateUserInfo(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	std::vector<Role*> list;
	list.push_back(pRole);
	CAntiAddictionMgr::Instance()->UpdateUserInfo(list);
	return true;
}

static bool DebugClearSetPre(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	pRole->Get<CPayV2Record>()->ClearSetPre();
	return true;
}

static bool DebugSetLevel(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = args[0] + " newLevel";
		return false;
	}

	int level = convert<unsigned>(args.at(1));

	int MaxLevel = PlayerLevelManager::Instance()->MaxLevel();
	if (level > MaxLevel)
	{
		level = MaxLevel;
	}

	if (level > pRole->GetLevel())
	{
		RoleAttrChangeNotify rolechange(pRole);
		while (pRole->GetLevel() < level)
		{
			pRole->Levelup();
		}
		rolechange.Send();
	}
	else
	{
		pRole->DownLevel(level);
		pRole->GetNetHandler()->SendExpAndLevelNtf();
		pRole->RecalcuateAttributeAndNotify(RecalAttr_GMSetLevel);
	}

	pRole->SetFlag(RSF_ISRoleDataNeedSave, true);

	outputMessage = "set level ok";
	return true;
}

static bool DebugTask(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	std::string usage;
	usage =  "task take takeID\n";
	usage += "task finish [taskID]\n";
	usage += "task finishall\n";
	usage += "task reset\n";

	if (args.size() == 1)
	{
		outputMessage = usage;
		return false;
	}

	TaskRecord* task = pRole->Get<TaskRecord>();
	if (args[1] == "take")
	{
		if (args.size() == 3)
		{
			int TaskID = convert<int>(args[2]);
			task->GMTakeTask(TaskID);
			outputMessage = "take task";
		}
		else
		{
			outputMessage = usage;
		}
	}
	else if (args[1] == "finish")
	{
		if (args.size() == 3)
		{
			int TaskID = convert<int>(args[2]);
			task->GMFinishTask(TaskID);
			outputMessage = "finish task";
		}
		else
		{
			task->GMFinishDoingTask();
			outputMessage = "finish doing task";
		}
	}
	else if (args[1] == "reset")
	{
		task->GMResetTask();
		outputMessage = "reset task";
	}
	else if (args[1] == "finishall")
	{
		task->GMFinishAll();
		outputMessage = "complete all task";
	}
	else
	{
		outputMessage = usage;
	}
	return true;
}

static bool DebugGiveItem(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() < 3)
	{
		outputMessage = args[0] + " itemID itemCount";
		return false;
	}

	unsigned itemID = convert<unsigned>(args.at(1));
	INT64 itemCount = convert<INT64>(args.at(2));
	bool bind = true;
	if (args.size() == 4)
	{
		bind = convert<bool>(args.at(3));
	}
	if (!XItem::IsVirtualItem(itemID))
	{
		if (itemCount > 1000)
		{
			outputMessage = "itemCount is too many";
			return false;
		}
	}
	BagGiveItemTransition transition(pRole);
	transition.SetReason(ItemFlow_GM, ItemFlow_GM_Give);
	transition.GiveItem(itemID, (UINT32)itemCount, bind);
	transition.NotifyClient();
	return true;
}

static bool DebugGiveSprite(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() < 3)
	{
		outputMessage = args[0] + " spriteID count(less than 100)";
		return false;
	}

	UINT32 spriteID = convert<UINT32>(args.at(1));
	UINT32 count = convert<UINT32>(args.at(2));
	if(count > 100)
	{
		outputMessage = "Too many count";
		return false;
	}
	const SpriteConf* pSpriteConf = SpriteConfig::Instance()->GetSpriteConf(spriteID);
	if(NULL == pSpriteConf) 
	{
		outputMessage = "spriteid can't find in table";
		return false;
	}
	SpriteTransition oSpriteTransition(pRole);
	for(UINT32 i = 0; i < count; i++)
	{
		oSpriteTransition.GiveSprite(spriteID);
	}
	oSpriteTransition.NotifyClient();
	return true;
}

static bool DebugGiveEquip(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() != 4)
	{
		outputMessage = args[0] + " equipID count enhancelevel";
		return false;
	}
	unsigned itemID = convert<unsigned>(args.at(1));
	INT64 itemCount = convert<INT64>(args.at(2));
	INT64 enhance = convert<INT64>(args.at(3));
	ItemConf  *pConf = ItemConfig::Instance()->GetItemConf(itemID);
	if (NULL == pConf)
	{
		outputMessage = "item can not find";
		return false;
	}
	if(pConf->ItemType != EQUIP && pConf->ItemType != EQUIP_PROTO) 
	{
		outputMessage = "item is not equip";
		return false;
	}
	///> 装备原型，把装备ID转换成对应职业的装备
	if (pConf->ItemType == EQUIP_PROTO)
	{
		UINT32 profItemID = EquipProtoCalc::ConvertToProfessionItemID(itemID, pRole->GetProfession());

		pConf = ItemConfig::Instance()->GetItemConf(profItemID);
		if (pConf == NULL)
		{
			return false;
		}

		itemID = profItemID;
	}
	UINT64 uid = UIDHelper::CreateUID(UID_Item);
	XItem item(uid, itemID, (int)itemCount, EQUIP);
	if(enhance > 0)
	{
		/*EnhanceConf* conf = EnhanceEquipMgr::Instance()->GetConf(pItem->itemID, (UINT32)enhance);
		if (NULL != conf)
		{
			pItem->pEnhanceInfo = new EnhanceInfo;
			pItem->pEnhanceInfo->dwCount = 0;
			pItem->pEnhanceInfo->dwLevel = (UINT32)enhance;
			pItem->pEnhanceInfo->vecAttr = conf->attrs;
		}*/
	}
	BagGiveItemTransition transition(pRole);
	transition.SetReason(ItemFlow_GM, ItemFlow_GM_Give);
	transition.GiveItem(&item);
	transition.NotifyClient();
	return true;
}

static bool DebugTakeVirtualItem(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() < 3)
	{
		outputMessage = args[0] + " itemID itemCount";
		return false;
	}
	Bag* pBag = pRole->Get<Bag>();
	if (NULL == pBag)
	{
		outputMessage = args[0] + " bag null";
		return false;
	}
	UINT32 itemID = convert<UINT32>(args.at(1));
	UINT32 itemCount = convert<UINT32>(args.at(2));
	INT64 nCount = 0;
	if(itemID < VIRTUAL_ITEM_MAX) 
	{
		nCount = pBag->CountVirtualItem(itemID);
	}
	else
	{
		nCount = pBag->CountItemInBag(itemID);
	}
	if ((INT64)itemCount > nCount)
	{
		itemCount = (UINT32)nCount;
	}
	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_GM, ItemFlow_GM_Clear);
	if(!take.TakeItem(itemID, itemCount))
	{
		take.RollBack();
		return false;
	}
	take.NotifyClient();

	outputMessage = "takeitem ok";
	return true;
}
static bool DebugOpenSystem(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	bool isopen = true;
	if (args.size() < 2)
	{

		outputMessage = args[0] + " sysID";
		return false;
	}
	if (args.size() == 3)
	{
		isopen = convert<bool>(args.at(2));
	}
	int nSystemId = convert<int>(args.at(1));		
	if (!isopen)
	{
		pRole->Get<CRoleSystem>()->SetSystemOpened(nSystemId, false);
		return true;
	}
	std::vector<int> vecSystemId;
	vecSystemId.push_back(nSystemId);
	if (!OpenSystemHandler::GlobaHandler.DoOpenSystem(pRole, vecSystemId))
	{
		return false;
	}
	CActivityRecord* pRecord = pRole->Get<CActivityRecord>();	
	CActivityMgr::Instance()->UpdateActiviyId(pRecord, pRole);

	return true;
}

static bool DebugOpenAllSystem(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 1)
	{
		return false;
	}
	
	///>  触发开启系统事件
	pRole->Get<LotteryRecord>()->OnSystemOpened(2);

	pRole->Get<CRoleSystem>()->SetAllSystemOpened();
	pRole->SetAllTuturial();

	CActivityRecord* pRecord = pRole->Get<CActivityRecord>();
	CActivityMgr::Instance()->UpdateActiviyId(pRecord, pRole);

	outputMessage = "openallsystem ok";
	
	return true;
}

static bool DebugTestGuildBoss(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	GuildBossMgr::Instance()->ClearAfterEnd(); // 清理
	return true;
}

static bool DebugReloadAllLuaFile(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	CScriptMgr::Instance()->ReloadAllScripts();
	return true;
}

static bool DebugRunLuaFile(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = args[0] + " luaFile";
		return false;
	}

	CScriptMgr::Instance()->ExecuteFile(args[1].c_str());
	return true;
}

static bool DebugAddArenaCount(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = args[0] + " arenaCount";
		return false;
	}

	UINT32 dwArenaCount = convert<UINT32>(args.at(1));			
	SceneInfo* sceneinfo = SceneConfig::Instance()->FindSceneInfo( GetGlobalConfig().ArenaSceneID);
	if (NULL == sceneinfo)
	{
		return false;
	}
	StageMgr* stage = pRole->Get<StageMgr>();
	if (NULL == stage)
	{
		return false;
	}
	stage->Update();

	stage->AddSceneBuyCount(sceneinfo, dwArenaCount);

	outputMessage = "add arenacount ok";

	return true;
}

static bool DebugServerInfo(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	outputMessage = GSConfig::Instance()->GetServerInfo();
	return true;
}

static bool DebugAddBuff(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 3 && args.size() != 4)
	{
		outputMessage = "addbuff buffID buffValue [target id]";
		return true;
	}

	Unit* pUnit = pRole;

	XAddBuffActionArgs e;
	e.singleBuff.buffID = convert<int>(args[1]);
	e.singleBuff.BuffLevel = convert<int>(args[2]);
	e.Caster = pRole->GetID();

	if(args.size() >= 4)
	{
		UINT64 uid = 0;
		if(args[3][0] == 't')
		{
			float time = convert<float>(args[3].substr(1));
			e.singleBuff.effecttime = UINT32(time * 1000);

			if(args.size() >= 5)
			{
				uid = convert<UINT64>(args[4]);
			}
		}
		else
		{
			uid = convert<UINT64>(args[3]);
		}
		
		if(uid != 0)
		{
			if (Unit::GetUnitType(uid) == KKSG::Category_Role)
			{
				pUnit = RoleManager::Instance()->FindByRoleID(uid);
			}
			else
			{
				pUnit = EnemyManager::Instance()->FindEnemy(uid);
			}
		}
	}
	else
	{
		pUnit = pRole;
	}
	if(pUnit == NULL)
		return true;

	pUnit->GetXObject()->DispatchAction(&e);
	outputMessage = "add buff ok!";

	return true;
}

static void ListBuffCB(void *arg, const XBuff* pBuff)
{
	std::stringstream &ss = *(std::stringstream *)arg;
	ss << '[' << pBuff->GetID() << ":" << pBuff->GetLevel() << "]\n";
}

static bool DebugListBuff(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	std::stringstream ss;
	ss << "buff list:\n";
	pRole->GetXRole()->BuffComponent()->IterateBuff(ListBuffCB, &ss);
	outputMessage = ss.str();
	return true;
}

static bool DebugListAttr(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() < 2)
	{
		outputMessage = "listattr uid [attr ids]";
		return true;
	}

	UINT64 uid = convert<UINT64>(args[1]);
	Unit* pUnit = uid == 0 ? pRole : Unit::FindUnit(uid);
	if (pUnit == NULL)
	{
		outputMessage = "Cant find unit.";
		return true;
	}

	std::stringstream ss;
	ss << "attr list:\n";

	ss.setf(ios::fixed);
	ss.precision(2);
	std::vector<int> attrlist;
	for (int i = 2; i < args.size(); ++i)
	{
		attrlist.push_back(convert<int>(args[i]));
	}
	if (attrlist.size() == 0)
	{
		for (int i = 0; i < AttributeDefInfo::UsedAttributeCount; ++i)
		{
			int a = AttributeDefInfo::UsedAttributeArray[i];
			attrlist.push_back(a);
			attrlist.push_back(CA_PERCENT_START + a);
			attrlist.push_back(CA_TOTAL_START + a);
		}
	}

	for (auto it = attrlist.begin(); it != attrlist.end(); ++it)
	{
		double v = pUnit->GetAttr((CombatAttrDef)(*it));
		if ( v > -0.0001 && v < 0.0001)
			continue;

		ss << "[" << *it << ": " << v << "]\n";
	}

	outputMessage = ss.str();
	return true;
}

static bool DebugArenaRank(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = args[0] + " pos";
		return false;
	}
	UINT32 dwPos = convert<UINT32>(args.at(1));			
	if (dwPos == 0)
	{
		outputMessage = "arena rank failed";
		return false;
	}

	pRole->Get<CArenaRecord>()->SetOptimalRank(dwPos);

	RpcG2M_EndAreanMs* rpc = RpcG2M_EndAreanMs::CreateRpc();
	rpc->m_oArg.set_isgm(true);
	rpc->m_oArg.set_roleid(pRole->GetID());
	rpc->m_oArg.set_rank(dwPos);
	pRole->SendToMS(*rpc);

	outputMessage = "arena rank ok";
	return true;
}

static bool DebugArena(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = args[0] + " pos";
		return false;
	}

	//UINT32 dwPos = convert<UINT32>(args.at(1));			
	//UINT64 qwChallengedId = CArenaMgr::Instance()->GetRoleId(dwPos);

	//UINT32 dwRank1 = CArenaMgr::Instance()->GetRank(qwChallengedId);
	//UINT32 dwRank2 = CArenaMgr::Instance()->GetRank(pRole->GetID());

	//CArenaRecord* record = pRole->Get<CArenaRecord>();	
	//record->SetOptimalRank(dwRank1);

	//CArenaRealTimeRankList* ranklist = (CArenaRealTimeRankList*)(CRankListMgr::Instance()->GetRankList(KKSG::RealTimeArenaRank));
	//ranklist->SwapPos(qwChallengedId, dwRank1, pRole->GetID(), dwRank2);

	outputMessage = "arena rank ok";

	return true;
}

static bool DebugSetConfig(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	return true;
}

static bool DebugClearBag(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = args[0] + " itemtype";
		return false;
	}

	UINT32 dwType = convert<UINT32>(args.at(1));

	pRole->Get<Bag>()->ClearBag(pRole, dwType);

	outputMessage = "clearbag ok";

	return true;
}

static bool DebugAddActivityCount(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() < 2)
	{
		outputMessage = args[0] + " id";
		return false;
	}

	UINT32 dwId = convert<UINT32>(args.at(1));
	bool random = false;
	if (args.size() == 3)
	{
		random = true;
	}
	CActivityRecord* pRecord = pRole->Get<CActivityRecord>();
	if (NULL == pRecord)
	{
		return false;
	}

	CActivityMgr::Instance()->OnFinishActivityId(pRecord, pRole, dwId,random);
	
	outputMessage = "add activity count  ok";

	return true;
}


static bool DebugToSkyWait(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 3)
	{
		outputMessage = args[0] + "floor type";
		return false;
	}

	UINT32 dwFloor = convert<UINT32>(args.at(1));
	if (dwFloor > 7 )
	{
		outputMessage = args[0] + "floor max 7";
	}
	if (dwFloor != 0)
	{
		UINT32 dwSceneId = 30+dwFloor;
		SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(dwSceneId);
		if (pSceneInfo == NULL)
		{
			outputMessage = "enter scene failed";
			return false;
		}

		if (NULL != pRole->GetLoadingScene() && dwSceneId == pRole->GetLoadingScene()->GetSceneTemplateID())
		{
			return false;
		}

		bool bRet = SceneManager::Instance()->ChangeScene(pRole, dwSceneId, pRole->GetID());
	}

	const std::vector<float>& confpos = GetGlobalConfig().SkyCityRestPos;
	if(pRole && 6 == confpos.size())
	{
		EnterPosition  pos;
		pos.face = pRole->GetXObject()->GetFaceDegree();
		if(args[2] == "1")
		{
			pos.position.x = confpos[0] + (float)(XRandom::randDouble(-0.5, 0.5));
			pos.position.y = confpos[1];
			pos.position.z = confpos[2] + (float)(XRandom::randDouble(-0.5, 0.5));
		}
		else
		{
			pos.position.x = confpos[3] + (float)(XRandom::randDouble(-0.5, 0.5));
			pos.position.y = confpos[4];
			pos.position.z = confpos[5] + (float)(XRandom::randDouble(-0.5, 0.5));
		}
		XEntity* entity = dynamic_cast<XEntity*>(pRole->GetXObject());
		if(entity)
		{
			entity->TransferLocation(pos.position, pos.face);
		}
	}
	return true;
	
}

static bool DebugECS(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
    Scene* pScene = pRole->GetCurrScene();
    if(NULL == pScene)
    {
        return false;
    }
    KKSG::SceneSwitchData data;
    CSceneSwitch::EnterScene(pRole, pScene->GetSceneTemplateID(), pScene->GetSceneID(), data, GSConfig::Instance()->IsCrossGS(), GSConfig::Instance()->GetLine());
    return true;
}

static bool DebugEnterScene(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2 && args.size() != 3)
	{
		outputMessage = args[0] + " sceneid";
		return false;
	}

	UINT32 dwSceneId = convert<UINT32>(args.at(1));
	SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(dwSceneId);
	if (pSceneInfo == NULL)
	{
		outputMessage = "enter scene failed";
		return false;
	}

	if (NULL != pRole->GetLoadingScene() && dwSceneId == pRole->GetLoadingScene()->GetSceneTemplateID())
	{
		//if (pRole->DoEnterNtf(pRole->GetLoadingScene(), dwSceneId))
		//{
		//	return true;
		//}
		//else
		//{
		//	return false;
		//}
		return false;
	}

	int savedMode = pSceneInfo->m_pConf->syncMode;

	if (args.size() == 3 && args[2] == "1")
	{
		pSceneInfo->m_pConf->syncMode = SERVERMODE;
	}

	bool bRet = SceneManager::Instance()->ChangeScene(pRole, dwSceneId, pRole->GetID());

	pSceneInfo->m_pConf->syncMode = savedMode;

	if (!bRet)
	{
		outputMessage = "enter scene failed";
		return false;
	}

	outputMessage = "enter scene succeed";

	//outputMessage = "enter scene not support now";
	return true;
}

static bool DebugGMEnterScene(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = args[0] + "uniqueid";
		return false;
	}

	UINT32 dwSceneId = convert<UINT32>(args.at(1));
	Scene* scene = SceneManager::Instance()->FindBySceneID(dwSceneId);
	if (scene == NULL)
	{
		outputMessage = "enter scene failed";
		return false;
	}
	SceneManager::Instance()->EnterScene(pRole, scene);

	outputMessage = "enter scene succeed";

	return true;
}

static bool DebugFollow(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = args[0] + "rolename";
		return false;
	}
	Role* role = RoleManager::Instance()->FindByName(args[1]);	
	if (role == NULL)
	{
		outputMessage = "enter scene failed";
		return false;
	}
	Scene* scene = role->GetCurrScene(); 
	if (scene == NULL)
	{
		outputMessage = "enter scene failed";
		return false;
	}
	SceneManager::Instance()->EnterScene(pRole, scene);

	outputMessage = "enter scene succeed";

	return true;
}

static bool DebugMobDummyRole(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(NULL == pRole->GetCurrScene())
	{
		return false;
	}
	UINT64 allyID = pRole->GetID();
	RoleSummary *pSummary = NULL;
	if (args.size() > 1)
	{
		allyID = convert<UINT64>(args.at(1));
		pSummary = RoleSummaryMgr::Instance()->GetRoleSummary(allyID);
		if (pSummary == NULL)
		{
			outputMessage = "not found role summary id";
		}
		return false;
	}

	if (pSummary == NULL)
	{
		pSummary = RoleSummaryMgr::Instance()->GetRoleSummary(pRole->GetID());
	}
	EnterSceneInfo entersceneinfo = SceneConfig::Instance()->GetEnterSceneInfo(KKSG::ENTER_SCENE_SWITCH, pRole->GetCurrScene()->GetSceneTemplateID());
	pSummary->qwRobotID = DummyRoleManager::Instance()->CreateNewID();
	DummyRole *pAlly = DummyRoleManager::Instance()->Create(pSummary, pRole->GetXRole()->GetPosition(), pRole->GetXRole()->GetFaceDegree(), (UINT32)(KKSG::FightRole), entersceneinfo);
	pRole->GetCurrScene()->AddUnitAndNotify(pAlly);

	outputMessage = "mob dummyrole succeed";
	return true;
}

static bool DebugMobMonster(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2 && args.size() != 3)
	{
		outputMessage = args[0] + " monster id";
		return false;
	}

    if (NULL == pRole->GetCurrScene())
    {
        outputMessage = "currscene = NULL";
        return false;
    }

    if (pRole->GetCurrScene()->SyncMode() != SERVERMODE)
    {
        outputMessage = "only server mode can do";
        return false;
    }

	UINT32 dwMonsterId = convert<UINT32>(args.at(1));
	Vector3 pos = pRole->GetXRole()->GetPosition();
	pos.x -= 3;
	Enemy* pMonster = NULL;
	if (args.size() == 3)
	{
		pMonster = EnemyManager::Instance()->CreateEnemyByCaller(pRole, dwMonsterId, pos, 0.0f, false);
	}
	else
	{
		pMonster = EnemyManager::Instance()->CreateEnemy(dwMonsterId, pos, 0.0f);
		if (NULL != pMonster)
		{
			pRole->GetCurrScene()->AddUnitAndNotify(pMonster);
		}
	}
	if(!pMonster)
	{
		outputMessage = "mob monster failed";
		return false;
	}

	outputMessage = "mob monster succeed";
	return true;
}

static bool DebugAICommand(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = args[0] + " command";
		return false;
	}

	if (args[1] == "rongyankill")
	{
		UINT32 skillid = 4160243969;
		Scene* curScene = pRole->GetCurrScene();
		XAIEventArgs args;
		args.MsgTargetTemplateId = 0;
		args.EventArg = "Dead";
		args.Templateid = pRole->GetTemplateID();
		args.Pos = pRole->GetXObject()->GetPosition_p();
		args.SkillId = skillid;
		args.EnemyTemplateId = 0;

		curScene->MsgChannel().SendMsgTo(AISceneMsgChannel::GlobalAI, AISceneMsgChannel::Cmd, args);
		return true;
	}
	return true; 
}

static bool DebugGiveReward(Role* pRole, std::vector<std::string>& args, std::string& outputMessage)
{
	if (args.size() != 4)
	{
		outputMessage = args[0] + " roleid itemid itemcount";
		return false;
	}
	//UINT64 qwRoleId = convert<UINT64>(args.at(1));
	//int nItemId = convert<int>(args.at(2));
	//int nItemCount = convert<int>(args.at(3));

	//std::vector<ItemDesc> vecItem;
	//vecItem.push_back(ItemDesc(nItemId, nItemCount));

	//std::vector<std::string> vecParam;

	//CRewardMgr::Instance()->PushOnceReward(qwRoleId, KKSG::RewardGM, vecParam, vecItem);
	
	return true;	
}

static bool DebugFetchReward(Role* pRole, std::vector<std::string>& args, std::string& outputMessage)
{
	if (args.size() != 3)
	{
		outputMessage = args[0] + " roleid uniqueid";
		return false;
	}
	//UINT64 qwRoleId = convert<UINT64>(args.at(1));
	//UINT64 qwU= convert<UINT64>(args.at(2));

	//CRewardMgr::Instance()->RemoveFromDb(qwRoleId, qwUniqueId);

	return true;	
}

static bool DebugNotice(Role* role, std::vector<std::string>& args, std::string& outputMessage)
{
	if (args.size() != 4)
	{
		outputMessage = args[0] + " channel message priority";
		return false;
	}
	UINT32 channel = convert<UINT32>(args.at(1));
	std::string message = convert<std::string>(args.at(2));
	UINT32 priority = convert<UINT32>(args.at(3));

	NoticeMgr::Instance()->RawNotice(channel, message, priority);

	return true;
}

static bool DebugAddFriend(Role* role, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2)
	{
		output = args[0] + " friendroleid";
		return false;
	}
	
	//UINT64 friendroleid = convert<UINT64>(args.at(1));
	//FriendOp op(role->GetID());
	//op.SetOther(friendroleid);
	//op.AddFriend();

	return true;
}

static bool DebugAddGuildBonus(Role* role, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 6)
	{
		output = args[0] + " bonusType maxPeopleNum bonusNum templateID content";
		return false;
	}

	UINT32 bonusType = convert<UINT32>(args.at(1));
	UINT32 maxPeopleNum = convert<UINT32>(args.at(2));
	UINT32 bonusNum = convert<UINT32>(args.at(3));
	UINT32 templateID = convert<UINT32>(args.at(4));
	std::string content = args.at(5);
	
	#if GUILD_GS
	UINT64 guildID = GuildMgr::Instance()->GetRoleGuild(role->GetID());
	Guild* pGuild = GuildMgr::Instance()->FindGuild(guildID);
	if (!pGuild)
	{
		return false;
	}
	pGuild->Get<GuildBonusMgr>()->addGuildBonus(role, "系统红包", bonusType, maxPeopleNum, bonusNum, templateID, content);
#endif
	return true;
}

void SetScene3Star(UINT32 sceneID, SceneInfo *pSceneInfo, void *arg)
{
	Role* role = (Role *)arg;
	/*if (pSceneInfo->m_pConf->type == KKSG::SCENE_BATTLE ||
		pSceneInfo->m_pConf->type == KKSG::SCENE_ABYSSS)*/
	{
		role->Get<StageMgr>()->SetRank(pSceneInfo->m_pConf->id, RANK_SSS);
	}
}

static bool DebugSceneStar(Role* role, std::vector<std::string>& args, std::string& output)
{
	SceneConfig::Instance()->TravelAllSceneInfo(SetScene3Star, role);
	return true;
}

static bool DebugAddGuildExp(Role* role, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2)
	{
		output = args[0] + " exp";
		return false;
	}

	UINT32 exp = convert<UINT32>(args.at(1));
	/*Guild* guild = GuildMgr::Instance()->FindGuildByRoleId(role->GetID());
	if (NULL != guild)
	{
		guild->AddExp(exp);
	}*/

	return true;
}

static bool DebugAddAttribute(Role* role, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 3)
	{
		output = args[0] + " attrID attrValue";
		return false;
	}

	UINT32 attrID = convert<UINT32>(args.at(1));
	UINT32 attrValue = convert<UINT32>(args.at(2));
	AttributeWatcher watcher(role);
	watcher.AddAttr((CombatAttrDef)attrID, attrValue);
	watcher.CheckChangedAttributeAndNotify(false);
	return true;
}

static bool DebugClearSceneCD(Role* role, std::vector<std::string>& args, std::string& output)
{
	role->Get<StageMgr>()->ClearSceneCoolDown();
	return true;
}

static bool DebugChangeScene(Role* role, std::vector<std::string>& args, std::string& output)
{
	/*if (args.size() == 4)
	{
		UINT32 sceneid = convert<UINT32>(args[1]);
		SceneInfo* info = SceneConfig::Instance()->FindSceneInfo(sceneid);
		if (NULL == info)
		{
			return false;
		}
		Scene* scene = SceneManager::CreateScene(info);
		if (NULL == scene)
		{
			return false;
		}
		std::vector<Unit*> rolea;
		std::vector<Unit*> roleb;
		Role* role1 = RoleManager::Instance()->FindByRoleID(convert<UINT64>(args[2]));
		Role* role2 = RoleManager::Instance()->FindByRoleID(convert<UINT64>(args[3]));
		if (NULL != role1 && NULL != role2)
		{
			rolea.push_back(role1);
			SceneManager::Instance()->ChangeScene(role1, scene);
			roleb.push_back(role2);
			SceneManager::Instance()->ChangeScene(role2, scene);
			if (NULL != scene->GetPkHandler())
			{
				scene->GetPkHandler()->Init(rolea, roleb);
			}
		}
	}*/

	return true;
}

static bool DebugTestMul(Role* pRole, std::vector<std::string>& args, std::string& output)
{
	MultActivityMgr::Instance()->GmTestCalcWeek();
	return true;
}

static bool DebugTest(Role* pRole, std::vector<std::string>& args, std::string& output)
{
	int size = (int)(args.size());
	if(size >= 2)
	{
		if(args[1] == "kb")
		{
			if(size >= 3)
			{
				pRole->Get<RoleMisc>()->AddKingBackRewardCount();
			}
			output = " kingback reward count = " + ToString(pRole->Get<RoleMisc>()->GetKingBackRewardCount());
		}
	}
	else
	{
	}
	return true;
}

static bool DebugSurvive(Role* pRole, std::vector<std::string>& args, std::string& output)
{
	int size = (int)(args.size());
	if(size >= 2)
	{
		if(args[1] == "useitem" && size >= 3)
		{
			UINT32 index = convert<UINT32>(args[2]);
			pRole->Get<BuffRecord>()->UseDoodItem(index);
		}
	}
	else
	{
		pRole->Get<BuffRecord>()->DoodItemInfoPrint();
		pRole->Get<RoleMisc>()->SurviveInfoPrint();	
	}
	return true;
}

static bool DebugNpcFeeling(Role* pRole, std::vector<std::string>& args, std::string& output)
{
	auto pRecord = pRole->Get<CNpcFeelingRecord>();
	pRecord->Update();
	const int size = int(args.size());
	if(size >= 2)
	{
		if(args[1] == "setc")
		{
			pRole->Get<CNpcFeelingRecord>()->SetChanged();
		}
		else if(args[1] == "dayzero")
		{
			UINT32 diff = 1;
			if(size >= 3)
			{
				diff = convert<UINT32>(args[2]);
			}
			pRole->Get<CNpcFeelingRecord>()->DayZero(diff);
		}
		else if(args[1] == "clearmy")
		{
			pRole->Get<CNpcFeelingRecord>()->Clear();
		}
		else if(args[1] == "resetnpc" && size >= 3)
		{
			UINT32 npcID = convert<UINT32>(args[2]);
			pRole->Get<CNpcFeelingRecord>()->SetNpcExpLevel(npcID, 0, 1);
		}
		else if(args[1] == "npcexp" && size >= 4)
		{
			UINT32 npcID = convert<UINT32>(args[2]);
			int exp = convert<UINT32>(args[3]);
			auto pData = pRecord->GetOneNpc(npcID);
			if(pData)
			{
				int newExp = int(pData->exp()) + exp;
				if(newExp < 0) newExp = 0;
				pRecord->SetNpcExpLevel(npcID, UINT32(newExp), pData->level());
			}
		}
		else if(args[1] == "resetcount")
		{
			pRecord->ResetBaseCount();
		}
	}
	else
	{
		CNpcFeelingMgr::Instance()->InfoPrint(pRole);
		//pRole->Get<CNpcFeelingRecord>()->InfoPrint();
	}
	return true;
}

static bool DebugCompeteDragon(Role* pRole, std::vector<std::string>& args, std::string& output)
{
	const int size = int(args.size());
	if(size >= 2)
	{
		if(args[1] == "reset")
		{
			pRole->Get<CCompeteDragonRecord>()->DebugReset();
		}
		else if(args[1] == "setpasscount")
		{
			if (size >= 3)
			{
				UINT32 count = convert<UINT32>(args[2]);
				pRole->Get<CCompeteDragonRecord>()->DebugSetThrough(count);
			}
			else
			{
				output = "format error, competedragon setpasscount 1";
			}
		}
		else
		{
			output = "format error, competedragon [reset|setpascount]";
		}
	}
	else
	{
		output = "format error, competedragon [reset|setpascount]";
	}
	return true;
}

static bool DebugInvfight(Role* pRole, std::vector<std::string>& args, std::string& output)
{
	if(pRole && pRole->GetCurrScene() && pRole->GetCurrScene()->GetInvFightHandler())
	{
		pRole->GetCurrScene()->GetInvFightHandler()->RoleReqFightAgain(pRole);
	}
	return true;
}

static bool DebugPk(Role* role, std::vector<std::string>& args, std::string& output)
{
	int size = (int)(args.size());
	if(size >= 2)
	{
		const std::string& op = args[1];
		if(op == "print" && size >= 3)
		{
			int type = convert<int>(args[2]);
			if(KKSG::PkNVNType_IsValid(type))
			{
				role->Get<CPkRecord>()->NVNInfoPrint(KKSG::PkNVNType(type));
			}
		}
		else if(op == "set" && size >= 4)
		{
			KKSG::PkNVNType type = KKSG::PkNVNType(convert<int>(args[2]));
			int point = convert<int>(args[3]);
			if(KKSG::PkNVNType_IsValid(type))
			{
				auto pRecord = role->Get<CPkRecord>();
				int old = pRecord->GetPoint(type);
				pRecord->AddPoint(type, point - old);
				output = ToString(pRecord->GetPoint(type));
			}
		}
		else if(op == "wreset")
		{
			role->Get<CPkRecord>()->WeekReset();
		}
		else if(op == "lastwp")
		{
			auto pRecord = role->Get<CPkRecord>();
			output = ToString(pRecord->GetLastWeekPoint());
			SSDebug << " lwp = " << output << END;
		}
		else if(op == "reset")
		{
			role->Get<CPkRecord>()->TestReset();
		}
	}
	return true;
}

static bool DebugAddPkPoint(Role* role, std::vector<std::string>& args, std::string& output)
{
	CPkRecord* record = role->Get<CPkRecord>();
	record->Update();
	if (args.size() == 2)
	{
		int point = convert<int>(args[1]);
		if (point > 0)
		{
			record->AddPoint(role->GetID(), point);
			DesignationMgr::Instance()->OnSkyLadderScore(role, record->GetPoint());
			DesignationMgr::Instance()->OnSkyLadderWinTimes(role,record->GetWin());
			AchieveV2Mgr::Instance()->OnPKWinTimes(role,record->GetWin());
			AchieveV2Mgr::Instance()->OnPKScore(role,record->GetPoint());
		}
		else
		{
			record->SubPoint(role->GetID(), -point);
		}
		output = "pkpint:" + ToString(record->GetPoint());
	}
	else if(args.size() == 3)
	{
		int point = convert<int>(args[1]);
		int type = convert<int>(args[2]);	
		if(KKSG::PkNVNType_IsValid(type))
		{
			record->AddPoint((KKSG::PkNVNType)(type), point);	
		}
	}
	else
	{
		output = "lastday continue win = " + ToString(record->GetContWinDay());
	}
	return false;
}

static bool DebugResetCount(Role* role, std::vector<std::string>& args, std::string& output)
{
	if (args.size() == 2)
	{
		int groupid = convert<int>(args[1]);
		StageMgr* record = role->Get<StageMgr>();
		record->Update();
		record->ResetPlayCount(groupid);
		output = "succeed";
	}
	return false;
}

static bool DebugResetTeamCount(Role* role, std::vector<std::string>& args, std::string& output)
{
	role->Get<CTeamRecord>()->DebugResetTeamCount();

	output = "succeed";

	return true;
}

static bool DebugPath(Role* role, std::vector<std::string>& args, std::string& output)
{
	int gz = 0, gx = 0;
	const Vector3 pos = role->GetXRole()->GetPosition_p();
	Grid *pGrid = role->GetCurrScene()->GetGrid();
	if (pGrid)
	{
		pGrid->GetCellGrid(pos.x, pos.z, gz, gx);
	}
	
	std::stringstream ss;
	if (args.size() == 1)
	{
		ss << "my position: " << pos.x << "," << pos.z << " at grid " << gx << "," << gz;
		output = ss.str();
	}
	else if (args.size() == 3)
	{
		int dgx = convert<int>(args[1]);
		int dgz = convert<int>(args[2]);
		Vector3 target;
		pGrid->GetSceneXY(target.x, target.z, dgz, dgx);
		std::vector<Vector3> waypoint;
		if (pGrid->FindPath(pos, target, 10000, waypoint))
		{
			ss << "find path " << waypoint.size() << " steps\n";
			for (UINT32 i = 0; i < waypoint.size(); ++i)
			{
				ss << i << "." << waypoint[i].x << "," << waypoint[i].z << "\n";
			}
			output = ss.str();
		}
		else
		{
			output = "not found way";
		}
	}

	return true;
}

static bool DebugGetAllFashion(Role* role, std::vector<std::string>& args, std::string& output)
{
	std::unordered_set<int> &fashionIDs = ItemConfig::Instance()->GetAllFashions();
	for (auto i = fashionIDs.begin(); i != fashionIDs.end(); ++i)
	{
		role->Get<RoleFashion>()->AddNewFashion(*i);
	}
	
	return true;
}

static bool DebugSubHp(Role* role, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 3)
	{
		return false;
	}
	UINT64 enemyid = convert<UINT64>(args[1]);
	double hurt = convert<double>(args[2]);

	Enemy* enemy = EnemyManager::Instance()->FindEnemy(enemyid);
	if (NULL != enemy)	
	{
		enemy->SubHp(role, hurt);
	}

	return true;
}

static bool DebugAddDegree(Role* role, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 3)
	{
		return false;
	}
	//RoleSummary* summary = RoleSummaryMgr::Instance()->FindRoleSummary(args[1]);
	//if (NULL == summary)
	//{
	//	output = "name not exist";
	//	return false;
	//}
	//UINT32 degree = convert<UINT32>(args[2]); 
	
	//FriendOp op(role->GetID());
	//op.SetOther(summary->qwRoleID);
	//if (UINT32(-1) == op.AddDegree(0, degree))
	//{
	//	output = "not friend";
	//	return false;
	//}
	//else
	//{
	//	FriendOp op(role->GetID());
	//	UINT32 totaldegree = op.GetDegree(summary->qwRoleID);
	//	output = "add succeed, total degree:" + ToString(totaldegree);
	//	return true;
	//}
	return true;
}

static bool DebugAddGaoJiGuExp(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	/*if (args.size() != 2)
	{
		outputMessage = args[0] + " expValue";
		return false;
	}

	int exp = convert<int>(args.at(1));
	GaoJiGu* info = SocialMgr::Instance()->GetGaoJiGuInfo(pRole->GetID());
	if (!info)
	{
		outputMessage = "gaojigu not open";
		return false;
	}
	if (SocialMgr::Instance()->IsSelfGaoJiGu(pRole))
	{
		info->AddExp(exp, pRole->GetCurrScene());
	}
	else
		info->AddExp(exp, NULL);
	SocialMgr::Instance()->DebugAddChangedRole(pRole->GetID());*/
	outputMessage = "add exp ok";
	return true;
}

static bool DebugAddFishExp(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = args[0] + " expValue";
		return false;
	}

	//int exp = convert<int>(args.at(1));
	//GaoJiGu* info = SocialMgr::Instance()->GetGaoJiGuInfo(pRole->GetID());
	//if (!info)
	//{
	//	outputMessage = "gaojigu not open";
	//	return false;
	//}

	//info->m_fishingInfo.AddExp(pRole, exp);
	//SocialMgr::Instance()->DebugAddChangedRole(pRole->GetID());

	outputMessage = "add exp ok";
	return true;
}

static bool DebugResetFishCount(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	//GaoJiGu* info = SocialMgr::Instance()->GetGaoJiGuInfo(pRole->GetID());
	//if (!info)
	//{
	//	outputMessage = "gaojigu not open";
	//	return false;
	//}

	//info->m_fishingInfo.ResetFishNum(pRole);

	//SocialMgr::Instance()->DebugAddChangedRole(pRole->GetID());

	outputMessage = "reset fishcount ok";
	return true;
}

static bool Trace(Role* role, std::vector<std::string>& args, std::string& output)
{
	if (args.size() == 2)
	{
		if (args[1] == "clear")
		{
			PositionDebugMgr::Instance()->TraceClear(role->GetID());
			output = "clear trace unit ok!";
			return true;
		}
		else if (args[1] == "list")
		{
			output = PositionDebugMgr::Instance()->TraceList(role->GetID());
			return true;
		}
	}
	else if (args.size() == 3)
	{
		if (args[1] == "add")
		{
			UINT64 unitID = convert<UINT64>(args.at(2));
			PositionDebugMgr::Instance()->TraceUnit(role->GetID(), unitID);
			output = "add trace " + args[2];
			return true;
		}
		else if (args[1] == "remove")
		{
			UINT64 unitID = convert<UINT64>(args.at(2));
			PositionDebugMgr::Instance()->TraceRemoveUnit(role->GetID(), unitID);
			output = "remove trace " + args[2];
			return true;
		}
	}
	
	output =  "trace add unitID\n";
	output += "trace remove unitID\n";
	output += "trace list\n";
	output += "trace clear\n";

	return true;
}

static bool DebugWayPoint(Role* role, std::vector<std::string>& args, std::string& output)
{
	std::vector<DummyRole*> dummys;
	role->GetCurrScene()->GetDummyRoles(dummys);
	if(dummys.empty())
	{
		SSError << " can not find dummy role " << END;
		return true;
	}
	DummyRole* dm = dummys[0];
	if((int)args.size() >= 2)
	{
		if("aion" == args[1])
		{
			if(dm->GetXObject()->AIComponent())
			{
				dm->GetXObject()->AIComponent()->OnAIEnable(NULL, NULL);
			}
		}
		else if("aioff" == args[1])
		{
			if(dm->GetXObject()->AIComponent())
			{
				dm->GetXObject()->AIComponent()->OnAIStop(NULL,NULL);
			}
		}
		else if("tran" == args[1] && (int)args.size() >= 4)
		{
			float x = convert<float>(args[2]);
			float z = convert<float>(args[3]);
			float y = role->GetCurrScene()->GetCollide().GetHeight(x,z);

			Vector3 despos(x,y,z); 
			float face = dm->GetXObject()->GetFaceDegree();
			XEntity* entity = dynamic_cast<XEntity*>(dm->GetXObject());

			if(!entity)
			{
				return true;
			}

			entity->TransferLocation(despos, face);
		}
	}
	return true;
}

static bool DebugTeleport(Role* role, std::vector<std::string>& args, std::string& output)
{
	if(args.size() != 3)
	{
		output = "para error : flash x y";
		return false;
	}
	float x = convert<float>(args.at(1));
	float z = convert<float>(args.at(2));
	if(role && role->GetCurrScene())
	{	
		//role->GetCurrScene()->AddTeleportDoor(role->GetXObject()->GetPosition().x + 2 , role->GetXObject()->GetPosition().z + 2 , 1 , x , z , role->GetXObject()->GetFaceDegree());
	}
	return true;
}

static bool DebugOneBlood(Role* role, std::vector<std::string>& args, std::string& output)
{
	if((int)args.size() == 1)
	{
		role->SetAttr(BASIC_CurrentHP , 1);
	}
	else if((int)args.size() == 2 && "op" == args[1])
	{
		const std::vector<Unit*>* ops = role->GetCurrScene()->GetOpponents(role->GetFightGroup());
		if (NULL != ops)
		{
			for(size_t i = 0; i < ops->size(); ++i)
			{
				(*ops)[i]->SetAttr(BASIC_CurrentHP , 1);
			}
		}
	}
	return true;
}

static bool DebugDrawCD(Role* role, std::vector<std::string>& args, std::string& output)
{
	LotteryRecord* record = role->Get<LotteryRecord>();
	record->ClearCoolDown();

	output = "succeed";

	return true;
}

static bool DebugKillAll(Role* role, std::vector<std::string>& args, std::string& output)
{
	if(role && role->GetCurrScene())
	{
		role->GetCurrScene()->KillAllEnemy();
	}
	return true;
}

static bool DebugDrop(Role* role, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2)
	{
		output = "drop id";
		return false;
	}
	int dropid = convert<int>(args[1]);
	const ItemDesc * conf = ItemDropConfig::Instance()->GetRandomDrop(dropid);
	if (NULL == conf)
	{
		output ="invalid id";
		return false;
	}

	BagGiveItemTransition trans(role);
	trans.SetReason(ItemFlow_GM, ItemFlow_GM_Give);
	trans.GiveItem(*conf);
	trans.NotifyClient();

	return true;
}

static bool DebugCanReach(Role* role, std::vector<std::string>& args, std::string& output)
{
	if(args.size() != 7)
	{
		output = "param error";
		return false;
	}

	Vector3 stPos , destPos;
	stPos.x = convert<float>(args[1]);
	stPos.y = convert<float>(args[2]);
	stPos.z = convert<float>(args[3]);

	destPos.x = convert<float>(args[4]);
	destPos.y = convert<float>(args[5]);
	destPos.z = convert<float>(args[6]);

	Scene* scene = role->GetCurrScene();
	if(scene)
	{
		scene->CheckMoveReach(role , stPos , destPos - stPos);
	}

	return true;
}

static bool DebugGMWin(Role* role, std::vector<std::string>& args, std::string& output)
{
	if(role && role->GetCurrScene())
	{
		role->GetCurrScene()->KillAllEnemy();
		role->GetCurrScene()->EndSyncScene(SCENE_WIN);
	}

	return true;
}

static bool DebugSetOpenServerTime(Role* role, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2)
	{
		output = "setopenservertime XXXX-XX-XX";
		return false;
	}

	GSConfig::Instance()->SetGameServerOpenTimeStamp(args[1]);

	output = "setopenservertime " + args[1] + " ok";

	return true;
}

static bool DebugPetOp(Role* role, std::vector<std::string>& args, std::string& output)
{
	/*if (role == NULL)
	{
	SSWarn << " role is NULL " << END;
	}
	if (args.size() != 3)
	{
	output = "petop petuid op";
	return false;
	}
	PetOp op(role->GetID());
	PetInfo* petinfo = op.GetPet(role->GetID(), convert<UINT64>(args[1]));
	UINT32 type = convert<UINT32>(args[2]);
	if (0 == type)
	{
	op.Fellow(role, petinfo);
	}
	else if (1 == type)
	{
	op.Fight(role, petinfo);
	}

	output = "ok";*/

	/*if (args.size() != 3)
	{
	output = "petop petuid op";
	return false;
	}
	*/
	PetSys* pPetSys = role->Get<PetSys>();
	if (pPetSys == NULL)
	{
		return false;
	}
	
	UINT64 pet_01  = convert<UINT64>(args[1]);
	UINT64 pet_02  = convert<UINT64>(args[2]);
	pPetSys->OPPetExpTransfer(pet_01,pet_02);
	return true;
}

static bool DebugPetExp(Role* role, std::vector<std::string>& args, std::string& output)
{
	/*
	if (args.size() != 3)
	{
		output = "petexp petuid exp";
		return false;
	}
	PetOp op(role->GetID());
	op.DebugExp(role, convert<UINT64>(args[1]), convert<UINT32>(args[2]));

	output = "ok";
	*/

	if (args.size() != 2)
	{
		output = "petop petuid op";
		return false;
	}
	PetSys* pPetSys = role->Get<PetSys>();	
	UINT32 add_exp  = convert<UINT32>(args[1]);
	pPetSys->TestAddExp(add_exp);

	return true;
}

static bool DebugTShowVoteEnd(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{

	/*TShowRankList* list = (TShowRankList*)CRankListMgr::Instance()->GetRankList(TShowVoteCountRank);
	if (list)
	{
		list->Update();
	}
	CBriefRankList* listBrief = (CBriefRankList*)CRankListMgr::Instance()->GetRankList(FashionPowerPointRank);
	if (listBrief)
	{
		listBrief->Update();
	}
	for (auto i = RoleSummaryMgr::Instance()->Begin(); i != RoleSummaryMgr::Instance()->End(); ++i)
	{
		UINT64 roleID = i->second->qwRoleID;
		SocialInfo* info = SocialMgr::Instance()->GetSocialInfo(roleID);
		if (info)
		{
			info->tshow.DebugReset();
		}
	}*/
	
	outputMessage = "tshow vote end ok";
	return true;
}

static bool DebugResetVoteInfo(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	CTShowRecord* record = pRole->Get<CTShowRecord>();
	if (record)
	{
		record->DebugReset();
	}
	outputMessage = "tshow reset sef vote info end ok";
	return true;
}

static bool DebugSRank(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	int size = (int)(args.size());
	if(size >= 3)
	{
		UINT32 sceneid = convert<UINT32>(args[1]); 
		int rank = convert<UINT32>(args[2]);
		pRole->Get<StageMgr>()->SetRank(sceneid, rank);
	}
	return true;
}

static bool DebugTeam(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	int size = (int)(args.size());
	if(size >= 2)
	{
		const string op = args[1];
		if(op == "adddh" && size == 2)
		{
			pRole->Get<CTeamRecord>()->AddHelpCount(DRAGON_TEAM_SCENE);
		}
		else if(op == "adddh" && size == 3)
		{
			int type = convert<int>(args[2]);
			pRole->Get<CTeamRecord>()->AddHelpCount(type);
		}
		else if(op == "cleardh")
		{
			pRole->Get<CTeamRecord>()->WeekZero();
		}
		else if(op == "fetched" && size >= 3)
		{
			int id = convert<int>(args[2]);
			DragonHelpConfig::Instance()->HandleFetchReward(pRole, id);
		}
		else if(op == "printdh")
		{
			DragonHelpConfig::Instance()->Print(pRole);
		}
	}
	else
	{
		TeamCopyMgr::Instance()->InfoPrint();
	}
	return true;
}

static bool DebugTeamCount(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	CTeamRecord* stage = pRole->Get<CTeamRecord>();
	if(3 == (int)args.size())
	{
		std::string op = args[1];
		int type = convert<int>(args[2]);
		if("update" == op)
		{
			stage->Update();
		}
		else if("addfi" == op)
		{
			stage->AddTeamFinishCountToday(type);	
		}
		else if("addex" == op)
		{
			stage->AddTeamExtraAddCount(type);
		}
		else if("addbuy" == op)
		{
			stage->AddTeamBuyCountToday(type);
		}
		else if("get" == op)
		{
			SSWarn << stage->GetTeamFinishCountToday(type) << " " 
				<< stage->GetTeamBuyCountToday(type) << " " 
				<< stage->GetTeamMaxCountToday(type) << " "
				<< stage->GetTeamLeftCountToday(type) << END;
		}
		else if("vip" == op)
		{
			SSWarn << " max = " << stage->GetTeamMaxCountToday(type) 
				<< " finish = " << stage->GetTeamFinishCountToday(type) 
				<< " left = " << stage->GetTeamLeftCountToday(type) << END;
		}
		else if("diamondcount" == op)
		{
			int count = type;
			for(int i = 0; i < count && i < 1000; ++i)
			{
				stage->AddDiamondCostCount();
			}
		}
		else if(args[1] == "addut")
		{
			pRole->Get<CTeamRecord>()->AddUseTicketCount();
		}
	}
	else
	{
		stage->TestPrint();
	}
	return true;
}

static bool DebugAddMail(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	int size = args.size();
	if(size < 6 || 0 != (size - 6)%2)
	{
		outputMessage = "addmail type title content istemplate expiretime(sec) itemid itemcount ";
		return false;
	}
	UINT32 type = convert<UINT32>(args[1]);
	std::string title = args[2];
	std::string content = args[3];
	bool istemplate = convert<bool>(args[4]);
	UINT32 expiretime = convert<UINT32>(args[5]);

	std::vector<ItemDesc> items;
	for (int i = 6; i < size; i = i+2)
	{
		UINT32 itemid = convert<UINT32>(args[i]);
		UINT32 itemcount = convert<UINT32>(args[i+1]);
		items.push_back(ItemDesc(itemid, itemcount));
	}

	bool istrue = false;
	if (istemplate)
	{
		//istrue = MailMgr::Instance()->AddSystemMail(expiretime, title, content, &items);
	}
	else
	{
		//MailSender sender;
		//sender.SetContent(Mail_System, title, content);
		//sender.SetExpiretime(expiretime);
		//sender.SetItems(items);
		//sender.Send(pRole->GetID());
	}
	if (istrue)
	{
		outputMessage = "addmail ok";
	}
	else
	{
		outputMessage = "addmail failed";
	}
	return true;
}

static bool DebugAddRoleMail(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	int size = args.size();
	if(size < 6 || 0 != (size - 6)%2)
	{
		outputMessage = "addrolemail type title content rolename expiretime(sec) itemid itemcount ";
		return false;
	}
	UINT32 type = convert<UINT32>(args[1]);
	std::string title = args[2];
	std::string content = args[3];
	std::string rolename = args[4];
	UINT32 expiretime = convert<UINT32>(args[5]);

	std::vector<ItemDesc> items;
	for (int i = 6; i < size; i = i+2)
	{
		UINT32 itemid = convert<UINT32>(args[i]);
		UINT32 itemcount = convert<UINT32>(args[i+1]);
		items.push_back(ItemDesc(itemid, itemcount));
	}
	
	//RoleSummary* summary = RoleSummaryMgr::Instance()->FindRoleSummary(rolename);
	//if (NULL == summary)
	//{
	//	outputMessage = "addmail failed";
	//	return true;
	//}

	MailSender sender;
	sender.SetReason(ItemFlow_GM_Give);
	sender.SetContent(Mail_System, title, content);
	sender.SetExpiretime(expiretime);
	sender.SetItems(items);
	sender.Send(pRole->GetID());

	outputMessage = "addmail ok";
	return true;
}

static bool DebugRobot(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() >= 2)
	{
		if (args[1] == "reload")
		{
			if (RobotConfig::Instance()->Reload())
			{
				outputMessage = "reload robot success!";
			}
			else
			{
				outputMessage = "reload robot failed!";
			}
		}
		else if (args[1] == "lib")
		{
			std::stringstream oss;
			RobotGenerator::Instance()->DebugDumpLibrary(oss);
			outputMessage = oss.str();
			LogDebug("%s", outputMessage.c_str());
		}
		else if (args[1] == "print")
		{
			RobotConfig::Instance()->TogglePrint();
			outputMessage = "toggle print ok";
		}
		else if (args[1] == "clone")
		{
			if (args.size() == 3)
			{
				auto *pConf = RobotConfig::Instance()->FindRobotConf(convert<int>(args[2]));
				if (pConf == NULL)
				{
					outputMessage = "not found robot template id: " + args[2];
					return true;
				}

				// 重置职业
				pRole->ResetProfession();

				// 清空身上的装备
				Bag *pBag = pRole->Get<Bag>();
				BagWearItemTransition transition(pRole);
				for (int i = 0; i < EquipCount; ++i)
				{
					XItem *pItem = pBag->GetItemOnBody(EQUIP, i);
					if (pItem)
					{
						transition.TakeoffItem(pItem);
					}
				}

				for (int i = 0; i < EmblemCount; ++i)
				{
					XItem *pItem = pBag->GetItemOnBody(EMBLEM, i);
					if (pItem)
					{
						transition.TakeoffItem(pItem);
					}
				}
				transition.NotifyClient();

				RobotGenerator::Instance()->DebugTransforRobotConfToRole(pRole, pConf);

				outputMessage = "move Robot Conf to Role success, please Logout!!!!!";
			}
			else
			{
				outputMessage = "robot clone RobotID";
			}
		}
		else if (args[1] == "create")
		{
			if(args.size() == 3)
			{
				int id = convert<int>(args[2]);
				RobotConf *pConf = RobotConfig::Instance()->FindRobotConf(id);
				if (pConf == NULL)
				{
					outputMessage = "not found robot template config: " + args[2];
					return false;
				}

				EnterSceneInfo entersceneinfo = SceneConfig::Instance()->GetEnterSceneInfo(KKSG::ENTER_SCENE_SWITCH, pRole->GetCurrScene()->GetSceneTemplateID());
				RoleSummary *pSummary = RobotGenerator::Instance()->CreateRobotSummary(pConf);
				RobotGenerator::Instance()->UseRobotSummary(pSummary);
				DummyRole *pRobot = DummyRoleManager::Instance()->Create(pSummary, pRole->GetXObject()->GetPosition_p(),
					pRole->GetXObject()->GetFaceDegree(), KKSG::FightRole, entersceneinfo);
				pRole->GetCurrScene()->AddUnitAndNotify(pRobot);
				RobotGenerator::Instance()->FreeRoleSummary(pSummary);
			}
			else
			{
				outputMessage = "robot create configRobotID";
				return false;
			}
		}
	}

	return true;
}

static bool DebugResetExpBack(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(2 != (int)args.size())
	{
		outputMessage = "error, format: resetexpback 1";
		return false;
	}
	int day = atoi(args[1].c_str());
	CFindBackRecord* record = pRole->Get<CFindBackRecord>();
	if (record)
	{
		record->DebugReset(day);
	}
	outputMessage = "expback reset ok";
	return true;
}

static bool DebugBuyEx(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if((int)args.size() >= 2)
	{
		int type = convert<int>(args[1]);
		int fagitueID = 0;
		if(3 == (int)args.size())
		{
			fagitueID = convert<int>(args[2]);
		}
		int ret = 0;
		switch(type)
		{
		case DIAMOND_BUY_FATIGUE:
			{
				ret = BuyExchangeMgr::Instance()->BuyFatigue(pRole, DIAMOND, fagitueID);
			}
			break;
		case DIAMONE_BUY_DRAGONCOIN:
			{
				ret = BuyExchangeMgr::Instance()->BuyDragonCoinFunPre(pRole, DIAMOND, 1);	
			}
			break;
		case DIAMONE_BUY_GOLD:
			{
				ret = BuyExchangeMgr::Instance()->BuyGold(pRole, DIAMOND);
			}
			break;
		case DRAGON_BUY_FATIGUE:
			{
				ret = BuyExchangeMgr::Instance()->BuyFatigue(pRole, DRAGON_COIN, fagitueID);
			}
			break;
		case DRAGONCOIN_BUY_GOLD:
			{
				ret = BuyExchangeMgr::Instance()->BuyGold(pRole, DRAGON_COIN);
			}
			break;
		default:
			break;
		}
	}
	return true;
}

static bool DebugForbidWord(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(2 == (int)args.size())
	{
		int flag = (int)ForbidWordMgr::Instance()->HaveForbidWord(args[1]);
		ForbidWordMgr::Instance()->FilterForbidWord(args[1]);
		SSWarn << flag << " " << args[1] << END;
		outputMessage = args[1];
	}
	return true;
}

static bool DebugGetDesignation(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if( 2 == (int)args.size() )
	{
		int did = convert<int>(args[1]);
		SSWarn << "Handle Gm Command " << args[0] << ", designationid:" << did << END;
		DesignationMgr::Instance()->OnAddGetDesigantion(pRole, did,GameTime::GetTime());
		outputMessage = "get designation success";
	}
	return true; 
}

static bool DebugGetAchievement(Role* pRole, std::vector<std::string>& args, std::string& outputMessage )
{
	if( 2 > (int)args.size() )
	{
		SSWarn << "Handle Gm FinishAchieve Leck Arg, " << args[0] << END;
		return true; 
	}

	int aid = convert<int>(args[1]);
	SSWarn << "Handle Gm Command " << args[0] << ", achieveid:" << aid << END;
	AchieveV2Mgr::Instance()->OnAddGetAchievement(pRole, aid);
	outputMessage = "get designation success";
	return true; 
}

static bool DebugSkill(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	int size = (int)args.size();	
	if(size >= 2 && args[1] == "clear")
	{
		pRole->GetXObject()->SkillComponent()->EndSkill();
		SSWarn << __FUNCTION__ << END;
	}
	return true;
}

static bool DebugGoldClick(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	int size = (int)args.size();	
	if(size >= 2)
	{
		if (args[1] == "reset")
		{
			pRole->Get<LotteryRecord>()->ResetCount();
			return true;
		}
		else if (args[1] == "cd")
		{
			pRole->Get<LotteryRecord>()->ResetCD();
			return true;
		}
	}
	outputMessage = "reset or cd";
	return false;
}

static bool DebugResetGuildBonus(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	pRole->Get<CGuildRecord>()->DebugResetGuildBonus();
	outputMessage = "resetguildbonus success";
	return true;
}

static bool DebugDailyTask(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	TaskRecord* task = pRole->Get<TaskRecord>();
	DailyTaskRecord& daily = task->GetDailyRecord();
	std::vector<SubTask*>& tasks = daily.GetAllSubTasks();
	std::ostringstream oss;
	foreach(i in tasks)
	{
		SubTask* t = *i;
		oss << "taskId: " << t->id << ", step: " << t->step << ", maxStep: " << t->maxStep << std::endl;
	}
	outputMessage = oss.str();

	return true;
}

static bool DebugCommon(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(3 == (int)args.size() && args[1] == "br")
	{
		int k = convert<int>(args[2]);
		if(0 == k) BossRushMgr::Instance()->FinishOneCount(pRole, false);
		if(1 == k) BossRushMgr::Instance()->FinishOneCount(pRole, true);
		if(2 == k) 
		{
			BossRushMgr::Instance()->MakeRefreshCost(pRole);
			BossRushMgr::Instance()->RefreshData(pRole);
			BossRushMgr::Instance()->AddRefreshCount(pRole);
		}
		if(3 == k) BossRushMgr::Instance()->TestClear(pRole);
		if(4 == k) BossRushMgr::Instance()->OnRoleCountinue(pRole);
		if(pRole->GetCurrScene() && pRole->GetCurrScene()->GetBRHandler())
		{
			if(5 == k) pRole->GetCurrScene()->GetBRHandler()->TestDead(true);
			if(6 == k) pRole->GetCurrScene()->GetBRHandler()->TestDead(false);
		}
		BossRushMgr::Instance()->TestPrint(pRole);
	}
	else if(2 == (int)args.size() && args[1] == "watcherop")
	{
		const std::unordered_map<UINT64, Role*>& tempwatchers = pRole->GetCurrScene()->GetWatchers();
		std::vector<Role*> watchers;
		for (auto i = tempwatchers.begin(); i != tempwatchers.end(); ++i)
		{
			watchers.push_back(i->second);
		}
		for(size_t i = 0; i < watchers.size(); ++i)
		{
			SceneManager::Instance()->EnterScene(watchers[i], watchers[i]->GetCurrScene(), false);
		}
	}
	else if(2 == (int)args.size() && args[1] == "addpklose")
	{
		pRole->Get<CPkRecord>()->AddLoseNum();
	}
	else if(2 == (int)args.size() && args[1] == "addhelper")
	{
		pRole->Get<StageMgr>()->AddHelperWinCount();
	}
	return true;
}


static bool DebugLeagueBattle(Role* pRole, std::vector<std::string>& args, std::string& output)
{
	int size = (int)args.size();
	if (NULL == pRole->GetCurrScene())
	{
		return false;
	}
	SceneLeague* lb = pRole->GetCurrScene()->GetLeagueBattleHandler();
	if (NULL == lb)
	{
		return false;
	}
	if (size < 2)
	{
		return false;
	}

	if (args[1] == "ready")
	{
		lb->GMSetReady();
		return true;
	}

	return true;
}

static bool DebugGmf(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	int size = (int)args.size();
	if(NULL == pRole->GetCurrScene())
	{
		return false;
	}
	if(NULL == pRole->GetCurrScene()->GetVsHandler())
	{
		return false;
	}
	SceneGWB* handler = (pRole->GetCurrScene()->GetVsHandler()->GetHandler<SceneGWB>());
	if(NULL == handler)
	{
		return false;
	}
	KKSG::GmfReadyArg oArg;
	KKSG::GmfReadyRes oRes;
	if(size >= 2 && "up" == args[1])
	{
		oArg.set_type(KKSG::GMF_READY_UP);
		handler->HandlerJoinRequest(pRole, oArg, oRes);
	}
	else if(size >= 2 && "down" == args[1])
	{
		oArg.set_type(KKSG::GMF_READY_DOWN);
		handler->HandlerJoinRequest(pRole, oArg, oRes);
	}
	else if(size >= 2 && "ins" == args[1])
	{
		if(pRole->GetCurrScene()->GetInspire())
		{
			KKSG::InspireArg oArg;
			KKSG::InspireRes oRes;
			pRole->GetCurrScene()->GetInspire()->HandlerReq(pRole, oArg, oRes);
		}
	}
		
	return true;
}
static bool DebugPvp(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	pRole->Get<MyPvpRecord>()->TestPrint();
	
	return true;
}

static bool DebugLoginRecord(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(2 <= (int)args.size())
	{
		if(4 == (int)args.size() && "set" == args[1])
		{
			int day = convert<int>(args[2]);	
			int state = convert<int>(args[3]);
			pRole->Get<MyLoginRecord>()->SetLoginRewardState(day, (KKSG::LoginRewardState)state);
			pRole->Get<MyLoginRecord>()->TestPrint();
		}
		else if(3 == (int)args.size() && "get" == args[1])
		{
			int day = convert<int>(args[2]);	
			pRole->Get<MyLoginRecord>()->TestPrint();
			//SSWarn << "item = " << pRole->Get<MyLoginRecord>()->GetLoginRewardItem(day) << END;
			SSWarn << "state = " << pRole->Get<MyLoginRecord>()->GetLoginRewardState(day) << END;
		}
		else if(2 == (int)args.size() && "clear" == args[1])
		{
			pRole->Get<MyLoginRecord>()->TestClear();
			pRole->Get<MyLoginRecord>()->TestPrint();
		}
		else if(2 == (int)args.size() && "addday" == args[1])
		{
			pRole->Get<MyLoginRecord>()->TestAddLoginDay();
		}
	}
	else
	{
		pRole->Get<MyLoginRecord>()->TestPrint();
	}

	return true;
}

static bool DebugSetGuildCheckInNum(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() != 2)
	{
		outputMessage = "set checkin num error, DebugSetGuildCheckInNum count";
		return false; 
	}
	#if GUILD_GS
	Guild* guild = GuildMgr::Instance()->FindGuildByRoleId(pRole->GetID());
	if (!guild)
	{
		outputMessage = "not in guild";
		return false;
	}

	guild->Get<CGuildCheckin>()->DebugSetCount(atoi(args[1].c_str()));
#endif
	outputMessage = "set checkin num ok";
	return true;
}

static bool DebugAgreeFriend(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() != 2)
	{
		outputMessage = "agreefriend roleid";
		return false; 
	}

	//RoleSummary* summary = RoleSummaryMgr::Instance()->FindRoleSummary(args[1]);
	//if (NULL == summary)
	//{
	//	outputMessage = "agree failed";
	//	return false;
	//}

	//UINT64 otherid = summary->qwRoleID;
	//FriendOp op(pRole->GetID());
	//if (op.DeleteApply(otherid))
	//{
	//	op.SetOther(otherid);
	//	op.AddFriend();
	//	outputMessage = "agree ok";
	//}
	//else
	//{
	//	outputMessage = "agree failed";
	//}

	return true;
}

static bool DebugUploadChatInfo(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() != 3)
	{
		outputMessage = "agreefriend roleid";
		SSError << "Lack Param, Least 3,  Command:" << args[0] << END;
		return false; 
	}
	int channel = convert<int>(args[1]);
	std::string content = args[2];
	SSInfo << "Handle Gm Command " << args[0] << END;
	///>AudioChatMgr::Instance()->GmUploadChatInfo(pRole, channel, content);
	return true; 
}

static bool DebugLoadChatInfo(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if( args.size() < 2 )
	{
		outputMessage = "lack arg, least 2, cmd: LoadChatInfo";
		return false; 
	}
	int channel = convert<int>(args[1]);
	SSInfo << "Handle Gm Command, RoleID:" << pRole->GetID() << ", Command:" << args[0] << END;

	UINT64 storekeyid = 0; 
	if( channel == KKSG::PrivateChannel )
	{
		storekeyid = pRole->GetID();
	}
	if( channel == KKSG::WorldChannel )
	{
		storekeyid = 0;
	}
	if( channel == KKSG::GuildChannel )
	{
		#if GUILD_GS
		Guild* guild = GuildMgr::Instance()->FindGuildByRoleId(pRole->GetID());
		if( NULL != guild )
		{
			storekeyid = guild->GetID();
		}
#endif
	}
	if( channel == KKSG::TeamChannel )
	{
		//storekeyid = pRole->GetTeamID();
	}
	///>AudioChatMgr::Instance()->LoadChatInfo(channel, storekeyid);
	SSInfo << "Finish Handle Gm Command, RoleID:" << pRole->GetID() << ", Command:" << args[0] << END;
	return true; 
}

static bool DebugChatCount(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if( args.size() < 2 )
	{
		outputMessage = "chat count, leck param, least 3";
		return false; 
	}
	int channel = convert<int>(args[1]);
	UINT64 keyId = convert<UINT64>( args[2] );

	///>AudioChatMgr::Instance()->GetChatCount( channel, keyId );
	return true; 
}


static bool DebugResetDragon(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if( args.size() < 3 )
	{
		outputMessage = "failed, resetdragon type hardlevel [clear]";
		return false; 
	}
	bool clear = false;
	if (args.size() == 4)
	{
		clear = true;
	}
	CDragonRecord* record = pRole->Get<CDragonRecord>();
	record->DebugReset(convert<int>(args.at(1)), convert<int>(args.at(2)), clear);
	return true; 
}

static bool DebugResetCheckInBonus(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	CGuildRecord* record = pRole->Get<CGuildRecord>();
	record->DebugResetCheckInBonusNum();
	return true; 
}

//flowerdesignationnotice
static bool DebugDesignationFlower(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	return true; 
}


static bool DebugWorldChatParse( Role *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	///>AudioChatMgr::Instance()->ParseWorldChat();
	return true; 
}

//showdesignation;
static bool DebugShowDesignation( Role *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	DesignationMgr::Instance()->OnShowDesignation(pRole, "GM-CMD");	
	return true;
}

static std::map<std::string, GMHandler> GMFunctions;

static bool DebugCmds(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	foreach (i in GMFunctions)
	{
		outputMessage += i->first;
		outputMessage += "\n";
	}

	return true;
}

static bool SetGuildBossTime( Role *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	if (args.size() != 3)
	{
		return false;
	}

	PtcG2M_GmsetGuildBossTime msg; 
	msg.m_Data.set_starttime(convert<UINT32>(args[1]) / 100 * 3600 + convert<UINT32>(args[1]) % 100 * 60);
	msg.m_Data.set_endtime(convert<UINT32>(args[2]) / 100 * 3600 + convert<UINT32>(args[2]) % 100 * 60);

	pRole->SendToMS(msg);
	MultActivityMgr::Instance()->SetGuildBossTime(convert<UINT32>(args[1]), convert<UINT32>(args[2]));

	/*ptcg2m_gmsetguildboss

	PtcG2M_GmsetGuildBossTime msg; 
	msg.m_Data.set_starttime(convert<UINT32>(args[1]));
	msg.m_Data.set_endtime(convert<UINT32>(args[2]));
	MSLink::Instance()->SendTo(msg);

	MultActivityMgr::Instance()->SetGuildBossTime(convert<UINT32>(args[1]), convert<UINT32>(args[2]));

	ptcg2m_gmsetguildboss

	UINT32 beginhour = convert<UINT32>(args[1]);
	UINT32 beginmin = convert<UINT32>(args[2]);
	UINT32 endhour = convert<UINT32>(args[3]);
	UINT32 endmin = convert<UINT32>(args[4]);
	GuildBossMgr::Instance()->ResetAll(beginhour, beginmin, endhour, endmin);

	 GetGlobalConfig().GuildBossEnterTime[1] = convert<UINT32>( args[1] ); 
	 GetGlobalConfig().GuildBossEnterTime[2] = convert<UINT32>( args[2] ); 
	 GetGlobalConfig().GuildBossEnterTime[3] = convert<UINT32>( args[3] ); 
	 GetGlobalConfig().GuildBossEnterTime[4] = convert<UINT32>( args[4] );*/ 
	return true; 
}


static bool SetGuildLadderTime( Role *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	if (args.size() < 4)
	{
		return false;
	}

	 GetGlobalConfig().GuildLadderTime.begin()->seq[0] = convert<UINT32>(args[1]); 
	 GetGlobalConfig().GuildLadderTime.begin()->seq[1] = convert<UINT32>(args[2]); 
	 GetGlobalConfig().GuildLadderTime.begin()->seq[2] = convert<UINT32>(args[3]); 
	 SSWarn<<"the ladder time:"<<GetGlobalConfig().GuildLadderTime.begin()->seq[0]<<GetGlobalConfig().GuildLadderTime.begin()->seq[1]<<GetGlobalConfig().GuildLadderTime.begin()->seq[2]<<END;
	return true;
}

static bool SetGuildArenaTime( Role *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	if (args.size() < 16)
	{
		return false;
	}

	 GetGlobalConfig().GuildArenaTimeVec.reserve(1);
	for (UINT32 i = 1; i <= 15; ++i)
	{
		 GetGlobalConfig().GuildArenaTimeVec[0][i - 1] = convert<UINT32>(args[i]);
	}

	return true;
}

static bool AddGuildDareTime( Role *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	if (args.size() < 1)
	{
		return false;
	}

	#if GUILD_GS
	Guild* pGuild = GuildMgr::Instance()->FindGuildByRoleId(pRole->GetID());
	if (pGuild != NULL)
	{
		pGuild->AddDareTimes(convert<UINT32>(args[1]));
	}
#endif
	return true;
}

///>adddesignationtodb
static bool DebugAddDesignationToDB( Role *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	if( args.size() < 3 )
	{
		return false; 
	}
	UINT64 roleID = convert<UINT64>( args[1] );
	UINT32 did = convert<UINT32>( args[2] );
	//DesignationMgr::Instance()->AddDesignationToDB( roleID, did );
	return true; 
}

static bool DebugGetDesignationFromDB( Role *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	if( args.size() < 2 )
	{
		return false; 
	}
	UINT64 roleID = convert<UINT64>( args[1] );
	EventMgr::Instance()->AddEvent(roleID,  DESIGNATION_COM_TYPE_FIRST_KILL_GUILD_BOSS, 1);
	EventMgr::Instance()->AddEvent(roleID,  DESIGNATION_COM_TYPE_FIRST_KILL_GUILD_BOSS, 2);
	EventMgr::Instance()->AddEvent(roleID,  DESIGNATION_COM_TYPE_FIRST_KILL_GUILD_BOSS, 3);
	return true; 
}

static bool DebugResetWorldBoss( Role *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	if( args.size() != 5 )
	{
		return false; 
	}
	UINT32 beginhour = convert<UINT32>(args[1]);
	UINT32 beginmin = convert<UINT32>(args[2]);
	UINT32 endhour = convert<UINT32>(args[3]);
	UINT32 endmin = convert<UINT32>(args[4]);

	PtcG2M_WorldBossSyncMS ntf;
	KKSG::WorldBossSyncTime* timenode = ntf.m_Data.mutable_timenode();
	timenode->set_beginhour(beginhour);
	timenode->set_beginmin(beginmin);
	timenode->set_endhour(endhour);
	timenode->set_endmin(endmin);
	pRole->SendToMS(ntf);

	return true; 
}

static bool SetCoverDesignation( Role *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	if( args.size() < 2 )
	{
		return false;
	}
	UINT32 coverDid = convert<UINT32>( args[1] );
	DesignationMgr::Instance()->SetCoverDesignation( pRole, coverDid );
	return true; 
}

static bool DebugSetPKWin( Role *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	if( args.size() < 2 )
	{
		return false; 
	}
	UINT32 winTimes = convert<UINT32>( args[1] );
	CPkRecord* record = pRole->Get<CPkRecord>();
	record->Update();
	record->SetWin( winTimes );
	return true; 
}

static bool DebugClearAchievement( Role *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	record->Init();
	return true; 
}

static bool DebugClearDesignation( Role *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	UINT32 coverId = record->GetCoverDesignation();
	record->Init();
	if( coverId > 0 )
	{
		if( coverId > 0 )
		{
			UINT32 tempCover = record->GetCoverDesignation();
			RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_DESIGNATION, (void*)&tempCover);
			RoleOutLook outlook( pRole );
			outlook.Fill(SUMMARY_DESIGNATION, NULL);
			outlook.BroadCast();
			DesignationMgr::Instance()->SendCoverDesignationNotify( pRole );
			SSWarn << "Update Player Cover Designation, RoleID:" << pRole->GetID() << ", CoverID:" << record->GetCoverDesignation()<< END;
		}
	}
	return true; 
}


static bool DebugCreateBattleScene(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() < 2)
	{
		return false; 
	}

	return true;
}
static bool DebugEnterBattleScene(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() < 3)
	{
		return false; 
	}
	UINT32 dwTemplateID = convert<UINT32>(args[1]);
	UINT32 dwInstanceID = convert<UINT32>(args[2]);
	KKSG::SceneSwitchData data;
	CSceneSwitch::EnterScene(pRole, dwTemplateID, dwInstanceID, data, false, 0);
	return true;
}

static bool DebugQAAnwerRight( Role *pRole, std::vector<std::string> &args, std::string &outputMessage ){	return true; }

static bool DebugClearChat( Role *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	ChatRecord* record = pRole->Get<ChatRecord>();
	if( NULL == record )
	{
		outputMessage = "chatrecord null";
		return false;
	}
	record->InitChat();
	ChatMgr::Instance()->SendWorldChannelLeftTimesNotify(pRole);
	return true; 
}

static bool DebugShowItem( Role *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	if( args.size() < 2 )
	{
		outputMessage = "arg not enough";
		return false;
	}
	Bag* pBag = pRole->Get<Bag>();
	if( NULL == pBag )
	{
		outputMessage = "showitem bag null";
		return false;
	}
	UINT32 dwItemID = convert<UINT32>( args[1] );
	UINT32 itemCount = 0; 
	if( dwItemID <= 32 )
	{
		itemCount = (UINT32)pBag->CountVirtualItem( dwItemID );
	}
	else
	{
		itemCount = pBag->CountItemInBag( dwItemID );
	}
	pBag->CountVirtualItem(dwItemID);
	outputMessage = "item id:" + ToString(dwItemID) + ", itemcount:" + ToString(itemCount);
	return true; 
}

static bool DebugAI( Role *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	if( args.size() < 2 )
	{
		outputMessage = "arg not enough, ex:[aidebug 100178675654654666752]";
		return false;
	}

	if (args[1] == "all")
	{
		if (args.size() > 2)
		{
			UINT32 param = convert<UINT32>(args[2]);
			AIOnlineDebugMgr::Instance()->SetDebugAll(pRole->GetID(), (bool)param);
			return true;
		}
	}
	UINT64 uid = convert<UINT64>(args[1]);
	Unit* pTarget;
	if (uid == 0)
		pTarget = pRole->GetCurrScene()->GetGlobalAI();
	else if (uid == ~0)
		pTarget = NULL;
	else
		pTarget = Unit::FindUnit(uid);
	
	if (pTarget)
		AIOnlineDebugMgr::Instance()->AddWatch(pTarget, pRole);
	else
		AIOnlineDebugMgr::Instance()->RemoveObserver(pRole->GetID());

    return true;
}

static bool DebugAIOld( Role *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	if( args.size() < 3 )
	{
		outputMessage = "arg not enough, ex:[aidebugold 100178675654654666752 1]";
		return false;
	}

	UINT64 dwUIDorTemplateID = convert<UINT64>(args[1]);
	UINT32 enable = convert<UINT32>(args[2]);

	if (enable > 0)
		AIOnlineDebugMgr::Instance()->AddDebugUnit(pRole);
	else
		AIOnlineDebugMgr::Instance()->RemoveDebugUnit(pRole);

	if (dwUIDorTemplateID == 0)
		XAIComponent::_global_debug = (enable != 0);
	else
	{
		Unit* unit = Unit::FindUnit(dwUIDorTemplateID);
		if (unit) unit->GetXObject()->AIComponent()->SetDebug(enable!=0);
		else
		{
			FindEnemyTemplateIdCondition cond( (UINT32)dwUIDorTemplateID );
			std::vector<Unit *> units;
			pRole->GetCurrScene()->GetUnitByCondition(cond, units);

			for (UINT32 i=0; i<units.size(); i++)
			{
				units[i]->GetXObject()->AIComponent()->SetDebug(enable);
			}
		}
	}

	return true; 
}


static bool DebugAddEvent(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if( args.size() < 2 )
	{
		outputMessage = "arg not enough";
		return false;
	}
	UINT32 nEventID = convert<UINT32>( args[1] );
	EventMgr::Instance()->AddEvent(pRole->GetID(),nEventID,10);
	return true;
}


static bool DebugIBShop(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	IBShopMgr::Instance()->ClearRecord(pRole);
	return true;
}

static bool DebugSetPlayerDegree(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if( args.size() < 2 )
	{
		outputMessage = "arg not enough";
		return false;
	}
	UINT32 dwDegree = convert<UINT32>( args[1] );
	IBShopRecord* pRecord	= pRole->Get<IBShopRecord>();
	pRecord->SetPayDegree(dwDegree);
	return true;
}


static bool DebugOpenQA(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size()<2)
		return false; 
	UINT32 qaType = convert<UINT32>(args[1]);
	QAMgr::Instance()->GMOpenQA(pRole, qaType);
	return true;
}

static bool DebugResetQA(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	QAMgr::Instance()->GMResetRole(pRole);
	return true;
}

static bool DebugCreateVoipRoom( Role *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	ApolloVoiceMgr::Instance()->GmCreateVoipRoom( pRole );
	return true; 
}

///>GM指令：显示voip实例
static bool DebugShowVoip( Role *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	ApolloVoiceMgr::Instance()->ShowVoipInfo();
	return true; 
}

///>加入voip房间
static bool DebugJoinVoipRoom( Role *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	ApolloVoiceMgr::Instance()->HandleJoinRoom(pRole);
	return true; 
}

///>退出voip房间
static bool DebugExitVoipRoom( Role *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	UINT64 roomID = 0; 
	UINT32 memberID = 0; 
	ApolloVoiceMgr::Instance()->FindRoleVoipInfo(pRole, roomID, memberID);
	ApolloVoiceMgr::Instance()->HandleExitRoom(pRole, roomID, memberID, "GMCommand");
	return true; 
}

static bool DebugLoadVoipXML( Role *pRole, std::vector<std::string> &args, std::string &outputMessage )
{
	ApolloVoiceMgr::Instance()->LoadVoipXML();
	return true; 
}

static bool DebugResetStageRank( Role *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	pRole->Get<CFirstPassRecord>()->DebugReset();
	outputMessage = "succeed";
	return true;
}

static bool DebugResetRisk( Role *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	pRole->Get<CSuperRiskRecord>()->DebugReset();
	outputMessage = "succeed";
	return true;
}

static bool DebugResetTower( Role *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 2)
	{
		outputMessage = "error, format: resettower 1";
		return false;
	}
	pRole->Get<CTowerRecord>()->DebugReset(atoi(args[1].c_str()) == 0);
	outputMessage = "succeed";
	return true;
}

static bool DebugPassTower( Role *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 2)
	{
		outputMessage = "error, format: passtower + floor, example: passtower 2";
		return false;
	}
	pRole->Get<CTowerRecord>()->DebugPassTower((UINT32)atoi(args[1].c_str()));
	outputMessage = "succeed";
	return true;
}

static bool DebugAddKillGoblin( Role *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 2)
	{
		outputMessage = "error, format: addkillgoblin + num, example: addkillgoblin 2";
		return false;
	}
	outputMessage = "succeed";
	return true;
}



static bool DebugSetAssist( Role *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 2)
	{
		return false;
	}
	for(int i = 0; i<(int)atoi(args[1].c_str());i++)
		pRole->Get<StageMgr>()->AddHelperWinCount();
	outputMessage = "succeed";
	return true;
}

static bool GMAutoPlay( Role *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 2)
	{
		outputMessage = "error, format: gmautoplay 0 or 1";
		return false;
	}

	int autoplay = atoi(args[1].c_str());

	pRole->AIComponent()->SetGmAutoPlay(autoplay != 0);
	outputMessage = "succeed";
	return true;
}

static bool DebugResetDice( Role *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	pRole->Get<CSuperRiskRecord>()->DebugResetDice();
	outputMessage = "succeed";
	return true;
}

static bool DebugDice( Role *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 2)
	{
		outputMessage = "error, format: dice 1";
		return false;
	}
	int num = atoi(args[1].c_str());
	if(!pRole->Get<CSuperRiskRecord>()->m_superRisk.DebugSetDiceNum(num))
	{
		outputMessage = "error, num: 1~6";
		return false;
	}
	outputMessage = "succeed";
	return true;
}

static bool DebugPassScene(Role* role, std::vector<std::string>& args, std::string& output)
{
	if (args.size() < 2)
	{
		output = "error, use like: passscene sceneid";
		return false;
	}
	SceneInfo* info = SceneConfig::Instance()->FindSceneInfo(convert<int>(args[1]));
	if (!info)
	{
		output = "error, sceneid not exist";
		return false;
	}
	int rank = RANK_SSS;
	if (args.size() >= 3 && args[2] == "not")
	{
		rank = RANK_EMPTY;
	}
	role->Get<StageMgr>()->SetRank(info->m_pConf->id, rank);
	return true;
}

static bool DebugStageAssist(Role* role, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2)
	{
		output = "stageassist point";
		return false;
	}
	output = role->Get<StageMgr>()->DumpStageAssistPoint();	
	return true;
}

static bool DebugChangeLine(Role* role, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2)
	{
		output = "changeline line";
		return false;
	}
	Scene* scene = role->GetCurrScene();
	if (NULL != scene)
	{
		std::vector<Role*> roles;
		roles.push_back(role);
		scene->ChangeLine(roles, convert<UINT32>(args[1]));
	}
	return true;
}

static bool DebugSetTitle(Role* role, std::vector<std::string>& args, std::string& output)
{
	if(args.size() != 2)
	{
		output = "settitle + num";
		return false;
	}
	UINT32 level = convert<UINT32>(args[1]);
	UINT32 maxlevel = TitleConfig::Instance()->GetMaxTitle();
	if(level < 1 || level > maxlevel)
	{
		std::stringstream ss;
		ss << "num must >= 1 && <= " << maxlevel;
		output = ss.str();
		return false;
	}
	role->Get<TitleRecord>()->DebugSetTitle(level);
	output = "set title succeed";
	return true;
}

static bool DebugSetWatchNum( Role *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 2)
	{
		outputMessage = "error, format: setwatchnum + num, example: setwatchnum 2";
		return false;
	}
	if (!pRole->GetCurrScene() || !pRole->GetCurrScene()->GetLiveID())
	{
		return false;
	}
	pRole->GetCurrScene()->DebugAddWatchNum((UINT32)atoi(args[1].c_str()));

	LiveManager::Instance()->OnEnterWatch(pRole, pRole->GetCurrScene()->GetLiveID());

	outputMessage = "succeed";
	return true;
}

static bool DebugResetCard( Role *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 1)
	{
		outputMessage = "resetcard";
		return false;
	}

	pRole->Get<CGuildRecord>()->ResetCardCount();

	outputMessage = "succeed";
	return true;
}

static bool DebugOpenAct(Role *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 4)
	{
		outputMessage = "usage: setact 1 1130 1140";
		return false;
	}
	UINT32 actId = convert<UINT32>(args[1]);
	UINT32 startTime = convert<UINT32>(args[2]);
	UINT32 endTime = convert<UINT32>(args[3]);
	UINT32 startSec = startTime / 100 * 3600 + startTime % 100 * 60;
	UINT32 endSec = endTime / 100 * 3600 + endTime % 100 * 60;

	MultActivityMgr::Instance()->GMAddOpenDayTime(actId, startSec, endSec);
	return true;
}

static bool DebugResetAct(Role *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 2)
	{
		outputMessage = "usage: resetact 1";
		return false;
	}
	UINT32 actId = convert<UINT32>(args[1]);
	MultActivityMgr::Instance()->ResetConfig(actId);
	return true;
}

static bool DebugAddBuffItem(Role *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 2)
	{
		outputMessage = "usage: addbuffitem id";
		return false;
	}
	UINT32 itemid = convert<UINT32>(args[1]);
	pRole->Get<BuffRecord>()->AddBuffItem(itemid);

	return true;
}

static bool DebugInherit(Role *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if (args.size() != 2)
	{
		outputMessage = "usage: inherit rolename";
		return false;
	}
	Role* other = RoleManager::Instance()->FindByName(args[1]);
	if (NULL == other)
	{
		outputMessage = "role none";
		return false;
	}
	// push
	CGuildInherit::Instance()->Push(pRole->GetID(), other->GetID());
	// change scene 
	if (pRole->GetCurrScene()->GetSceneTemplateID() == GUILD_MAP_ID)
	{
		CGuildInherit::Instance()->Add(pRole);
	}
	else
	{
		SceneManager::Instance()->ChangeScene(pRole, GUILD_MAP_ID);
	}

	if (other->GetCurrScene()->GetSceneTemplateID() == GUILD_MAP_ID)
	{
		CGuildInherit::Instance()->Add(other);
	}
	else
	{
		SceneManager::Instance()->ChangeScene(other, GUILD_MAP_ID);
	}

	return true;
}

static bool DebugDailyHelp(Role *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	TaskRecord* task = pRole->Get<TaskRecord>();
	DailyTaskRecord& daily = task->GetDailyRecord();
	daily.GMResetHelpNum();

	return true;
}

static bool DebugRestAncientTimesAward(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	outputMessage = "RestAncientTimesAward sucess";
	pRole->Get<SpActivityRecord>()->ResetAncientTimesAward();
	return true;
}

static bool DebugSpActivity(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() == 1 || args.size() > 3)
	{
		outputMessage = "1:spactivity start + actid\n2:spactivity end + actid\n3:spactivity addday + actid\n4:spactivity cleanmyself\n5:spactivity completeall";
		return false;
	}
	if(args.size() == 2)
	{
		if(args[1] == "cleanmyself")
		{
			pRole->Get<SpActivityRecord>()->GMClear();
			outputMessage = "clean succeed, please relogin to refresh";
			return false;
		}
		else if(args[1] == "completeall")
		{
			pRole->Get<SpActivityRecord>()->GMCompleteAll();
			outputMessage = "completeall succeed, please relogin to refresh";
			return true;
		}
		else 
		{
			outputMessage = "1:spactivity start + actid\n2:spactivity end + actid\n3:spactivity addday + actid\n4:spactivity cleanmyself\n5:spactivity completeall";
			return false;
		}
	}
	UINT32 actid = convert<UINT32>(args[2]);
	if(args[1] == "start")
	{	
		if(actid > 200)
		{
			outputMessage = "Don't use this gm for start LevelSealActivity, you can use levelSeal gm command";
			return false;
		}
		if(SpActivityMgr::Instance()->GMStart(actid))
		{
			outputMessage = string("start actid:") + ToString(actid) + string(" succeed");
			return true;
		}
		else
		{
			outputMessage = string("actid: ") + ToString(actid) + string("is already open or can't find actid:") + ToString(actid) + string(" in SpActivityTime.txt");
			return false;
		}
	}
	else if(args[1] == "end")
	{	
		if(SpActivityMgr::Instance()->GMEnd(actid))
		{
			outputMessage = string("end actid:") + ToString(actid) + string(" succeed");
			return true;
		}
		else
		{
			outputMessage = string("actid:") + ToString(actid) + string(" not open yet");
			return false;
		}
	}
	else if(args[1] == "addday")
	{
		/*if(SpActivityMgr::Instance()->GMAddOneDay(actid))
		{
			outputMessage = "add one day succeed";
			return true;
		}
		else
		{
			outputMessage = string("actid:") + ToString(actid) + string(" not open yet");
			return false;
		}*/
		return false;
	}
	else
	{
		outputMessage = "1:spactivity start + actid\n2:spactivity end + actid\n3:spactivity addday + actid\n4:spactivity cleanmyself\n5:spactivity completeall";
		return false;
	}
	return true;
}

typedef bool (*ReLoadHandler)();

template<typename T>
bool ReLoadFun()
{
	return T::Instance()->LoadFile();
}

bool SendPtc()
{
	PtcG2M_ReloadVersionNtf versionPtc;
	MSLink::Instance()->SendTo(versionPtc);
	return true;
}

std::map<string, ReLoadHandler>reloadTable;

static void InitReLoad()
{
	reloadTable["BuffList"] = ReLoadFun<XBuffTemplateManager>;
	reloadTable["SkillList"] = ReLoadFun<XSkillEffectMgr>;
	reloadTable["DNExpedition"] = ReLoadFun<ExpeditionConfigMgr>;
	reloadTable["RandomSceneList"] = ReLoadFun<ExpeditionConfigMgr>;
	reloadTable["PkProfession"] = ReLoadFun<AttributePkMgr>;
	reloadTable["XEntityPresentation"] = ReLoadFun<XEntityInfoLibrary>;
	reloadTable["XEntityStatistics"] = ReLoadFun<XEntityInfoLibrary>;
	reloadTable["PlayerLevel"] = ReLoadFun<PlayerLevelManager>;
	reloadTable["SystemList"] = ReLoadFun<OpenSystemMgr>;
	reloadTable["TaskList"] = ReLoadFun<TaskMgr>;
	reloadTable["DoodadList"] = ReLoadFun<ItemConfig>;
	reloadTable["DropList"] = ReLoadFun<ItemConfig>;
	reloadTable["EquipList"] = ReLoadFun<ItemConfig>;
	reloadTable["FashionList"] = ReLoadFun<ItemConfig>;
	//reloadTable["ItemList"] = ReLoadFun<ItemConfig>;
	reloadTable["Shop"] = ReLoadFun<ShopMgr>;
	reloadTable["ShopType"] = ReLoadFun<ShopMgr>;
	reloadTable["Notice"] = ReLoadFun<NoticeMgr>;
	reloadTable["IBShop"] = ReLoadFun<IBShopMgr>;
	reloadTable["SceneList"] = ReLoadFun<SceneConfig>;
	reloadTable["AITree"] = ReLoadFun<AITreeMgr>;
	reloadTable["ChannelCloseSys"] =  ReLoadFun<ChannelCloseSysMgr>;
	reloadTable["Version"] = SendPtc;

}

static bool DebugReLoadTable( Role *pRole, std::vector<std::string> &args, std::string &outputMessage) 
{
	if(args.size() != 2)
	{
		outputMessage = "reload + tablename(not use suffix \".txt\")";
		return false;
	}
	if(reloadTable.find(args[1]) == reloadTable.end())
	{
		outputMessage = "can't find this table in reload list";
		return false;
	}
	bool ret = reloadTable[args[1]]();
	if(ret == false)
	{
		outputMessage = "table format is wrong, reload failed";
		return false;
	}
	outputMessage = "reload succeed";
	return true;
}

static bool DebugAddGuildInherit(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() != 2)
	{
		return false;
	}

	GetGlobalConfig().GuildInheritTeaLimit = convert<UINT32>(args[1]); 
	GetGlobalConfig().GuildInheritStuLimit = convert<UINT32>(args[1]); 
	return true;
}


static bool SetHorse(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() != 3)
	{
		outputMessage = "param error != 3";
		return false;
	}
	if(pRole->GetCurrScene()->GetSceneType() != SCENE_HORSE_RACE)
	{
		outputMessage = "scenetype error";
	}

	UINT32 nType = convert<UINT32>(args[1]);
	SceneHorseRace* pRace = pRole->GetCurrScene()->GetHorseRaceHandler();
	if (pRace)
	{
		if (nType ==1)
		{
			pRace->SetHorseAngle(pRole->GetID(), convert<UINT32>(args[2]));
		}else if(nType ==2)
		{
			pRace->SetHorseLaps(pRole->GetID(), convert<UINT32>(args[2]));
		}
	}
	outputMessage = "SetHorse 1 10 or SetHorse 2 2";
	return true;
}

static bool SetInheritCDtime(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() != 2)
	{
		return false;
	}

	GetGlobalConfig().GuildInheritCDTime = convert<UINT32>(args[1]); 
	return true;
}

static bool DebugEquipSuit(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() != 2)
	{
		outputMessage = "equipsuit + suitid";
		return false;
	}
	int suitid = convert<int>(args[1]);
	EquipSuitConf* conf = ItemConfig::Instance()->GetEquipSuitConf(suitid);
	if(conf == NULL)
	{
		outputMessage = "suitid can't find in EquipSuit.txt";
		return false;
	}
	BagGiveItemTransition give(pRole);
	give.SetReason(ItemFlow_GM, ItemFlow_GM_Give);
	for(auto i = conf->EquipID.begin(); i != conf->EquipID.end(); i++)
	{
		give.GiveItem(*i, 1);
	}
	give.NotifyClient();
	outputMessage = "get equip succeed";
	return true;
}

static bool DebugDragonExp(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	int size = (int)args.size();
	if(size == 3 && args[1] == "reset")
	{
		UINT32 sceneid = convert<UINT32>(args[2]);
		DragonExpConfig::Instance()->ResetProgress(pRole, sceneid);
	}
	else if(size == 3 && args[1] == "clcount")
	{
		UINT32 sceneid = convert<UINT32>(args[2]);
		pRole->Get<StageMgr>()->ResetPlayCount(sceneid);
	}
	else if(size == 2 && args[1] == "spflag")
	{
		if(pRole->Get<StageMgr>()->GetDeSpecialFlag())
		{
			outputMessage = "true";
		}
		else
		{
			outputMessage = "false";
		}
	}
	else if(size == 3 && args[1] == "spflag")
	{
		pRole->Get<StageMgr>()->SetDeSpecialFlag(args[2] != "0");
	}
	else
	{
		pRole->Get<StageMgr>()->InfoPrint();
	}
	return true;
}

static bool DebugTrans(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() < 2)
	{
		outputMessage = "trans id";
		return false;
	}
	pRole->Transform(convert<UINT32>(args[1]), UseHeroType_HeroBattle);

	outputMessage = "trans succeed";

	return true;
}

static bool OpenAtlas(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() < 2)
	{
		outputMessage = "trans id";
		return false;
	}
	pRole->Get<CAtlasSys>()->openGroup(convert<UINT32>(args[1]));

	return true;
}

static bool DebugRoleConfig(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() < 3)
	{
		outputMessage = "roleconfig 0/1 aa bb";
		return false;
	}
	if (args[1] == "0")
	{
		pRole->DelConfig(args[2]);
	}
	else
	{
		pRole->SetConfig(args[2], args.size() > 3 ? args[3] : "");
	}

	return true;
}

static bool DebugInvisible(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() < 2)
	{
		outputMessage = "invisible 0/1";
		return false;
	}
	pRole->MakeInvisible(convert<bool>(args[1]));

	outputMessage = "invisible succeed";

	return true;
}

static bool DebugGuildCastle(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(NULL == pRole->GetCurrScene())
	{
		return false;
	}
	auto pCastle = pRole->GetCurrScene()->GetCastleHandler();
	if(NULL == pCastle)
	{
		return false;
	}
	int size = args.size();
	
	if(size == 3 && args[1] == "occupy")
	{
		int type = convert<int>(args[2]);
		if(GCFJvDianType_IsValid(type))
		{
			pCastle->MakeOccupy(pRole->GetID(), (KKSG::GCFJvDianType)type);		
		}
	}
	else if(size == 2 && args[1] == "back")
	{
		pCastle->BackToReady(pRole->GetID());
	}
	else if(size == 3 && args[1] == "jvdian")
	{
		int type = convert<int>(args[2]);
		if(GCFJvDianType_IsValid(type))
		{
			VsJvDian jvdian(type, pRole->GetXObject()->GetPosition(), 2, pCastle);
			pCastle->AddJvdian(jvdian);
		}
	}

	return true;
}

static bool DebugDamage(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() < 2)
	{
		outputMessage = "damage + start/end";
		return false;
	}

	if(args[1] == "start")
	{
		pRole->Statistics().StartPrintDamage(0);
	}
	else if(args[1] == "end")
	{
		outputMessage = ToString(pRole->Statistics().StopPrintDamage());
	}
	return true;
}

static bool DebugGMSkill(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args.size() < 1)
	{
		outputMessage = "gmskill";
		return false;
	}
	///> 转职
	pRole->ResetProfession();
	UINT32 profession = ProfessionMgr::Instance()->GetBasicProfession(pRole->GetProfession()) + 10;
	if (0 != pRole->ChooseProfession(profession))
	{
		outputMessage = "choose profession failed";
		return false;
	}
	///>学习技能
	std::vector<ProfessionSkillInfo> skills = XSkillEffectMgr::Instance()->GetProfessionSkill(profession);
	for (auto i = skills.begin(); i != skills.end(); ++i)
	{
		UINT32 hashskill = i->SkillHash;
		int ret = pRole->GetRoleSkillLevelMgr()->GMTryLevelup(hashskill);
		if (0 != ret)
		{
			outputMessage += "skillid:" + ToString(hashskill) + ", error:" + ToString(ret);
		}
	}
	///> 绑定技能
	//Skill_1_Attack = 2,
	//Skill_2_Attack = 3,
	//Skill_3_Attack = 4,
	//Skill_4_Attack = 5,
	//Skill_5_Attack = 6,
	//Skill_1_Buff = 7,
	//Skill_2_Buff = 8,
	//Ultra_Attack = 9,
	std::vector<UINT32> bindskills;
	for (auto i = skills.rbegin(); i != skills.rend(); ++i)
	{
		bindskills.push_back(i->SkillHash);
	}
	for (int i = KKSG::Skill_1_Attack; i <= KKSG::Ultra_Attack; ++i)
	{
		for (auto j = bindskills.begin(); j != bindskills.end(); ++j)
		{
			if (0 == pRole->GetRoleSkillLevelMgr()->BindSkillSlot(i, *j))
			{
				bindskills.erase(j);
				break;
			}
		}
	}
	
	outputMessage = "succeed";
	return true;
}


static bool PetAttr(Role* pRole, std::vector<std::string>& args, std::string& outputMessage)
{
	outputMessage.append(pRole->Get<PetSys>()->GetStr());
	return true;
}

static bool DebugHeroEnd(Role* pRole, std::vector<std::string>& args, std::string& outputMessage)
{
	if (pRole->GetCurrScene() == NULL)
	{
		return false;
	}
	SceneHeroBattle* battle = pRole->GetCurrScene()->GetHeroBattleHandler();
	if (battle == NULL)
	{
		return false;
	}
	battle->GMEnd(pRole);
	return true;
}

static bool DebugMobaEnd(Role* pRole, std::vector<std::string>& args, std::string& outputMessage)
{
	if (pRole->GetCurrScene() == NULL)
	{
		return false;
	}
	SceneMobaBattle* battle = pRole->GetCurrScene()->GetMobaBattleHandler();
	if (battle == NULL)
	{
		return false;
	}
	battle->GMEnd(pRole->GetID());
	return true;
}

static bool DebugMobaAddExp(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = "mobaaddexp + num";
		return false;
	}
	if (pRole->GetCurrScene() == NULL)
	{
		return false;
	}
	SceneMobaBattle* battle = pRole->GetCurrScene()->GetMobaBattleHandler();
	if (battle == NULL)
	{
		return false;
	}
	UINT32 addExp = convert<UINT32>(args[1]);
	battle->GMAddExp(pRole->GetID(), addExp);
	return true;
}

static bool DebugSetHeroELO(Role* pRole, std::vector<std::string>& args, std::string& outputMessage)
{
	if (args.size() < 2)
	{
		outputMessage = "setheroelo + elopoint";
		return false;
	}
	double point = convert<double>(args[1]);
	pRole->Get<CHeroBattleRecord>()->SetELOPoint(point);
	outputMessage = "setheroelo succeed";
	return true;
}

static bool DebugGetHeroELO(Role* pRole, std::vector<std::string>& args, std::string& outputMessage)
{
	outputMessage = ToString(pRole->Get<CHeroBattleRecord>()->GetELOPoint());
	return true;
}

static bool DebugPopWindow(Role* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() == 3)
	{
		PopWindows::Instance()->SyncCommon(pRole,(KKSG::CommentType)convert<UINT32>(args[1]),(KKSG::CommentType)convert<UINT32>(args[2])); 
		return true;
	}
	output = "args error, format:popwindow type";
	return true;
}


static bool DebugClearWindow(Role* pRole, std::vector<std::string>& args, std::string& output)
{
	PopWindows::Instance()->ClearWindows(pRole);
	return true;
}

static bool DebugIdipOpenSystem(Role* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 3)
	{
		output = "args error, format:idipopensystem id [open|close]";
		return false;
	}
	UINT32 id = convert<UINT32>(args[1].c_str());
	bool isOpen = strcmp(args[2].c_str(), "open") == 0 ? true : false;
	OpenSystemMgr::Instance()->IdipOpSystem(id, isOpen);

	return true;
}

static bool DebugSetMilitary(Role* pRole, std::vector<std::string>& args, std::string& outputMessage)
{
	if(args[1] == "clear")
	{
		pRole->Get<MilitaryRankRecord>()->DebugClear();
	}
	else if (args[1] == "reduce")
	{
		UINT32  military_expoit  = SafeConvert<UINT32>(args[2]);	
		pRole->Get<MilitaryRankRecord>()->ReduceMilitaryExploit(military_expoit);
	}
	else
	{
		UINT32  military_expoit  = SafeConvert<UINT32>(args[1]);	
		pRole->Get<MilitaryRankRecord>()->SetMilitaryExploit(military_expoit);
	}
	outputMessage = "setmilitary succeed";
	return true;
}


static bool DebugShareGift(Role* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2)
	{
		output = "sharegift + id";
		return false;
	}
	UINT32 id = convert<UINT32>(args[1]);
	if (LotteryMgr::Instance()->GetShareGiftConf(id) == NULL)
	{
		output = "id is not exist in ShareRandomGift.txt";
		return false;
	}
	PtcG2C_ShareRandomGiftNtf oPtc;
	oPtc.m_Data.set_id(id);
	pRole->Send(oPtc);

	return true;
}

static bool DebugClearShareGift(Role* pRole, std::vector<std::string>& args, std::string& output)
{
	pRole->Get<LotteryRecord>()->GMClearShareGiftCount();

	return true;
}

static bool DebugEnhanceAll(Role* pRole, std::vector<std::string>& args, std::string& output)
{
	EnhanceEquipMgr::Instance()->GMEnhanceAllEquipOnBody(pRole);
	
	return true;
}

static bool DebugArgentaPreData(Role* pRole, std::vector<std::string>& args, std::string& output)
{
	pRole->Get<SpActivityRecord>()->GMPrintArgentaPreData(output);

	return true;
}

static bool DebugBackFlowPreData(Role* pRole, std::vector<std::string>& args, std::string& output)
{
	pRole->Get<SpActivityRecord>()->GMPrintBackFlowPreData(output);

	return true;
}

static bool DebugResetLoginActivity(Role* pRole, std::vector<std::string>& args, std::string& output)
{
	RoleMisc* record = pRole->Get<RoleMisc>();
	record->SetLoginActTime(0);
	output = "reset login activity sucess";
	return true;
}


static bool DebugGuildcampparty(Role* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() != 2)
	{
		output = "guildcampparty + rate";
		return false;
	}
	UINT32 rate = convert<float>(args[1]);
	GetGlobalConfig().GuildCampRandomEnemyFrequency = rate;

	return true;
}

static bool DebugBMResult(Role* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() < 2)
	{
		output = "bmresult + point";
		return false;
	}
	UINT32 nPoint = convert<UINT32>(args[1]);
	SceneBMFight* pBmFight = pRole->GetCurrScene()->GetBMFightHandler();;
	if (pBmFight)
	{
		pBmFight->ReportToWorld(pRole->GetID(),nPoint,0,1,0,pRole->GetProfession(),pRole->GetLevel());
	}else
	{
		output = "bigfightscene not exist";
		return false;
	}
	return true;
}

static bool DebugGetArtifact(Role* pRole, std::vector<std::string>& args, std::string& output)
{
	if (args.size() < 3)
	{
		output = "artifact + itemid + itemcount + effectid1 + effectid2 + ... + effectidn";
		return false;
	}
	UINT32 itemID = convert<UINT32>(args[1]);
	const ItemConf* itemConf = ItemConfig::Instance()->GetItemConf(itemID);
	if (itemConf == NULL)
	{
		output = "itemid:" + ToString(itemID) + " not in ItemList.txt";
		return false;
	}
	if (itemConf->ItemType != ARTIFACT)
	{
		output = "itemid:" + ToString(itemID) + " type is not Artifact";
		return false;
	}
	UINT32 count = convert<UINT32>(args[2]);
	if (count > 100)
	{
		output = "itemcount too many, bigger than 100";
		return false;
	}
	BagGiveItemTransition give(pRole);
	give.SetReason(ItemFlow_GM, ItemFlow_GM_Give);
	for (UINT32 i = 0; i < count; i++)
	{
		UINT64 uid = UIDHelper::CreateUID(UID_Item);
		XItem item(uid, itemID, 1, ARTIFACT);
		item.effects.clear();
		for (UINT32 j = 3; j < args.size(); j++)
		{
			KKSG::EffectData data;
			if (EffectConfig::Instance()->GetEffectData(convert<UINT32>(args[j]), data))
			{
				item.effects.push_back(data);
			}
		}
		EffectConfig::Instance()->UpdateEffectData(item.effects);
		give.GiveItem(&item);
	}
	give.NotifyClient();

	return true;}
static bool DebugPlatformshareClear(Role* pRole, std::vector<std::string>& args, std::string& output)
{
	pRole->Get<PlatformShareRecord>()->DebugTest();
	return true;
}

static bool DebugWeekEnd4v4ActivitySort(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
    if (args.size() != 2)
    {
        outputMessage = args[0] + " thisweekid";
        return false;
    }

    UINT32 thisweekid = convert<unsigned>(args.at(1));

    bool slag = WeekEnd4v4Mgr::Instance()->GMChangeActivitySort(thisweekid);
    if(slag)
    {
        outputMessage = "set weekend4v4 sort ok";
    }else
    {
        outputMessage = "set weekend4v4 sort failed";
    }
    return true;
}

static bool DebugWeekEnd4v4ClearRecord(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
    if (args.size() != 1)
    {
        outputMessage = args[0];
        return false;
    }

    pRole->Get<WeekEnd4v4Record>()->ResetCount();

    outputMessage = "clear weekend4v4 record ok";
    return true;
}

static bool DebugDragonGroupRecord(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
    if (args.size() != 1)
    {
        outputMessage = args[0];
        return false;
    }

    pRole->Get<DragonGroupRecord>()->GMResetRecord();

    outputMessage = "clear DragonGroup Record ok";
    return true;
}

static bool DebugTrophy(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if(args[1] == "clear")
	{
		pRole->Get<StageMgr>()->DebugTest(0,0,0);
	}
	else if(args[1] == "score")
	{
		UINT32 score = convert<UINT32>(args[2]);
		pRole->Get<StageMgr>()->DebugTest(0,0,score);
	}
	else if (args[1] == "sceneid")
	{
		UINT32 sceneid = convert<UINT32>(args[2]);
		UINT32 trophy_id = 0;
		UINT32 trophy_order = 0;
		pRole->Get<StageMgr>()->GetTrophyOrderBySceneID(sceneid,trophy_id,trophy_order);
		LogDebug("sceneid=%d,trophy_id=%d,trophy_order=%d",sceneid,trophy_id,trophy_order);
	}
	else
	{
		if (args.size() != 3)
		{
			outputMessage = "trophy_id  trophy_order";
			return false;
		}
		UINT32 trophy_id = convert<UINT32>(args[1]);
		UINT32 trophy_order = convert<UINT32>(args[2]);
		pRole->Get<StageMgr>()->DebugTest(trophy_id,trophy_order,0);

		outputMessage = "set  trophy_id  trophy_order ok";
	}
	return true;
}

static bool DebugAITree(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() < 2)
	{
		outputMessage = args[0] + " treename [uid]";
		return false;
	}

	Unit* pUnit = pRole;
	if (args.size() >= 3)
	{
		pUnit = Unit::FindUnit(convert<UINT64>(args[2]));
	}
	if (pUnit != NULL)
	{
		if (!pUnit->AIComponent())
		{
			pUnit->GetXObject()->SetAIComponent(static_cast<XAIComponent*>(pUnit->GetXObject()->AttachComponent(new XAIComponent())));
			pUnit->GetXObject()->AIComponent()->ManualAttached();
		}

		vector<std::string> vec(Split(GetGlobalConfig().GetConfig("WeddingPatrolPath"), '|'));
		std::vector<Sequence<float, 4>> navigation;
		for (size_t i = 0; i < vec.size(); ++i)
		{
			Sequence<float, 4> result;
			std::vector<std::string> vec1 = Split(vec[i], '=');
			for (unsigned i = 0; i < vec1.size(); ++i)
			{
				result[i] = SafeConvert<float>(vec1[i]);
			}

			navigation.push_back(result);
		}

		pUnit->AIComponent()->GetPatrol().InitNavPath(navigation, XPatrol::PT_NORMAL);
		pUnit->AIComponent()->SetBehaviorTree(args[1]);
		pUnit->AIComponent()->Enable(true);
		pUnit->EnableAI(true);

	}

	return true;
}

static bool DebugAddGMAttr(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() < 3)
	{
		outputMessage = args[0] + " id value";
		return false;
	}

	UINT32 id = convert<UINT32>(args[1]);
	double value = convert<double>(args[2]);
	if (pRole->Get<RoleMisc>()->AddGMAttr((CombatAttrDef)id, value))
	{
		pRole->RecalcuateAttributeAndNotify(RecalAttr_GMSetLevel);
		outputMessage = "succeed";
	}
	else
	{
		outputMessage = "failed";
	}

	return true;
}

static bool DebugClearGMAttr(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	pRole->Get<RoleMisc>()->ClearGMAttr();
	pRole->RecalcuateAttributeAndNotify(RecalAttr_GMSetLevel);

	outputMessage = "succeed";

	return true;
}

static bool DebugListGMAttr(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	pRole->Get<RoleMisc>()->ListGMAttr(outputMessage);

	return true;
}

static bool DebugWedding(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() < 2)
	{
		outputMessage = "need 2 paramater";
		return false;
	}
	if (args[1] == "stateend")
	{
		Scene* pScene = pRole->GetCurrScene();
		if (pScene == NULL)
		{
			return false;
		}
		SceneWedding* pWed = pScene->GetWeddingHandler();
		if (pWed == NULL)
		{
			return false;
		}
		pWed->GMSetStateEnd();
	}

	return true;
}

static bool DebugTransMonster(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() < 4)
	{
		outputMessage = "transmonster nickid buffid time";
		return false;
	}
	
	UINT32 nickid = convert<UINT32>(args[1]);
	UINT32 buffid = convert<UINT32>(args[2]);
	UINT32 time = convert<UINT32>(args[3]);

	Role* role = RoleManager::Instance()->FindByNickID(nickid);
	if (NULL == role)
	{
		outputMessage = "not find role";
		return false;
	}
	UINT32 ret = role->Get<BuffRecord>()->CanTrans();
	if (KKSG::ERR_SUCCESS != ret)
	{
		outputMessage = "failed";
		return false;
	}
	role->Get<BuffRecord>()->AddTransBuff(buffid, 1, pRole->GetID());
	
	outputMessage = "succeed";

	return true;
}

static bool DebugAddActivityValue(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = args[0] + " value";
		return false;
	}

	UINT32 value = convert<UINT32>(args.at(1));
	CActivityRecord* pRecord = pRole->Get<CActivityRecord>();
	if (NULL == pRecord)
	{
		return false;
	}

	pRecord->TestAddVaule(value);

	outputMessage = "add activity  value ok";

	return true;
}

static bool DebugResetFuseCompensation(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	pRole->Get<Bag>()->GMResetFuseCompensation();
	
	outputMessage = "Reset Succeed";

	return true;
}

static bool DebugResetForgeCompensation(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	pRole->Get<Bag>()->GMResetForgeCompensation();

	outputMessage = "Reset Succeed";

	return true;
}

static bool DebugAddBattleFieldPoint(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = args[0] + " value";
		return false;
	}

	Scene *pScene = pRole->GetCurrScene();
	if (pScene)
	{
		SceneBFFight* pFightScene = pScene->GetBFFightHandler();
		if(pFightScene)
		{
			pFightScene->AddPoint(pRole->GetID(),  convert<UINT32>(args.at(1)));
		}
	}
	return true;
}


static bool DebugSetBattleFieldRecord(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		return false;
	}
	UINT32 id		= convert<UINT32>(args.at(1));
	CBattleFieldRecord* pRecord = pRole->Get<CBattleFieldRecord>();
	pRecord->AddPointIdCount(id);
	return true;
}


static bool DebugTestHellDoor(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 4)
	{
		return false;
	}
	UINT32 type		= convert<UINT32>(args.at(1));
	UINT32 id		= convert<UINT32>(args.at(2));
	UINT32 value	= convert<UINT32>(args.at(3));
	if (type==1)
	{
		std::vector<const ItemDesc*> itemList;
		for (int j = 0; j < value; j++)
		{
			HellDoorDropMgr::Instance()->GetItem(pRole,id,itemList);
			for (size_t i = 0; i < itemList.size(); i++)
			{
				BagGiveItemTransition transition(pRole);
				transition.SetReason(ItemFlow_GM, ItemFlow_GM_Give);
				transition.GiveItem(itemList[i]->itemID, (UINT32)itemList[i]->itemCount, true);
				transition.NotifyClient();
				LogDebug("helldoor itemid=%u itemcount=%u",itemList[i]->itemID,itemList[i]->itemCount);
			}
		}
	}else if(type==2)
	{
		CBattleFieldRecord* pRecord = pRole->Get<CBattleFieldRecord>();
		pRecord->SetDropInfo(id,value);
	}
	return true;
}

static bool DebugSceneCastle(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		outputMessage = args[0] + " value";
		return false;
	}

	Scene *pScene = pRole->GetCurrScene();
	if (pScene)
	{
		SceneCastle* pFightScene = pScene->GetCastleHandler();
		if(pFightScene)
		{				
			for (int i = 0; i < convert<UINT32>(args.at(1)); i++)
			{
				pFightScene->OnKill(pRole->GetID(), 0);
			}
		}
	}
	return true;
}

static bool DebugSpecialEffectsID(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		return false;
	}
	UINT32 id		= convert<UINT32>(args.at(1));

	pRole->Get<RoleFashion>()->TestAddDebugSpecialEffectsID(id);	

	return true;
}

static bool DebugUpdateStage(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	pRole->Get<StageMgr>()->UpdateStageInfo2Client();
	return true;
}

static bool DebugGiveUIDItem(Role *pRole, std::vector<std::string> &args, std::string &outputMessage)
{
	if (args.size() != 3)
	{
		outputMessage = "uiditem + itemid + uid";
		return false;
	}
	UINT32 itemid = convert<UINT32>(args[1]);
	UINT64 uid = convert<UINT64>(args[2]);
	ItemConf* conf = ItemConfig::Instance()->GetItemConf(itemid);
	if (conf == NULL)
	{
		outputMessage = "itemid can't find in ItemList.txt";
		return false;
	}
	XItem item(uid, itemid, 1, conf->ItemType);
	BagGiveItemTransition give(pRole);
	give.SetReason(ItemFlow_GM, ItemFlow_GM_Give);
	give.GiveItem(&item);
	give.NotifyClient();
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
static void InitGMFunctions()
{
	GMFunctions["attr"] = DebugAddAttribute;
	GMFunctions["exp"] = DebugAddExp;
	GMFunctions["level"] = DebugSetLevel;
	GMFunctions["task"] = DebugTask;
	GMFunctions["item"] = DebugGiveItem;
	GMFunctions["equip"] = DebugGiveEquip;
	GMFunctions["takevirtualitem"] = DebugTakeVirtualItem;
	GMFunctions["opensystem"] = DebugOpenSystem;
	GMFunctions["openallsystem"] = DebugOpenAllSystem;
	GMFunctions["addarenacount"] = DebugAddArenaCount;
	GMFunctions["arenarank"] = DebugArenaRank;
	GMFunctions["setconfig"] = DebugSetConfig;
	GMFunctions["clearbag"] = DebugClearBag;
	GMFunctions["addactivitycount"] = DebugAddActivityCount;
	GMFunctions["enterscene"] = DebugEnterScene;
	GMFunctions["enter"] = DebugEnterScene;
	GMFunctions["gmenter"] = DebugGMEnterScene;
	GMFunctions["mobmonster"] = DebugMobMonster;
	GMFunctions["aicommand"] = DebugAICommand;
	GMFunctions["mobally"] = DebugMobDummyRole;
	GMFunctions["givereward"] = DebugGiveReward;
	GMFunctions["fetchreward"] = DebugFetchReward;
	GMFunctions["notice"] = DebugNotice;
	GMFunctions["addfriend"] = DebugAddFriend;
	GMFunctions["addguildbonus"] = DebugAddGuildBonus;
	GMFunctions["star"] = DebugSceneStar;
	GMFunctions["passscene"] = DebugPassScene;
	GMFunctions["guildexp"] = DebugAddGuildExp;
	GMFunctions["cmd"] = DebugCmds;
	//GMFunctions["version"] = DebugServerInfo;
	GMFunctions["trace"] = Trace;
	GMFunctions["addbuff"] = DebugAddBuff;
	GMFunctions["buff"] = DebugAddBuff;
	GMFunctions["listbuff"] = DebugListBuff;
	GMFunctions["changescene"] = DebugChangeScene;
	GMFunctions["sceneCD"] = DebugClearSceneCD;
	GMFunctions["addpkpoint"] = DebugAddPkPoint;
	GMFunctions["pk"] = DebugPk;
	GMFunctions["resetcount"] = DebugResetCount;
	GMFunctions["path"] = DebugPath;
	GMFunctions["fashion"] = DebugGetAllFashion;
	GMFunctions["waypoint"] = DebugWayPoint;
	GMFunctions["subhp"] = DebugSubHp;
	GMFunctions["addgaojiguexp"] = DebugAddGaoJiGuExp;
	GMFunctions["teleport"] = DebugTeleport;
	GMFunctions["oneblood"] = DebugOneBlood;
	GMFunctions["killall"] = DebugKillAll;
	GMFunctions["drawcd"] = DebugDrawCD;
	GMFunctions["addfishexp"] = DebugAddFishExp;
	GMFunctions["resetfishcount"] = DebugResetFishCount;
	GMFunctions["drop"] = DebugDrop;
	GMFunctions["reach"] = DebugCanReach;
	GMFunctions["gmwin"] = DebugGMWin;
	GMFunctions["setopenservertime"] = DebugSetOpenServerTime;
	GMFunctions["petop"] = DebugPetOp;
	GMFunctions["petexp"] = DebugPetExp;
	GMFunctions["tshowvoteend"] = DebugTShowVoteEnd;
	GMFunctions["resetselfvoteinfo"] = DebugResetVoteInfo;
	GMFunctions["teamcount"] = DebugTeamCount;
	GMFunctions["steam"] = DebugTeam;
	GMFunctions["addmail"] = DebugAddMail;
	GMFunctions["addrolemail"] = DebugAddRoleMail;
	GMFunctions["robot"] = DebugRobot;
	GMFunctions["resetexpback"] = DebugResetExpBack;
	GMFunctions["arena"] = DebugArena;
	GMFunctions["buyex"] = DebugBuyEx;
	GMFunctions["forbidword"] = DebugForbidWord;
	GMFunctions["getdesignation"] = DebugGetDesignation;
	GMFunctions["getachievement"] = DebugGetAchievement;
	GMFunctions["common"] = DebugCommon;
	GMFunctions["pvp"] = DebugPvp;
	GMFunctions["gmf"] = DebugGmf;
	GMFunctions["loginr"] = DebugLoginRecord;
	GMFunctions["levelsealtype"] = DebugSetLevelSealType;
	GMFunctions["levelsealtime"] = DebugSetLevelSealTime;
	GMFunctions["levelsealbosscount"] = DebugSetLevelSealBossCount;
	GMFunctions["levelsealcollectcount"] = DebugSetLevelSealCollectCount;
	GMFunctions["levelseal"] = DebugShowLevelSeal;
	GMFunctions["setpay"] = DebugSetPayV2;
	GMFunctions["paycard"] = DebugSetPayCardTime;
	GMFunctions["payvip"] = DebugPayVip;
	GMFunctions["gettodayconsume"] = DebugGetTodayConsume;
	GMFunctions["idippunish"] = DebugSetIdipPunish;
	GMFunctions["paymemberdragon"] = DebugPayMemberDragonFlowerTime;
	GMFunctions["paymemberexpire"] = DebugSetPayMemExpireTime;
	GMFunctions["payscore"] = DebugAddScore;
	GMFunctions["remind"] = DebugAntiAddictionRemind;
	GMFunctions["qqvip"] = DebugSetQQVip;
	GMFunctions["diamond"] = DebugAddDiamond;
	GMFunctions["consumediamond"] = DebugConsumeDiamond;
	GMFunctions["reportdata"] = DebugReportData;
	GMFunctions["clearsetpre"] = DebugClearSetPre;
	GMFunctions["updateuserinfo"] = DebugUpdateUserInfo;
	GMFunctions["setadult"] = DebugSetAdult;
	GMFunctions["paytotal"] = DebugPayTotal;
	GMFunctions["follow"] = DebugFollow;
	GMFunctions["setguildcheckinnum"] = DebugSetGuildCheckInNum;
	GMFunctions["agreefriend"] = DebugAgreeFriend;
	GMFunctions["uploadchatinfo"] = DebugUploadChatInfo;
	GMFunctions["loadchatinfo"] = DebugLoadChatInfo;
	GMFunctions["chatcount"] = DebugChatCount;
	GMFunctions["resetdragon"] = DebugResetDragon;
	GMFunctions["resetcheckinbonus"] = DebugResetCheckInBonus;
	GMFunctions["flowerdesignationnotice"] = DebugDesignationFlower;
	GMFunctions["parseworldchat"] = DebugWorldChatParse;
	GMFunctions["showdesignation"] = DebugShowDesignation;
	GMFunctions["setguildbosstime"] = SetGuildBossTime;
	GMFunctions["setcoverdesignation"] = SetCoverDesignation;
	GMFunctions["resetteamcount"] = DebugResetTeamCount;
	GMFunctions["setguildarenatime"] = SetGuildArenaTime;
	GMFunctions["adddesignationtodb"] = DebugAddDesignationToDB;
	GMFunctions["getdesignationfromdb"] = DebugGetDesignationFromDB;
	GMFunctions["resetworldboss"] = DebugResetWorldBoss;
	GMFunctions["addguilddaretime"] = AddGuildDareTime;
	GMFunctions["setpkwin"] = DebugSetPKWin;
	GMFunctions["removeallachievement"] = DebugClearAchievement;
	GMFunctions["removealldesignation"] = DebugClearDesignation;
	GMFunctions["createbattlescene"] = DebugCreateBattleScene;
	GMFunctions["enterbattlescene"] = DebugEnterBattleScene;
	GMFunctions["qaanswer"] = DebugQAAnwerRight;
	GMFunctions["clearchat"] = DebugClearChat;
	GMFunctions["showitem"] = DebugShowItem;
	GMFunctions["aidebugold"] = DebugAIOld;
	GMFunctions["aidebug"] = DebugAI;
	GMFunctions["createvoiproom"] = DebugCreateVoipRoom;
	GMFunctions["showvoip"] = DebugShowVoip;
	GMFunctions["joinvoiproom"] = DebugJoinVoipRoom;
	GMFunctions["quitvoiproom"] = DebugExitVoipRoom;
	GMFunctions["loadvoipxml"] = DebugLoadVoipXML;
	GMFunctions["openqa"]	= DebugOpenQA;
	GMFunctions["resetqa"] = DebugResetQA;
	GMFunctions["resetstagerank"]	= DebugResetStageRank;
	GMFunctions["ibshop"]			= DebugIBShop;
	GMFunctions["sprite"] = DebugGiveSprite;
	GMFunctions["stageassist"] = DebugStageAssist;
	GMFunctions["resetrisk"] = DebugResetRisk;
	GMFunctions["resetdice"] = DebugResetDice;
	GMFunctions["dice"] = DebugDice;
	GMFunctions["settitle"] = DebugSetTitle;
	GMFunctions["resettower"] = DebugResetTower;
	GMFunctions["passtower"] = DebugPassTower;
	GMFunctions["addkillgoblin"] = DebugAddKillGoblin;
	GMFunctions["setassist"] = DebugSetAssist;
	GMFunctions["gmautoplay"] = GMAutoPlay;
	GMFunctions["addevent"] = DebugAddEvent;
	GMFunctions["setwatchnum"] = DebugSetWatchNum;
	GMFunctions["setguildladdertime"] = SetGuildLadderTime;
	GMFunctions["resetcard"] = DebugResetCard;
	GMFunctions["setact"] = DebugOpenAct;
	GMFunctions["resetact"] = DebugResetAct;
	GMFunctions["spactivity"] = DebugSpActivity;
	GMFunctions["changeline"] = DebugChangeLine;
	GMFunctions["skill"] = DebugSkill;
	GMFunctions["goldclick"] = DebugGoldClick;	
	GMFunctions["resetguildbonus"] = DebugResetGuildBonus;	
	GMFunctions["dailytask"] = DebugDailyTask;
	GMFunctions["addbuffitem"] = DebugAddBuffItem;	
	GMFunctions["resethelp"] = DebugDailyHelp;
	GMFunctions["inherit"] = DebugInherit;
	GMFunctions["addguildinherit"] = DebugAddGuildInherit;
	GMFunctions["equipsuit"] = DebugEquipSuit;
	GMFunctions["damage"] = DebugDamage;
	GMFunctions["gmskill"] = DebugGMSkill;
	GMFunctions["setinheritcdtime"] = SetInheritCDtime;
	GMFunctions["horse"] = SetHorse;
	GMFunctions["petattr"] = PetAttr;
	GMFunctions["castle"] = DebugGuildCastle;
	GMFunctions["dragonexp"] = DebugDragonExp;
	GMFunctions["trans"] = DebugTrans;
	GMFunctions["invisible"] = DebugInvisible;
	GMFunctions["heroend"] = DebugHeroEnd;
	GMFunctions["openatlas"] = OpenAtlas;
	GMFunctions["roleconfig"] = DebugRoleConfig;
	GMFunctions["lb"] = DebugLeagueBattle;
	GMFunctions["idipopensystem"] = DebugIdipOpenSystem;
	GMFunctions["popwindow"] = DebugPopWindow;
	GMFunctions["setheroelo"] = DebugSetHeroELO;
	GMFunctions["getheroelo"] = DebugGetHeroELO;
	GMFunctions["setmilitary"] = DebugSetMilitary;
	GMFunctions["guildtest"] = DebugTestGuildBoss;
	GMFunctions["sharegift"] = DebugShareGift; 
	GMFunctions["clearsharegift"] = DebugClearShareGift;
	GMFunctions["reloadlua"] = DebugReloadAllLuaFile;
	GMFunctions["runluafile"] = DebugRunLuaFile;
	GMFunctions["enhanceall"] = DebugEnhanceAll;
	GMFunctions["srank"] = DebugSRank;
	GMFunctions["toskywait"] = DebugToSkyWait;
	GMFunctions["argentadata"] = DebugArgentaPreData;
	GMFunctions["backflowdata"] = DebugBackFlowPreData;
	GMFunctions["resetloginactivity"] = DebugResetLoginActivity;
	GMFunctions["setplayerdegree"] = DebugSetPlayerDegree;
	GMFunctions["guildcampparty"] = DebugGuildcampparty;
	GMFunctions["platformshareclear"] = DebugPlatformshareClear;
	GMFunctions["artifact"] = DebugGetArtifact;
	GMFunctions["mobaend"] = DebugMobaEnd;
    GMFunctions["weekend4v4sort"] = DebugWeekEnd4v4ActivitySort;
    GMFunctions["weekend4v4clear"] = DebugWeekEnd4v4ClearRecord;
    GMFunctions["dragongroupclear"] = DebugDragonGroupRecord;
	GMFunctions["listattr"] = DebugListAttr;
	GMFunctions["bmresult"] = DebugBMResult;
	GMFunctions["mobaaddexp"] = DebugMobaAddExp;
	GMFunctions["ecs"] = DebugECS;
	GMFunctions["trophy"] = DebugTrophy;
	GMFunctions["RestAncientTimesAward"] = DebugRestAncientTimesAward;
	GMFunctions["aitree"] = DebugAITree;
	GMFunctions["addgmattr"] = DebugAddGMAttr;
	GMFunctions["cleargmattr"] = DebugClearGMAttr;
	GMFunctions["listgmattr"] = DebugListGMAttr;
	GMFunctions["wedding"] = DebugWedding;
	GMFunctions["transmonster"] = DebugTransMonster;
	GMFunctions["invf"] = DebugInvfight;
	GMFunctions["npcfl"] = DebugNpcFeeling;
	GMFunctions["survive"] = DebugSurvive;
	GMFunctions["competedragon"] = DebugCompeteDragon;
	GMFunctions["addactivityvalue"] = DebugAddActivityValue;
	GMFunctions["resetfusecompensation"] = DebugResetFuseCompensation;
	GMFunctions["resetforgecompensation"] = DebugResetForgeCompensation;
	GMFunctions["addbfpoint"] = DebugAddBattleFieldPoint;
	GMFunctions["bfaward"] = DebugSetBattleFieldRecord;
	GMFunctions["helldoor"] = DebugTestHellDoor;
	GMFunctions["test"] = DebugTest;
	GMFunctions["testmul"] = DebugTestMul;
	GMFunctions["scenecasle"] = DebugSceneCastle;
	GMFunctions["fashionspecialeffect"] = DebugSpecialEffectsID;
	GMFunctions["updatestage"] = DebugUpdateStage;
	GMFunctions["uiditem"] = DebugGiveUIDItem;
	InitReLoad();
	GMFunctions["reload"] = DebugReLoadTable;
}

void RpcC2G_GMCommand::OnCall(const GMCmdArg &roArg, GMCmdRes &roRes)
{
	if (GMFunctions.empty())
	{
		InitGMFunctions();
	}

	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(false);
		roRes.set_outputmessage("");
		return;
	}
	// server debug or role gm	
	if (!GSConfig::Instance()->IsDebug() && !pRole->IsGM())
	{
		roRes.set_result(false);
		roRes.set_outputmessage("not gm");
		return;
	}

	bool result;
	std::string outputMessage;

	std::vector<std::string> args = Split(roArg.cmd(), ' ');
	if (args.size() == 0)
	{
		roRes.set_result(true);
		DebugCmds(pRole, args, outputMessage);
		roRes.set_outputmessage(outputMessage);
		return;
	}

	RpcG2M_GMCommandG2M* rpc = RpcG2M_GMCommandG2M::CreateRpc();
	UINT32 delayid = DelayRpc();
	rpc->m_oArg.set_delayid(delayid);
	rpc->m_oArg.set_roleid(pRole->GetID());
	rpc->m_oArg.mutable_c2garg()->CopyFrom(m_oArg);
	pRole->SendToMS(*rpc);

	if (GMFunctions.find(args[0]) != GMFunctions.end())
	{
		result = GMFunctions[args[0]](pRole, args, outputMessage);
		roRes.set_result(result);
		roRes.set_outputmessage(outputMessage);
		return;
	}

	roRes.set_result(false);
	roRes.set_outputmessage("command not found!");
	//result = CmdLine::RunCmd(roArg.cmd(), outputMessage);
	//roRes.set_result(result);
	//roRes.set_outputmessage(outputMessage);
}

void RpcC2G_GMCommand::OnDelayReplyRpc(const GMCmdArg &roArg, GMCmdRes &roRes, const CUserData &roUserData)
{
	if(NULL == roUserData.m_pUserPtr)
	{
		return ;
	}
	GMCmdg2mRes* tmp = (GMCmdg2mRes*)(roUserData.m_pUserPtr); 
	if(tmp->handled())//没有考虑共同处理的情况
	{
		roRes.CopyFrom(tmp->c2gres());
	}
}
