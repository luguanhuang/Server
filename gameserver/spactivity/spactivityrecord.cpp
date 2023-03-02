#include "pch.h"
#include "spactivityrecord.h"
#include "spactivitymgr.h"
#include "spactivityhandler.h"
#include "unit/role.h"
#include "spactivitybase.h"
#include "spactivitydef.h"
#include "spactivitydatabase.h"
#include "spactivitydatasmall.h"
#include "util/gametime.h"
#include "util/XCommon.h"
#include "define/spactivityiddef.h"
#include "spactivityconfig.h"
#include "define/maildef.h"
#include "mail/mailsender.h"
#include "define/tlogenum.h"
#include "expeditionconfigmgr.h"
#include "mail/mailconfig.h"
#include "gamelogic/taskrecord.h"
#include "gamelogic/rolemisc.h"


SpActivityDataBase* GetNewDataBase(Role* pRole, UINT32 actid)
{
	switch (actid)
	{
	case SpActivity_OpenServer:
		{
			return new SpActOpenServerData(pRole, actid);
		}
	case SpActivity_BackFlow:
		{
			return new SpActBackFlowData(pRole, actid);
		}
	case SpActivity_Argenta:
		{
			return new SpActArgentaData(pRole, actid);
		}
	case SpActivity_AncientTimes:
		{
			return new SpActAncientData(pRole, actid);
		}
	case SpActivity_BioHell:
	case SpActivity_Duck:
		{
			return new SpActThemeData(pRole, actid);
		}
	default:
		{
			return new SpActivityDataBase(pRole, actid);
		}
	}
}

SpActivityRecord::SpActivityRecord(Role* pRole)
:m_pRole(pRole)
{
	m_lastBackFlowStartTime = 0;
	m_lastArgentStartTime = 0;
}

SpActivityRecord::~SpActivityRecord()
{
	for (auto i = m_actid2Data.begin(); i != m_actid2Data.end(); i++)
	{
		delete i->second;
	}
	m_actid2Data.clear();
}

bool SpActivityRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (false == poRoleAllInfo->has_spactivityrecord())
	{
		UpdateArgentaPreData();
		UpdateBackFlowPreData(true);
		SpActivityMgr::Instance()->UpdateSpActivityData(m_pRole);	
		return true;
	}
	const KKSG::SpActivity& spAct = poRoleAllInfo->spactivityrecord();

	for (int i = 0; i < spAct.spactivity_size(); i++)
	{
		UINT32 actid = spAct.spactivity(i).actid();
		if (GSConfig::Instance()->IsHallGS() && SpActivityConfig::Instance()->GetTimeConf(actid) == NULL)
		{
			LogWarn("Role [%llu], actid [%u] can't find in SuperActivityTime.txt, erase it", m_pRole->GetID(), actid);
			continue;
		}
		if (m_actid2Data.find(actid) == m_actid2Data.end())
		{
			m_actid2Data.insert(std::make_pair(actid, GetNewDataBase(m_pRole, actid)));
		}
		m_actid2Data[actid]->Load(spAct.spactivity(i));
	}

	m_lastBackFlowStartTime = spAct.lastbackflowstarttime();
	m_lastArgentStartTime = spAct.lastargentastarttime();
	m_argentaPreData = spAct.argentapredata();
	m_backflowPreData = spAct.backflowpredata();

	UpdateArgentaPreData();
	UpdateBackFlowPreData(true);
	///> 运行完这个，actid2data里的actid是当前正在开的活动，task会刷新成表中的task（表如果修改了）
	SpActivityMgr::Instance()->UpdateSpActivityData(m_pRole);

	return true;
}

void SpActivityRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_isModify.TestAndReset())
	{
		KKSG::SpActivity* spAct = poRoleAllInfo->mutable_spactivityrecord();
		spAct->Clear();

		for(auto i = m_actid2Data.begin(); i != m_actid2Data.end(); i++)
		{
			KKSG::SpActivityOne* spActOne = spAct->add_spactivity();
			i->second->Save(*spActOne);
		}

		spAct->set_lastbackflowstarttime(m_lastBackFlowStartTime);
		spAct->set_lastargentastarttime(m_lastArgentStartTime);
		spAct->mutable_argentapredata()->CopyFrom(m_argentaPreData);
		spAct->mutable_backflowpredata()->CopyFrom(m_backflowPreData);

		roChanged.insert(spAct);
	}
}

void SpActivityRecord::UpdatePerMinute()
{
	if (!GSConfig::Instance()->IsHallGS())
	{
		return;
	}
	const std::map<UINT32, SpActivityBase*>& nowActMap = SpActivityMgr::Instance()->GetAllSpActivity();
	for (auto i = nowActMap.begin(); i != nowActMap.end(); i++)
	{
		///> 用于活动关闭是跟人物自身相关的
		if (!i->second->CheckOpenCondition(m_pRole) && i->first != SpActivity_OpenServer)
		{
			DeleteOldSpAct(i->first);
		}
	}
	SpActivityMgr::Instance()->CloseNotOpenSpActSystem(m_pRole);

	for (auto i = m_actid2Data.begin(); i != m_actid2Data.end(); i++)
	{
		i->second->UpdatePerMinute();
	}
}

void SpActivityRecord::OnFirstEnterScene()
{
	if (!GSConfig::Instance()->IsHallGS())
	{
		return;
	}

	for (auto i = m_actid2Data.begin(); i != m_actid2Data.end(); i++)
	{
		i->second->OnFirstEnterScene();
	}	
}

SpActivityDataBase* SpActivityRecord::GetSpActData(UINT32 actid)
{
	auto iter = m_actid2Data.find(actid);
	if (iter == m_actid2Data.end())
	{
		return NULL;
	}
	return iter->second;
}

SpActivityDataBase* SpActivityRecord::AddAndGetSpActData(UINT32 actid, UINT32 startTime, UINT32 stage)
{
	auto iter = m_actid2Data.find(actid);
	if (iter == m_actid2Data.end())
	{
		m_actid2Data.insert(std::make_pair(actid, GetNewDataBase(m_pRole, actid)));
		_OnSpActivityOpen(m_actid2Data[actid]);
	}
	return m_actid2Data[actid];
}

void SpActivityRecord::AddOrUpdateSpActivityTask(UINT32 actid, UINT32 startTime, UINT32 stage, const std::vector<SpActivityTaskConf*>* allTask)
{
	if (m_actid2Data.find(actid) == m_actid2Data.end())
	{
		m_actid2Data.insert(std::make_pair(actid, GetNewDataBase(m_pRole, actid)));
		_OnSpActivityOpen(m_actid2Data[actid]);
	}

	SpActivityDataBase* pDataBase = m_actid2Data[actid];
	pDataBase->UpdateSpActivityTask(startTime, stage, allTask);

	m_isModify.Set();
}

void SpActivityRecord::DeleteOldSpAct(UINT32 actid)
{
	auto iter = m_actid2Data.find(actid);
	if (iter == m_actid2Data.end())
	{
		return;
	}

	_OnSpActivityClose(iter->second);

	delete iter->second;
	m_actid2Data.erase(iter);

	m_isModify.Set();
}

void SpActivityRecord::OnIncreaseProgress(const std::vector<SpActivityTaskConf*>& tasks, UINT32 count)
{
	for(auto i = tasks.begin(); i != tasks.end(); i++)
	{
		auto iter = m_actid2Data.find((*i)->actid);
		if (iter != m_actid2Data.end())
		{
			iter->second->IncreaseProgress(*i, count);
		}
	}
}

void SpActivityRecord::OnDecreaseProgress(const std::vector<SpActivityTaskConf*>& tasks, UINT32 count)
{
	for(auto i = tasks.begin(); i != tasks.end(); i++)
	{
		auto iter = m_actid2Data.find((*i)->actid);
		if (iter != m_actid2Data.end())
		{
			iter->second->DecreaseProgress(*i, count);
		}
	}
}

UINT32 SpActivityRecord::GetSpActStartTime(UINT32 actid) const
{
	auto iter = m_actid2Data.find(actid);
	if (iter == m_actid2Data.end())
	{
		return 0;
	}
	return iter->second->GetStartTime();
}

UINT32 SpActivityRecord::GetSpActStage(UINT32 actid) const
{
	auto iter = m_actid2Data.find(actid);
	if (iter == m_actid2Data.end())
	{
		return 0;
	}
	return iter->second->GetActStage();
}

void SpActivityRecord::AddSpActStage(UINT32 actid)
{
	auto iter = m_actid2Data.find(actid);
	if (iter == m_actid2Data.end())
	{
		return;
	}
	iter->second->AddActStage();
}

void SpActivityRecord::ResetSpActTask(UINT32 actid)
{
	auto iter = m_actid2Data.find(actid);
	if (iter == m_actid2Data.end())
	{
		return;
	}
	iter->second->ResetTask();
}

KKSG::ErrorCode SpActivityRecord::GetReward(UINT32 actid, UINT32 taskid, const std::vector<Sequence<UINT32, 2>>& reward, int subreason)
{
	auto iter = m_actid2Data.find(actid);
	if (iter == m_actid2Data.end())
	{
		return KKSG::ERR_UNKNOWN;
	}
	return iter->second->GetReward(taskid, reward, subreason);
}

void SpActivityRecord::UpdateArgentaPreData()
{	
	UINT32 nowTime = (UINT32)GameTime::GetTime();

	if (m_argentaPreData.lastupdatetime() == 0 || m_argentaPreData.lastupdatetime() > nowTime)
	{
		m_argentaPreData.set_lastupdatetime(nowTime);
		m_argentaPreData.clear_activitypoint();
		m_argentaPreData.add_activitypoint(0);
		m_argentaPreData.clear_finishnestcount();
		m_argentaPreData.add_finishnestcount(0);
		m_isModify.Set();
		return;
	}
	UINT32 offsetDay = XCommon::GetDiffDayCount(m_argentaPreData.lastupdatetime(), nowTime);
	if (offsetDay == 0)
	{
		m_argentaPreData.set_lastupdatetime(nowTime);
		m_isModify.Set();
		return;
	}

	m_argentaPreData.set_lastupdatetime(nowTime);

	std::vector<UINT32> vec;
	vec.clear();
	for (UINT32 i = 0; i < offsetDay && i < GetGlobalConfig().ArgentaWithinDays + 1; i++)
	{
		vec.push_back(0);
	}
	for (int i = 0; i < m_argentaPreData.activitypoint_size(); i++)
	{
		if (i + offsetDay >= GetGlobalConfig().ArgentaWithinDays + 1)
		{
			break;
		}
		vec.push_back(m_argentaPreData.activitypoint(i));
	}
	m_argentaPreData.clear_activitypoint();
	for (auto i = vec.begin(); i != vec.end(); i++)
	{
		m_argentaPreData.add_activitypoint(*i);
	}

	vec.clear();
	for (UINT32 i = 0; i < offsetDay && i < GetGlobalConfig().ArgentaWithinDays + 1; i++)
	{
		vec.push_back(0);
	}
	for (int i = 0; i < m_argentaPreData.finishnestcount_size(); i++)
	{
		if (i + offsetDay >= GetGlobalConfig().ArgentaWithinDays + 1)
		{
			break;
		}
		vec.push_back(m_argentaPreData.finishnestcount(i));
	}
	m_argentaPreData.clear_finishnestcount();
	for (auto i = vec.begin(); i != vec.end(); i++)
	{
		m_argentaPreData.add_finishnestcount(*i);
	}

	m_isModify.Set();
}

void SpActivityRecord::OnAddActivityPoint(UINT32 point)
{
	UpdateArgentaPreData();

	if (m_argentaPreData.activitypoint_size() < 1)
	{
		m_argentaPreData.add_activitypoint(point);
	}
	else
	{
		m_argentaPreData.set_activitypoint(0, m_argentaPreData.activitypoint(0) + point);
	}

	m_isModify.Set();
}

void SpActivityRecord::OnFinishNest()
{
	UpdateArgentaPreData();

	if (m_argentaPreData.finishnestcount_size() < 1)
	{
		m_argentaPreData.add_finishnestcount(1);
	}
	else
	{
		m_argentaPreData.set_finishnestcount(0, m_argentaPreData.finishnestcount(0) + 1);
	}

	m_isModify.Set();
}

bool SpActivityRecord::CheckArgentaOpen()
{
	if (m_actid2Data.find(SpActivity_BackFlow) != m_actid2Data.end())
	{
		return false;
	}

	UpdateArgentaPreData();

	UINT32 lastOfflineTime = m_pRole->GetRoleAllInfo().brief().offlinetime();
	if (lastOfflineTime != 0 && lastOfflineTime + GetGlobalConfig().ArgentaWithinDays * 24 * 3600 <= GameTime::GetTime())
	{
		return true;
	}

	bool result1 = false, result2 = false;
	if (m_argentaPreData.activitypoint_size() >= GetGlobalConfig().ArgentaWithinDays + 1)
	{
		UINT32 sum = 0;
		for (int i = 1; i <= (int)GetGlobalConfig().ArgentaWithinDays; i++)
		{
			sum += m_argentaPreData.activitypoint(i);
		}
		if (sum <= GetGlobalConfig().ArgentaActivityPoint)
		{
			result1 = true;
		}
	}

	if (m_argentaPreData.finishnestcount_size() >= GetGlobalConfig().ArgentaWithinDays + 1)
	{
		UINT32 sum = 0;
		for (int i = 1; i <= (int)GetGlobalConfig().ArgentaWithinDays; i++)
		{
			sum += m_argentaPreData.finishnestcount(i);
		}
		if (sum <= GetGlobalConfig().ArgentaNestCount)
		{
			result2 = true;
		}
	}
	return result1 && result2;
}

void SpActivityRecord::GMPrintArgentaPreData(std::string& data)
{
	UpdateArgentaPreData();

	data += "lastUpdateTime:" + ToString(m_argentaPreData.lastupdatetime()) + "\n";
	data += "activityPoint:";
	for (int i = 0; i < m_argentaPreData.activitypoint_size(); i++)
	{
		data += ToString(m_argentaPreData.activitypoint(i)) + " ";
	}
	data += "\n";
	data += "finishNestCount:";
	for (int i = 0; i < m_argentaPreData.finishnestcount_size(); i++)
	{
		data += ToString(m_argentaPreData.finishnestcount(i)) + " ";
	}
}

void SpActivityRecord::UpdateBackFlowPreData(bool isLogin)
{
	UINT32 nowTime = (UINT32)GameTime::GetTime();
	if (isLogin)
	{
		UINT32 lastOfflineTime = m_pRole->GetRoleAllInfo().brief().offlinetime();
		if (lastOfflineTime != 0 && m_backflowPreData.lastupdatetime() == 0)
		{
			m_backflowPreData.set_lastupdatetime(nowTime);
			UINT32 offsetDay = XCommon::GetDiffDayCount(lastOfflineTime, nowTime);
			m_backflowPreData.clear_isonline();
			m_backflowPreData.add_isonline(true);
			if (offsetDay > 0)
			{
				for (UINT32 i = 0; i < offsetDay - 1 && i < GetGlobalConfig().BackFlowJudgeDayTime; ++i)
				{
					m_backflowPreData.add_isonline(false);
				}
			}
			if (offsetDay != 0 && m_backflowPreData.isonline_size() <= GetGlobalConfig().BackFlowJudgeDayTime)
			{
				m_backflowPreData.add_isonline(true);
			}
			m_isModify.Set();
			return;
		}
	}
	if (nowTime < m_backflowPreData.lastupdatetime() || m_backflowPreData.lastupdatetime() == 0)
	{
		m_backflowPreData.set_lastupdatetime(nowTime);
		m_backflowPreData.clear_isonline();
		m_backflowPreData.add_isonline(true);
		m_isModify.Set();
		return;
	}
	UINT32 offsetDay = XCommon::GetDiffDayCount(m_backflowPreData.lastupdatetime(), nowTime);
	if (offsetDay == 0)
	{
		return;
	}

	m_backflowPreData.set_lastupdatetime(nowTime);

	std::deque<bool> vec;
	vec.push_back(true);
	for (UINT32 i = 0; i < offsetDay - 1 && i < GetGlobalConfig().BackFlowJudgeDayTime; ++i)
	{
		vec.push_back(!isLogin);
	}
	for (int i = 0; i < m_backflowPreData.isonline_size(); ++i)
	{
		if (i + offsetDay >= GetGlobalConfig().BackFlowJudgeDayTime + 1)
		{
			break;
		}
		vec.push_back(m_backflowPreData.isonline(i));
	}
	m_backflowPreData.clear_isonline();
	for (auto i = vec.begin(); i != vec.end(); ++i)
	{
		m_backflowPreData.add_isonline(*i);
	}

	m_isModify.Set();
}

bool SpActivityRecord::CheckBackFlowOpen()
{
	UpdateBackFlowPreData();

	UINT32 nowTime = (UINT32)GameTime::GetTime();
	UINT32 lastOfflineTime = m_pRole->GetRoleAllInfo().brief().offlinetime();
	if (lastOfflineTime == 0) ///> 新号第一次创建
	{
		return false;
	}
	if (lastOfflineTime + GetGlobalConfig().BackFlowNoLoginDayTime * 24 * 3600 <= nowTime)
	{
		return true;
	}
	if (m_backflowPreData.isonline_size() != GetGlobalConfig().BackFlowJudgeDayTime + 1)
	{
		return false;
	}
	UINT32 loginDayNum = 0;
	for (int i = 0; i < m_backflowPreData.isonline_size(); ++i)
	{
		loginDayNum += m_backflowPreData.isonline(i) == true ? 1 : 0;
	}
	return loginDayNum <= GetGlobalConfig().BackFlowJudgeMaxDayTime;
}

void SpActivityRecord::GMPrintBackFlowPreData(std::string& data)
{
	UpdateBackFlowPreData();

	data += "lastUpdateTime:" + ToString(m_backflowPreData.lastupdatetime()) + "\n";
	data += "LoginDay:";
	for (int i = 0; i < m_backflowPreData.isonline_size(); i++)
	{
		data += ToString(m_backflowPreData.isonline(i)) + " ";
	}
}

void SpActivityRecord::CheckSendTickets()
{
	auto it = GetSpActData(SpActivity_Duck);
	if (NULL == it)
	{
		return;
	}
	const SpActivityTimeConf* pConf = SpActivityConfig::Instance()->GetTimeConf(SpActivity_Duck);
	if (NULL == pConf)
	{
		return;
	}
	if (pConf->minlevel == m_pRole->GetLevel())
	{
		std::string strMailTitle;
		std::string strMailContent;
		const MailConf* Conf = MailConfig::Instance()->GetMailConf(MailConf_DuckTicketsAward);
		if (Conf)
		{
			strMailTitle = Conf->m_title;
			strMailContent = Conf->m_content;
		}
		std::vector<ItemDesc> rewards;
		UINT32 itemcount = GetGlobalConfig().DayDuckTickets.seq[1];
		UINT32 nWeek =  GameTime::GetWeekDay();
		if (nWeek == 6 || nWeek == 7)
		{
			itemcount *= 2;
		}
		ItemDesc item(GetGlobalConfig().DayDuckTickets.seq[0], itemcount);
		rewards.push_back(item);
		MailSender sender;
		sender.SetContent(Mail_System, strMailTitle, strMailContent);
		sender.SetItems(rewards);
		sender.SetReason(ItemFlow_DuckDayTickets);
		sender.Send(m_pRole->GetID());
	}
}

bool SpActivityRecord::CheckBioHellOpen(ExpeditionTable::RowData* pRow)
{
	UINT32 stage = GetSpActStage(SpActivity_BioHell);
	if (!stage)
	{
		return false;
	}
	auto& info = GetGlobalConfig().BioHellSceneStage;
	if (info.size() < stage)
	{
		return false;
	}
	UINT32 sceneid = ExpeditionConfigMgr::Instance()->GetBioHellScene(pRow);
	if (sceneid && info[stage-1] == sceneid)
	{
		return true;
	}
	return false;
}

bool SpActivityRecord::CheckIsFirstCompleteScene(UINT32 actid, UINT32 scenetype, UINT32 sceneid)
{
	if (actid != SpActivity_BioHell)
	{
		SSWarn << "invalid roleid=" << m_pRole->GetID()  << ",actid=" << actid << END;
		return false;
	}
	SpActivityDataBase* pBase = GetSpActData(actid);
	if (NULL == pBase)
	{
		return false;
	}

	SpActThemeData* pThemeData = static_cast<SpActThemeData*>(pBase);
	return pThemeData->IsFirstCompleteScene(scenetype, sceneid);
}

void SpActivityRecord::UpdateFirstCompleteScene(UINT32 actid, UINT32 scenetype, UINT32 sceneid)
{
	if (actid != SpActivity_BioHell)
	{
		SSWarn << "invalid roleid=" << m_pRole->GetID()  << ",actid=" << actid << END;
		return ;
	}
	SpActivityDataBase* pBase = GetSpActData(actid);
	if (NULL == pBase)
	{
		return;
	}

	SpActThemeData* pThemeData = static_cast<SpActThemeData*>(pBase);
	pThemeData->SetFirstCompleteScene(scenetype, sceneid);
}

void SpActivityRecord::GMClear()
{
	for(auto i = m_actid2Data.begin(); i != m_actid2Data.end(); i++)
	{
		i->second->GMClear();
	}
}

void SpActivityRecord::ResetAncientTimesAward()
{
	SpActAncientData* pAncientData = static_cast<SpActAncientData*>(GetSpActData(SpActivity_AncientTimes));
	if(pAncientData)
	{
		pAncientData->SetAward(0);
	}
}

void SpActivityRecord::GMCompleteAll()
{
	for(auto i = m_actid2Data.begin(); i != m_actid2Data.end(); i++)
	{
		i->second->GMCompleteAll();
	}
}

void SpActivityRecord::_OnSpActivityOpen(SpActivityDataBase* base)
{
	LogInfo("role [%llu] start spactivity actid [%u]", m_pRole->GetID(), base->GetActID());
	base->OnSpActivityOpen();

	m_pRole->Get<TaskRecord>()->OnActivityOpen(base->GetActID());

	///> 没有继承SpActivityDataBase的可以写在下面
	switch (base->GetActID())
	{
	case SpActivity_BackFlow:
		{
			m_pRole->OnTakeBackFlow();
			break;
		}
	case SpActivity_LuckyTurntable:
		{
			_LotteryDraw(base->GetActID());
			break;
		}
	default:
		{
			break;
		}
	}
}

void SpActivityRecord::_OnSpActivityClose(SpActivityDataBase* base)
{
	LogInfo("role [%llu] close spactivity actid [%u]", m_pRole->GetID(), base->GetActID());
	base->OnSpActivityClose();

	m_pRole->Get<TaskRecord>()->OnActivityClose(base->GetActID());

	if (base->GetActID() == SpActivity_LuckyTurntable)
	{
		m_pRole->Get<RoleMisc>()->DeleteOldSpAct(base->GetActID());
	}
}

void SpActivityRecord::_LotteryDraw(UINT32 m_actid)
{
	if (m_actid != SpActivity_LuckyTurntable)
	{
		return;
	}
	std::vector<KKSG::ItemRecord> all;
	for (auto it = GetGlobalConfig().LotteryDrawItems.begin(); it != GetGlobalConfig().LotteryDrawItems.end(); it++)
	{
		KKSG::ItemRecord itemrecord;
		itemrecord.set_itemid(it->seq[0]);
		itemrecord.set_itemcount(it->seq[1]);
		itemrecord.set_isreceive(false);
		all.push_back(itemrecord);
	}
	m_pRole->Get<RoleMisc>()->InitLuckyActivity(all);
	const SpActivityTimeConf* conf = SpActivityConfig::Instance()->GetTimeConf(m_actid);
}