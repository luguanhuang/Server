#include "pch.h"
#include "rolemisc.h"
#include "util/gametime.h"
#include "table/globalconfig.h"
#include "util/XCommon.h"
#include "gamelogic/qqviprecord.h"
#include "unit/role.h"
#include "unit/systeminfo.h"
#include "idip/idiprecord.h"
#include "globalconfig.h"
#include "mail/mailconfig.h"
#include "mail/mailsender.h"
#include "gamelogic/nestconfig.h"
#include "table/expeditionconfigmgr.h"
#include "gamelogic/stagemgr.h"
#include "gamelogic/rolesummary.h"
#include "gamelogic/rolesummarymgr.h"
#include "common/ptcg2c_notifystartuptypetoclient.h"
#include "define/spactivityiddef.h"
#include "define/tlogenum.h"

RoleMisc::RoleMisc(Role *pRole)
:m_role(pRole)
{
	m_changeprocount = 0;
	m_lastchangeprotime = 0;
	m_dailyLbnum = 0;
	m_qqVipHint = false;
	m_qqVipReadTime = 0;
	m_eGameHint = false;
	m_eGameReadTime = 0;
	m_XinYueHint = false;
	m_XinYueReadTime = 0;
	m_lastLevel = 0;
	m_loginacttime = 0;
	m_loginactstatus = false;

	m_daygiftitems = 0;
	
	m_hardestNestExpID = 0;

	m_weddingFlowerCount = 0;
	m_weddingFireworksCount = 0;
	m_weddingCandyCount = 0;

	m_surviveData.set_lastweekuptime(GameTime::GetTime());
	m_kingBackReward = 0;
	m_freeHintTime = 0;
	m_chargeBackRewardLevel =0;
	m_chargeBackTotal = 0;
}

RoleMisc::~RoleMisc()
{

}

bool RoleMisc::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	const KKSG::RoleMiscData& data = poRoleAllInfo->misc();
	m_nPushTime = data.lastpush();
	m_nPushFlag = data.pushflag();
	m_nLadderTime = data.laddertime();
	m_nAnswersIndex = data.answersindex();
	m_nAnswersVersion = data.answersversion();
	m_nHintFlag = data.hintflag();

	m_changeprocount = data.changeprocount();
	m_lastchangeprotime = data.lastchangeprotime();
	m_dailyLbnum = data.daily_lb_num();
	m_updateTime = data.updatetime();

	declaration_ = data.declaration();

	m_qqVipHint = data.qqvip_hint();
	m_qqVipReadTime = data.qqvip_hint_read_time();

	m_eGameHint = data.egame_hint();
	m_eGameReadTime = data.egame_hint_readtime();

	m_XinYueHint = data.xinyue_hint();
	m_XinYueReadTime = data.xinyue_readtime();
	m_loginacttime = data.loginacttime();
	m_loginactstatus = data.loginactstatus();
	m_lastLevel = data.last_level();
	if (m_lastLevel == 0 && m_role->GetLevel())
	{
		m_lastLevel = m_role->GetLevel();
		SetSaveFlag();
	}

	m_daygiftitems = data.daygiftitems();

	m_hardestNestExpID = data.hardestnestexpid();
	m_startUpType = data.startuptype();
	m_startUpTypeTime = data.startuptime();
	m_weddingFlowerCount = data.weddingflow_count();
	m_weddingFireworksCount = data.weddingfireworks_count();
	m_weddingCandyCount = data.weddingcandy_count();
	m_turntable = data.turntable();

	if(data.has_surviverec())
	{
		m_surviveData = data.surviverec();
	}
	m_kingBackReward = data.kingbackrewardcount();

	if (GSConfig::Instance()->IsDebug())
	{
		for (int i = 0; i < data.gmattrs_size(); ++i)
		{
			const KKSG::AttributeInfo& info = data.gmattrs(i);
			m_id2gmattr[info.id()] = info.value(); 
		}
	}

	m_freeHintTime = data.freeflow_hinttime();
	m_chargeBackRewardLevel = data.chare_back_rewardlevel();
	m_chargeBackTotal = data.charge_back_total();

	return true;
}

void RoleMisc::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (!m_dirty.TestAndReset())
	{
		return;
	}
	KKSG::RoleMiscData *pData = poRoleAllInfo->mutable_misc();
	pData->set_lastpush(m_nPushTime);
	pData->set_pushflag(m_nPushFlag);
	pData->set_laddertime(m_nLadderTime);
	pData->set_answersindex(m_nAnswersIndex);
	pData->set_answersversion(m_nAnswersVersion);
	pData->set_hintflag(m_nHintFlag);

	pData->set_changeprocount(m_changeprocount);
	pData->set_lastchangeprotime(m_lastchangeprotime);
	pData->set_daily_lb_num(m_dailyLbnum);
	pData->set_updatetime(m_updateTime);

	pData->set_declaration(declaration_);
	pData->set_qqvip_hint(m_qqVipHint);
	pData->set_qqvip_hint_read_time(m_qqVipReadTime);

	pData->set_egame_hint(m_eGameHint);
	pData->set_egame_hint_readtime(m_eGameReadTime);

	pData->set_xinyue_hint(m_XinYueHint);
	pData->set_xinyue_readtime(m_XinYueReadTime);

	pData->set_last_level(m_lastLevel);
	pData->set_loginactstatus(m_loginactstatus);
	pData->set_loginacttime(m_loginacttime);
	pData->set_daygiftitems(m_daygiftitems);

	pData->set_hardestnestexpid(m_hardestNestExpID);
	pData->set_startuptype(m_startUpType);
	pData->set_startuptime(m_startUpTypeTime);

	pData->set_weddingflow_count(m_weddingFlowerCount);
	pData->set_weddingfireworks_count(m_weddingFireworksCount);
	pData->set_weddingcandy_count(m_weddingCandyCount);

	pData->mutable_turntable()->CopyFrom(m_turntable);
	*pData->mutable_surviverec() = m_surviveData;
	pData->set_kingbackrewardcount(m_kingBackReward);

	if (GSConfig::Instance()->IsDebug())
	{
		for (auto i = m_id2gmattr.begin(); i != m_id2gmattr.end(); ++i)
		{
			KKSG::AttributeInfo* info = pData->add_gmattrs();
			info->set_id(i->first);
			info->set_value(i->second);
		}
	}
	pData->set_freeflow_hinttime(m_freeHintTime);
	pData->set_chare_back_rewardlevel(m_chargeBackRewardLevel);
	pData->set_charge_back_total(m_chargeBackTotal);

	roChanged.insert(pData);
}


void RoleMisc::SetSaveFlag()
{
	m_dirty.Set();
}

bool RoleMisc::ChangeProCountLimit()
{
	return m_changeprocount >= GetGlobalConfig().ProfessionChangeCount;
}

bool RoleMisc::ChangeProTimeLimit()
{
	return 0 == m_lastchangeprotime ? false : (m_lastchangeprotime + GetGlobalConfig().ProfessionChangeTime) >= (UINT32)time(NULL);
}

void RoleMisc::OnHallEnterScene()
{
	CheckQQVipHint();
	CheckEGameHint();
	CheckXinYueHint();
	OnDayPass();
	m_role->Get<CIdipRecord>()->CheckAllHint();
}

void RoleMisc::OnLogin()
{
	if (!m_hardestNestExpID)
	{
		bool passEasyestNest = false;
		for (auto iter = NestConfig::Instance()->GetNestListTable().Table.begin(); iter != NestConfig::Instance()->GetNestListTable().Table.end(); ++iter)
		{
			if ((*iter)->Type == 1 && (*iter)->Difficulty == 1)
			{
				UINT32 expID = (*iter)->NestID;
				UINT32 sceneID = ExpeditionConfigMgr::Instance()->GetExpRandomScene(expID);
				passEasyestNest = m_role->Get<StageMgr>()->IsPassed(sceneID);
				break;
			}
		}

		if (passEasyestNest)
		{
			m_hardestNestExpID = NestConfig::Instance()->CalcHardestNestExpID(m_role);
			SetSaveFlag();
			LogInfo("%llu, init through expid:%u", m_role->GetID(), m_hardestNestExpID);
		}
	}
}

void RoleMisc::OnDayPass()
{
	UINT32 now = GameTime::GetTime();
	if (XCommon::IsDaySame(m_updateTime, now))
	{
		return;
	}
	m_dailyLbnum = 0;
	m_updateTime = now;
	m_daygiftitems = 0;
	CheckQQVipHint();
	CheckEGameHint();
	CheckXinYueHint();

	m_weddingFlowerCount = 0;
	m_weddingFireworksCount = 0;
	m_weddingCandyCount = 0;

	m_kingBackReward = 0;

	m_dirty.Set();
}

void RoleMisc::AddDailyLBNum(UINT32 value)
{
	m_dailyLbnum += value;
	m_dirty.Set();
}

void RoleMisc::CheckQQVipHint()
{
	bool hint = false;
	QQVipRecord* pQQRecord = m_role->Get<QQVipRecord>();

	do 
	{
		if (!m_role->Get<CRoleSystem>()->IsSystemOpened(SYS_QQVIP))
		{
			break;
		}
		/*
		if (!pQQRecord->IsVip() && !pQQRecord->IsSVip())
		{
			break;
		}
		*/
		INT32 weekDay = GameTime::GetWeekDay();
		if (std::find(GetGlobalConfig().QQVipHintDay.begin(), GetGlobalConfig().QQVipHintDay.end(), weekDay)
			== GetGlobalConfig().QQVipHintDay.end())
		{
			break;
		}
		UINT32 now = GameTime::GetTime();
		if (XCommon::IsDaySame(now, m_qqVipReadTime))
		{
			break;
		}
		hint = true;
	} while (0);

	if (hint && m_qqVipHint == false)
	{
		m_qqVipHint = hint;
		m_qqVipReadTime = 0;
		m_dirty.Set();
	}
	if (hint == false && m_qqVipHint)
	{
		m_qqVipHint = hint;
		m_dirty.Set();
	}

	NotifyQQVipHint();
}

void RoleMisc::ReadQQVipHint()
{
	m_qqVipReadTime = GameTime::GetTime();
	m_dirty.Set();
	CheckQQVipHint();
}

void RoleMisc::NotifyQQVipHint()
{
	UINT32 now = GameTime::GetTime();
	if (m_qqVipHint && !XCommon::IsDaySame(now, m_qqVipReadTime))
	{
		m_role->HintNotify(SYS_QQVIP, false);
	}
	else
	{
		m_role->HintNotify(SYS_QQVIP, true);
	}
}

void RoleMisc::CheckEGameHint()
{
	if (!m_role->Get<CRoleSystem>()->IsSystemOpened(SYS_EGame))
	{
		return;
	}

	bool hint = false;
	do
	{
		INT32 weekDay = GameTime::GetWeekDay();
		if (std::find(GetGlobalConfig().EGameHintDay.begin(), GetGlobalConfig().EGameHintDay.end(), weekDay) == GetGlobalConfig().EGameHintDay.end())
		{
			break;
		}
		UINT32 now = GameTime::GetTime();
		if (XCommon::IsDaySame(now, m_eGameReadTime))
		{
			break;
		}
		hint = true;
	} while (0);

	if (hint && m_eGameHint == false)
	{
		m_eGameHint = true;
		m_eGameReadTime = 0;
		m_dirty.Set();
	}
	else if (hint == false && m_eGameHint)
	{
		m_eGameHint = false;
		m_dirty.Set();
	}

	if (hint)
	{
		m_role->HintNotify(SYS_EGame, false);
	}
	else
	{
		m_role->HintNotify(SYS_EGame, true);
	}
}

void RoleMisc::ReadEGameHint()
{
	m_eGameReadTime = GameTime::GetTime();
	m_dirty.Set();
	CheckEGameHint();
}

void RoleMisc::CheckXinYueHint()
{
	do 
	{
		UINT32 now = GameTime::GetTime();
		if (XCommon::IsDaySame(now, m_XinYueReadTime))
		{
			m_XinYueHint = false;
			break;
		}
		bool isVip = m_role->Get<QQVipRecord>()->IsXinYueVip();
		if (!isVip)
		{
			m_XinYueHint = false;
			break;
		}

		if (m_role->Get<CIdipRecord>()->GetXinYueHint())
		{
			m_XinYueHint = true;
			break;
		}

		INT32 weekDay = GameTime::GetWeekDay();
		if (std::find(GetGlobalConfig().XinYueHintDay.begin(), GetGlobalConfig().XinYueHintDay.end(), weekDay) != GetGlobalConfig().XinYueHintDay.end())
		{
			m_XinYueHint = true;
			break;
		}
	} while (0);
	if (m_XinYueHint == true && m_XinYueReadTime != 0)
	{
		m_XinYueReadTime = 0;
		m_dirty.Set();
	}

	if (m_XinYueHint)
	{
		m_role->HintNotify(SYS_XINYUEVIP, false);
	}
	else
	{
		m_role->HintNotify(SYS_XINYUEVIP, true);
	}
}

void RoleMisc::ReadXinYueHint()
{
	m_XinYueReadTime = GameTime::GetTime();
	m_role->Get<CIdipRecord>()->SetXinYueHint(false);
	CheckXinYueHint();
	m_dirty.Set();
}

const UINT32 RoleMisc::GetLoginActTime()
{
	return m_loginacttime;
}


const bool RoleMisc::GetLoginActStatus()
{
	return m_loginactstatus;
}

void RoleMisc::OnLevelUp()
{
	UINT32 level = m_role->GetLevel();
	if (level >= GetGlobalConfig().LevelMailLevel && m_lastLevel < GetGlobalConfig().LevelMailLevel)
	{
		const MailConf* conf = MailConfig::Instance()->GetMailConf(GetGlobalConfig().LevelMailID);
		if (conf)
		{
			MailSender sender;
			sender.SetContent(Mail_System, conf->m_title,  conf->m_content);
			sender.AddConfigItem(conf);
			sender.Send(m_role->GetID());
		}
		m_lastLevel = level;
		SetSaveFlag();
	}
	CheckChargeBack();
}

void RoleMisc::CheckHardestNestExpID(UINT32 nestExpID)
{
	auto data = NestConfig::Instance()->GetNestListTableData(nestExpID);
	if (!data || data->Difficulty == 4)
	{
		return;
	}

	UINT32 curType = 0;
	UINT32 curDiff = 0;

	auto cur = NestConfig::Instance()->GetNestListTableData(m_hardestNestExpID);
	if (cur)
	{
		curType = cur->Type;
		curDiff = cur->Difficulty;
	}

	if (data->Type > curType || ( data->Type == curType && data->Difficulty > curDiff))
	{
		m_hardestNestExpID = nestExpID;
		SetSaveFlag();

		LogInfo("%llu, through expid:%u", m_role->GetID(), nestExpID);
	}
}

void RoleMisc::SetStartUpType(KKSG::StartUpType type)
{
	UINT32 now = GameTime::GetTime();
	if (type != KKSG::StartUp_Normal || !XCommon::IsDaySame(m_startUpTypeTime, now))
	{
		m_startUpType = type;
		m_startUpTypeTime = GameTime::GetTime();
		RoleSummaryMgr::Instance()->UpdateRoleSummary(m_role, SUMMARY_STARTUP);
		SetSaveFlag();
	}
	PtcG2C_NotifyStartUpTypeToClient ntf;
	ntf.m_Data.set_type(m_startUpType);
	m_role->Send(ntf);
}

KKSG::StartUpType RoleMisc::GetStartUpType()
{ 
	if (!m_role->Get<CRoleSystem>()->IsSystemOpened(SYS_PLATFORM_STARTPRIVILEGE))
	{
		return KKSG::StartUp_Normal;
	}
	return m_startUpType;
}

bool RoleMisc::CheckAndAddWeddingFlowerCount()
{
	if (m_weddingFlowerCount >= GetGlobalConfig().WeddingFlowerDailyMax)
	{
		return false;
	}
	m_weddingFlowerCount++;
	SetSaveFlag();
	return true;
}

bool RoleMisc::CheckAndAddWeddingFireworksCount()
{
	if (m_weddingFireworksCount >= GetGlobalConfig().WeddingFireworksDailyMax)
	{
		return false;
	}
	m_weddingFireworksCount++;
	SetSaveFlag();
	return true;
}

bool RoleMisc::CheckAndAddWeddingCandyCount()
{
	if (m_weddingCandyCount >= GetGlobalConfig().WeddingDoodadDailyMax)
	{
		return false;
	}
	m_weddingCandyCount++;
	SetSaveFlag();
	return true;
}

bool RoleMisc::AddGMAttr(CombatAttrDef attrDef, double value)
{
	if (GSConfig::Instance()->IsDebug())
	{
		if (AttributeDefInfo::IsBasicAttr(attrDef) || AttributeDefInfo::IsPercentAttr(attrDef))
		{
			SetSaveFlag();
			m_id2gmattr[attrDef] += value;
			return true;
		}
		else
		{
			SSWarn<<"role:"<<m_role->GetID()<<" add attr invalid id:"<<attrDef<<END;
			return false;
		}
	}
	else
	{
		SSWarn<<"role:"<<m_role->GetID()<<" add attr in release mode"<<END;
		return false;
	}
}

void RoleMisc::ClearGMAttr()
{
	SetSaveFlag();
	m_id2gmattr.clear();
}

void RoleMisc::ApplyGMAttr(CombatAttribute* attrs)
{
	if (GSConfig::Instance()->IsDebug())
	{
		for (auto i = m_id2gmattr.begin(); i != m_id2gmattr.end(); ++i)
		{
			attrs->AddAttr((CombatAttrDef)i->first, i->second);
		}
	}
}

void RoleMisc::ListGMAttr(std::string& out)
{
	std::stringstream ss;
	for (auto i = m_id2gmattr.begin(); i != m_id2gmattr.end(); ++i)
	{
		ss<<i->first<<":"<<i->second<<"\n";
	}
	out = ss.str();
}

void RoleMisc::InitLuckyActivity(const std::vector<KKSG::ItemRecord>& all)
{
	m_turntable.Clear();
	m_turntable.set_ispay(false);
	m_turntable.set_state(1);
	for (auto i = 0;i < all.size();i++)
	{
		KKSG::ItemRecord* info = m_turntable.add_itemrecord();
		info->set_itemid(all[i].itemid());
		info->set_itemcount(all[i].itemcount());
		info->set_isreceive(all[i].isreceive());
	}
	SetSaveFlag();
}

void RoleMisc::DeleteOldSpAct(UINT32 actid)
{
	if (actid == SpActivity_LuckyTurntable)
	{
		m_turntable.Clear();
		SetSaveFlag();
	}
}

const KKSG::LuckyActivity RoleMisc::GetLuckyActicityInfo()
{
	return m_turntable;
}

const UINT32 RoleMisc::GetPriceState()
{
	return m_turntable.state();
}

const bool RoleMisc::GetIsPay()
{
	return m_turntable.ispay();
}

void RoleMisc::UpdateIsPay(bool ispay)
{
	m_turntable.set_ispay(ispay);
	SetSaveFlag();
}

void RoleMisc::GetRoleItemRecord(std::vector<KKSG::ItemRecord>& tmp)
{
	for (int i = 0;i < m_turntable.itemrecord_size();i++)
	{
		tmp.push_back(m_turntable.itemrecord(i));
	}
}

void RoleMisc::UpdateAfterLottery(UINT32 index)
{
	m_turntable.set_state(m_turntable.state() + 1);
	m_turntable.set_ispay(false);
	m_turntable.mutable_itemrecord(index)->set_isreceive(true);
	SetSaveFlag();
}

bool RoleMisc::IsConfigModify()
{
	if (GetGlobalConfig().LotteryDrawItems.size() != m_turntable.itemrecord_size())
	{
		return true;
	}
	auto it = GetGlobalConfig().LotteryDrawItems.begin();
	for (int i = 0;i < m_turntable.itemrecord_size() && it != GetGlobalConfig().LotteryDrawItems.end();i++,it++)
	{
		if (it->seq[0] != m_turntable.itemrecord(i).itemid() || it->seq[1] != m_turntable.itemrecord(i).itemcount())
		{
			return true;
		}
	}
	return false;
}

UINT32 RoleMisc::SurviveGetPoint()
{
	SurviveUpdate();

	return m_surviveData.point();
}

void RoleMisc::SurviveAddPoint(int addp)
{
	SurviveUpdate();
	m_dirty.Set();
	int newPoint = int(m_surviveData.point()) + addp;
	if(newPoint < 0)
	{
		newPoint = 0;
	}
	m_surviveData.set_point(newPoint);
}

UINT32 RoleMisc::SurviveGetTopCount()
{
	SurviveUpdate();
	return m_surviveData.topcount();
}

void RoleMisc::SurviveAddTopCount()
{
	SurviveUpdate();
	m_dirty.Set();
	m_surviveData.set_topcount(m_surviveData.topcount() + 1);
}

bool RoleMisc::SurviveGetRewardFlag()
{
	SurviveUpdate();
	return m_surviveData.givereward();
}

void RoleMisc::SurviveSetRewardFlag()
{
	SurviveUpdate();
	m_dirty.Set();
	m_surviveData.set_givereward(true);
}

void RoleMisc::SurviveUpdate()
{
	UINT32 now = UINT32(GameTime::GetTime());
	if(!GameTime::IsInSameWeek(m_surviveData.lastweekuptime(), now, true))
	{
		m_dirty.Set();
		m_surviveData.set_lastweekuptime(now);

		m_surviveData.set_point(0);
		m_surviveData.set_topcount(0);
		m_surviveData.set_givereward(false);
	}
}

void RoleMisc::SurviveInfoPrint()
{
	SurviveUpdate();
	SSInfo << " lastuptime = " << m_surviveData.lastweekuptime() << " point = " << m_surviveData.point() << END;
	SSInfo << " topcount = " << m_surviveData.topcount() << " rewardflag = " << m_surviveData.givereward() << END;
}

void RoleMisc::SetFreeFlowHintTime(UINT32 t)
{
	m_freeHintTime = t;
	m_dirty.Set();
}

UINT32 RoleMisc::GetKingBackRewardCount()
{
	return m_kingBackReward;
}

void RoleMisc::AddKingBackRewardCount()
{
	m_kingBackReward ++;
	m_dirty.Set();
}

void RoleMisc::StartChargeBack(UINT32 payCnt)
{
	UINT32 backPay = payCnt * GetGlobalConfig().NewZoneChargeBackRate / 100;
	if (backPay >= GetGlobalConfig().NewZoneChargeBackMax)
	{
		backPay = GetGlobalConfig().NewZoneChargeBackMax;
	}
	m_chargeBackTotal = backPay;
	m_chargeBackRewardLevel = 0;
	LogInfo("Role[%llu] start charge back, total pay[%u], backpay[%u]", m_role->GetID(), payCnt, backPay);
	CheckChargeBack();
	m_dirty.Set();
}

void RoleMisc::CheckChargeBack()
{
	UINT32 now = GameTime::GetTime();
	if (now > GetGlobalConfig().NewZoneChargeBackRewardEndTime)
	{
		return ;
	}

	UINT32 level = m_role->GetLevel();
	auto& range = GetGlobalConfig().NewZoneChargeBackRange;
	for (auto it = range.begin(); it != range.end(); ++it)
	{
		if (level >= it->first && m_chargeBackRewardLevel < it->first)
		{
			m_chargeBackRewardLevel = it->first;
			m_dirty.Set();
			UINT32 num = m_chargeBackTotal * it->second / 100;
			if (num == 0)
			{
				continue;
			}

			UINT32 nextLevel = 0;
			if (++it != range.end())
			{
				nextLevel = it->first;
			}
			--it;

			if (nextLevel)
			{
				std::string dateStr = GameTime::GetDateStr(GetGlobalConfig().NewZoneChargeBackRewardEndTime);
				const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_ChargeBack);
				if (conf)
				{
					MailSender sender;
					std::string dest;
					dest = MailConfig::Instance()->ReplaceString("%L", ToString(it->first), conf->m_content);
					dest = MailConfig::Instance()->ReplaceString("%M", ToString(nextLevel), dest);
					dest = MailConfig::Instance()->ReplaceString("%d", dateStr, dest);
					dest = MailConfig::Instance()->ReplaceString("%s", ToString(num), dest);

					sender.SetContent(Mail_System, conf->m_title, dest);
					sender.SetReason(ItemFlow_BackFlow, ItemFlow_BackFlowChargeBack);
					sender.AddItem(ItemDesc(DIAMOND, num));
					sender.Send(m_role->GetID());
				}
			}
			else
			{
				const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_ChargeBackLast);
				if (conf)
				{
					MailSender sender;
					std::string dest;
					dest = MailConfig::Instance()->ReplaceString("%L", ToString(it->first), conf->m_content);
					dest = MailConfig::Instance()->ReplaceString("%s", ToString(num), dest);
					sender.SetContent(Mail_System, conf->m_title, dest);
					sender.SetReason(ItemFlow_BackFlow, ItemFlow_BackFlowChargeBack);
					sender.AddItem(ItemDesc(DIAMOND, num));
					sender.Send(m_role->GetID());
				}
			}
		}
	}
}
