#include "pch.h"
#include "guildrecord.h"
#include "pb/project.pb.h"
#include "util/XCommon.h"
#include "util/XRandom.h"
#include "guild/guildcardmgr.h"
#include "gamelogic/itemsuffix.h"
#include "table/XSkillEffectMgr.h"
#include "unit/role.h"
#include "gamelogic/bagtransition.h"
#include "guild/rpcg2m_guildcardsyncms.h"
#include "network/mslink.h"
#include "gamelogic/noticemgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildrecord.h"
#include "define/systemiddef.h"

CGuildRecord::CGuildRecord(Role* role)
{
	m_pRole = role;
	m_guildid = 0;
	m_updateday = 0;
	m_ishintcard = false;
	m_cardplaycount = 0;
	m_cardchangecount = 0;
	m_cardbuychangecount = 0;
	m_recvFatigue = 0;
	m_checkin = 0;
	m_boxmask = 0;
	//m_askBonusTime = 0;
	//m_getCheckInBonusNum = 0;
	m_cardmatchid = 0;
	m_guildInheritTeaTime = 0;
	m_guildInheritStuTime = 0;
	m_guildInheritCDTime = 0;
	m_teacherInheritTime = 0;
}

CGuildRecord::~CGuildRecord()
{

}

bool CGuildRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_guildrecord())
	{
		const KKSG::GuildRecord* record = &poRoleAllInfo->guildrecord();
		m_cardplaycount = record->cardplaycount();
		m_cardchangecount = record->cardchangecount();
		m_cardbuychangecount = record->cardbuychangecount();
		m_updateday = record->updateday();
		m_checkin = record->checkin();
		m_boxmask = record->boxmask();
		m_recvFatigue = record->recvfatigue();
		m_ishintcard = record->ishintcard();
		m_cardmatchid = record->cardmatchid();
		m_guildInheritStuTime = record->inheritstutime();
		m_guildInheritTeaTime = record->inheritteatime();
		m_guildInheritCDTime = record->guildinheritcdtime();
		//m_guildid = record->guildid();
		for (UINT32 i = 0; i < record->darereward_size(); ++i)
		{
			m_dareReward.insert(record->darereward(i));
		}
		for (UINT32 cnt = 0; cnt < record->guildskills_size(); ++cnt)
		{
			m_guildSkillMap[record->guildskills(cnt).skillid()] = record->guildskills(cnt).skilllvl();
		}

		m_guildBonusData.Load(record);

		// 工会营地奖励
		for (UINT32 i = 0; i < record->partyreward_size(); ++i)
		{
			auto& reward =  record->partyreward(i);
			party_rewards_.insert(std::make_pair(reward.key(),reward.value()));
		}
		Update();
	}
	return true;
}

void CGuildRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_switch.TestAndReset())
	{
		KKSG::GuildRecord* record = poRoleAllInfo->mutable_guildrecord();
		Convert2_KKSGGuildRecord(record);

		roChanged.insert(record);
	}
}

void CGuildRecord::Convert2_KKSGGuildRecord(KKSG::GuildRecord* record)
{
	if (!record)
	{
		return;
	}
	record->Clear();
	record->set_cardplaycount(m_cardplaycount);
	record->set_cardchangecount(m_cardchangecount);
	record->set_cardbuychangecount(m_cardbuychangecount);
	record->set_updateday(m_updateday);
	record->set_checkin(m_checkin);
	record->set_boxmask(m_boxmask);
	record->set_recvfatigue(m_recvFatigue);
	record->set_ishintcard(m_ishintcard);
	record->set_cardmatchid(m_cardmatchid);
	record->set_inheritstutime(m_guildInheritStuTime);
	record->set_inheritteatime(m_guildInheritTeaTime);
	record->set_guildinheritcdtime(m_guildInheritCDTime);
	record->set_teacherinherittime(m_teacherInheritTime);
	//record->set_guildid(m_guildid);
	for (auto iter = m_dareReward.begin(); iter != m_dareReward.end(); ++iter)
	{
		record->add_darereward(*iter);
	}
	for (auto iter = m_guildSkillMap.begin(); iter != m_guildSkillMap.end(); ++iter)
	{
		KKSG::GuildSkill* pGuildSkill = record->add_guildskills();
		pGuildSkill->set_skillid(iter->first);
		pGuildSkill->set_skilllvl(iter->second);
	}

	m_guildBonusData.Save(record);
	// 保存领奖的信息
	for (auto it = party_rewards_.begin(); it != party_rewards_.end(); ++it)
	{
		KKSG::MapKeyValue* reward = record->add_partyreward();
		reward->set_key(it->first);
		reward->set_value(it->second);
	}
}

void CGuildRecord::Reset()
{
	m_cardstore.clear();
	m_cards.clear();
}

void CGuildRecord::Update()
{
	UINT32 day = XCommon::GetTodayUnique();
	if (m_updateday != day)
	{
		m_ishintcard = false; 
		m_cardplaycount = 0;
		m_cardchangecount = 0;
		m_cardbuychangecount = 0;
		m_recvFatigue = 0;
		m_checkin = 0;
		m_boxmask = 0;
		m_cardmatchid = 0;
		m_guildInheritTeaTime = 0;
		m_guildInheritStuTime = 0;
		m_dareReward.clear();
		m_guildBonusData.OnDayPass();

		m_updateday = day;
		party_rewards_.clear();
		m_switch.Set();
	}
}

UINT32 CGuildRecord::RandomStore()
{
	Reset();
	return GuildCardMgr::Instance()->RandomStore(m_cards, m_cardstore);
}

UINT32 CGuildRecord::ChangeCard(UINT32 card)
{
	return GuildCardMgr::Instance()->ChangeCard(card, m_cards, m_cardstore);
}

void CGuildRecord::AddPlayCount()
{
	++m_cardplaycount;
	m_switch.Set();
}

void CGuildRecord::AddChangeCount()
{
	++m_cardchangecount;
	m_switch.Set();
}

void CGuildRecord::AddBuyChangeCount()
{
	++m_cardbuychangecount;
	m_switch.Set();
}

UINT32 CGuildRecord::AddGetBonusNum(UINT32 bonusTemplateID, UINT32 num)
{
	auto data = GuildConfig::Instance()->GetBonusTemplateInfo(bonusTemplateID);
	if (!data)
	{
		return KKSG::ERR_FAILED;
	}

	UINT32 error = KKSG::ERR_SUCCESS;

	error = CheckAddGetBonusNum(data, bonusTemplateID, num);

	if (error != KKSG::ERR_SUCCESS)
	{
		return error;
	}
	
	if (data->DayMaxNum)
	{
		m_guildBonusData.m_gotGuildBonusDayNum[bonusTemplateID] += num;
	}
	if (data->ForeverMaxNum)
	{
		m_guildBonusData.m_gotGuildBonusTotalNum[bonusTemplateID] += num;
	}

	m_switch.Set();

	return KKSG::ERR_SUCCESS;
}

UINT32 CGuildRecord::CheckAddGetBonusNum(const GuildBonusTable::RowData* data, UINT32 bonusTemplateID, UINT32 num)
{
	if (!data)
	{
		return KKSG::ERR_FAILED;
	}

	if (!data->DayMaxNum && !data->ForeverMaxNum)
	{
		return KKSG::ERR_SUCCESS;
	}

	UINT32 getNum = 0;
	auto find = m_guildBonusData.m_gotGuildBonusDayNum.find(bonusTemplateID);
	if (data->DayMaxNum && find != m_guildBonusData.m_gotGuildBonusDayNum.end())
	{
		getNum = find->second;
		if (getNum >= data->DayMaxNum)
		{
			LogError("%llu, %s, get bonus template:%u too fast", m_pRole->GetRoleID(), m_pRole->GetName().c_str(), bonusTemplateID);
			return KKSG::ERR_GUILDBONUS_ALREADYGET;
		}
	}
	auto findTotal = m_guildBonusData.m_gotGuildBonusTotalNum.find(bonusTemplateID);
	if (data->ForeverMaxNum && findTotal != m_guildBonusData.m_gotGuildBonusTotalNum.end())
	{
		getNum = findTotal->second;
		if (getNum >= data->ForeverMaxNum)
		{
			return KKSG::ERR_GUILDBONUS_ALREADYGET;
		}
	}

	return KKSG::ERR_SUCCESS;
}

bool CGuildRecord::IsCountLimit(UINT32 count)
{
	return m_cardplaycount >= count;
}

bool CGuildRecord::IsChangeLimit(UINT32 count)
{
	return m_cardchangecount >= count;
}

bool CGuildRecord::IsCheckin()
{
	return m_checkin > 0;
}

bool CGuildRecord::IsBoxTaken(UINT32 index)
{
	return (m_boxmask & (1 << index)) > 0;
}

void CGuildRecord::SetCheckin(UINT32 checkin)
{ 
	m_checkin = checkin;
	m_switch.Set();
}

void CGuildRecord::SetBoxMask(UINT32 index)
{ 
	m_boxmask |= (1 << index); 
	m_switch.Set();
}

void CGuildRecord::AddRecvFatigue()
{
	++m_recvFatigue;
	m_switch.Set();
}

void CGuildRecord::AddRecvFatigue(UINT32 addNum)
{
	m_recvFatigue += addNum;
	m_switch.Set();
}

void CGuildRecord::AddDareReward(UINT32 rewardType)
{
	m_dareReward.insert(rewardType);
	m_switch.Set();
}

/*
void CGuildRecord::ClearDareRewardTimes()
{
m_dareReward.clear();
m_switch.Set();
}
*/

void CGuildRecord::ApplyEffect(CombatAttribute *combatAttr)
{
	std::vector<ItemChangeAttr> attrsVec;
	XSkillEffectMgr::Instance()->GetGuildSkillAttr(m_guildSkillMap, attrsVec);
	ApplyEffectAux(attrsVec, combatAttr);
}

UINT32 CGuildRecord::EndCard(Role* role, UINT64 guildid)
{
	if (m_cards.empty())
	{
		return CardDummy;
	}
	UINT32 result = GuildCardMgr::Instance()->GetResult(GetCards());

	std::vector<ItemDesc> items;
	GuildCardMgr::Instance()->GetReward(result, items);

	BagGiveItemTransition transition(role);
	transition.SetReason(ItemFlow_Guild, ItemFlow_Guild_Card);
	for (auto i = items.begin(); i != items.end(); ++i)
	{
		transition.GiveItem(*i);
	}
	transition.NotifyClient();

	NoticeMgr::Instance()->GuildCardNotice(role, guildid, result, items);

	RpcG2M_GuildCardSyncMs* rpc = RpcG2M_GuildCardSyncMs::CreateRpc();
	rpc->m_oArg.set_roleid(role->GetID());
	rpc->m_oArg.set_op(GuildCard_End);
	rpc->m_oArg.set_result(result);
	for (auto i = m_cards.begin(); i != m_cards.end(); ++i)
	{
		rpc->m_oArg.add_cards(*i);
	}
	MSLink::Instance()->SendTo(*rpc);

	Reset();

	return result;
}

void CGuildRecord::HintNotify(Role* pRole)
{
	CGuildSimple* guild = GuildSimpleMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (NULL == guild)
	{
		return;
	}

	if (!pRole->Get<CGuildRecord>()->IsCheckin())
	{
		pRole->HintNotify(SYS_GUILD_CHECKIN, false);
	}
}

bool CGuildRecord::CheckSendGuildBonus(UINT32 bonusType, UINT32 bonusVar)
{
	if (bonusType != KKSG::GBONUS_DRAGONNEST_SCENE && bonusType != KKSG::GBONUS_DRAGONJADEL_ALLLEVEL)
	{
		return true;
	}
	SOnlyOnceGuildBonusData bonusData;
	bonusData.bonusType = bonusType;
	bonusData.bonusVar = bonusVar;

	auto find = m_guildBonusData.m_onceBonusData.find(bonusData.GetKey());
	if (find != m_guildBonusData.m_onceBonusData.end())
	{
		return false;
	}

	m_guildBonusData.m_onceBonusData.insert(std::make_pair(bonusData.GetKey(), bonusData));

	m_switch.Set();

	return true;
}

void CGuildRecord::DebugResetGuildBonus()
{
	m_guildBonusData.m_onceBonusData.clear();
	m_guildBonusData.m_gotGuildBonusDayNum.clear();
	m_guildBonusData.m_gotGuildBonusTotalNum.clear();
	m_switch.Set();
}

void CGuildRecord::ResetCardCount()
{
	m_cardplaycount = 0;
	m_cardchangecount = 0;
	m_cardbuychangecount = 0;
}


UINT32 CGuildRecord::GetPartyRewardCount(UINT32 reward_id)
{
	auto it = party_rewards_.find(reward_id);
	if (it != party_rewards_.end())
	{
		return it->second;
	}
	return 0;
}

void CGuildRecord::AddPartyRewardCount(UINT32 reward_id)
{
	auto it = party_rewards_.find(reward_id);
	if (it != party_rewards_.end())
	{
		it->second += 1;
	}
	else
	{
		party_rewards_.insert(std::make_pair(reward_id,1));
	}
	m_switch.Set();
}

void CGuildRecord::GetPartyRewardInfo(std::map<UINT32,UINT32>& reward_info)
{
	// 获取领奖的信息
	for (auto it = party_rewards_.begin(); it != party_rewards_.end(); ++it)
	{		
		reward_info.insert(std::make_pair(it->first,it->second));
	}
}
