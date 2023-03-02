#include "pch.h"
#include "matchmgrbase.h"
#include "pb/enum.pb.h"
#include "pb/project.pb.h"
#include "matchunit.h"
#include "expmatchunit.h"
#include "matchunitmgr.h"
#include <ctime>

MatchMgrBase::MatchMgrBase()
{
}

MatchMgrBase::~MatchMgrBase()
{
	Clear();

	for(auto it = m_type2Mgr.begin(); it != m_type2Mgr.end(); ++it)
	{
		delete it->second;
	}
	m_type2Mgr.clear();
}

void MatchMgrBase::Update()
{
	UINT32 now = (UINT32)time(NULL);
	for(auto it = m_type2Mgr.begin(); it != m_type2Mgr.end(); ++it)
	{
		(it->second)->Update();
	}
	
	m_delay.Update(now);
}

bool MatchMgrBase::AddMatchUnit(int type, const KKSG::KMatchUnit& kunit)
{
	SSInfo << " add match unit type = " << type << " server = " << kunit.serverid() << " role = " << kunit.roleid() << " team = " << kunit.teamid() << END;

	if(m_matchUnits.size() > 100000)
	{
		SSWarn << " match unit to much size = " << m_matchUnits.size() << END;
		return false;
	}

	if(!AddCond(kunit))//条件判断
	{
		return false;
	}

	KMatchID matchID = Add(type, kunit);
	if(InvalidKMatchID == matchID)
	{
		return false;
	}

	return true;
}

void MatchMgrBase::DelMatchUnitTeam(UINT32 serverID, int teamID)
{
	KMatchKey key;
	key.serverID = serverID;
	key.teamID = teamID;
	KMatchID matchID = GetMatchIDByKey(key);
	if(InvalidKMatchID == matchID)
	{
		SSInfo << "find matchid failed serverid = " << serverID << " teamid = " << teamID << END;
		return ;	
	}
	Del(matchID, MDR_NONE);
}

void MatchMgrBase::DelMatchUnitRole(UINT32 serverID, UINT64 roleID)
{
	KMatchKey key;
	key.serverID = serverID;
	key.roleID = roleID;
	KMatchID matchID = GetMatchIDByKey(key);
	if(InvalidKMatchID == matchID)
	{
		SSInfo << "find matchid failed serverid = " << serverID << " roleid = " << roleID << END;
		return ;	
	}
	Del(matchID, MDR_NONE);
}

KMatchID MatchMgrBase::Add(int type, const KKSG::KMatchUnit& kunit)
{
	auto pUnitMgr = GetUnitMgr(type);
	if(NULL == pUnitMgr)
	{
		SSError << " find unitmgr failed type = " << type << END;
		return InvalidKMatchID;
	}

	KMatchID matchID = NewMatchID();
	MatchUnit* pMatchUnit = NULL;
	switch(type)
	{
	case KKSG::KMT_PKTWO:
		{
			pMatchUnit = new MatchUnitPkTwo(matchID, type, kunit);
		}
		break;
	case KKSG::KMT_CUSTOM_PKTWO:
		{
			pMatchUnit = new MatchUnitPkTwo(matchID, type, kunit);
		}
		break;
	case KKSG::KMT_EXP:
		{
			pMatchUnit = new MatchUnitExp(matchID, type, kunit);
		}
		break;
	case KKSG::KMT_WEEKEND_ACT:
		{
			pMatchUnit = new MatchUnitWeekendAct(matchID, type, kunit);
		}
		break;
	case KKSG::KMT_SURVIVE:
		{
			pMatchUnit = new MatchUnitSurvive(matchID, type, kunit);
		}
		break;
	default:
		break;
	}

	if(NULL == pMatchUnit)
	{
		SSError << " add failed type = " << type << END;
		return InvalidKMatchID;
	}

	auto ret = m_matchUnits.insert(std::make_pair(pMatchUnit->GetMatchID(), pMatchUnit));
	if(!ret.second)	
	{
		SSError << " insert failed type = " << type << " matchid = " << pMatchUnit->GetMatchID() << END;
		delete pMatchUnit;
		pMatchUnit = NULL;
		return InvalidKMatchID;
	}

	pUnitMgr->AddMatchUnit(pMatchUnit);

	AddSyn(pMatchUnit);

	pMatchUnit->InfoPrint();

	return pMatchUnit->GetMatchID();
}

void MatchMgrBase::Del(std::vector<KMatchID>& waitDel, MatchDelReason reason/* = MDR_NONE*/)
{
	for(size_t i = 0; i < waitDel.size(); ++i)
	{
		Del(waitDel[i], reason);
	}
}

void MatchMgrBase::Del(std::list<KMatchID>& waitDel, MatchDelReason reason/* = MDR_NONE*/)
{
	for(auto i = waitDel.begin(); i != waitDel.end(); ++i)
	{
		Del(*i, reason);
	}
}

void MatchMgrBase::Del(KMatchID matchID, MatchDelReason reason)
{
	auto pMatchUnit = Get(matchID);
	if(NULL == pMatchUnit)
	{
		SSError << " find matchunit failed matchid = " << matchID << END;
		return;
	}

	auto pUnitMgr = GetUnitMgr(pMatchUnit->GetType());
	if(pUnitMgr)
	{
		pUnitMgr->DelMatchUnit(pMatchUnit);
	}
	else
	{
		SSError << " find unitmgr failed type = " << pMatchUnit->GetType() << END;
	}

	DelSyn(pMatchUnit);

	OnDelReason(pMatchUnit, reason);

	m_matchUnits.erase(matchID);
	delete pMatchUnit;
	pMatchUnit = NULL;
}

MatchUnit* MatchMgrBase::Get(KMatchID matchID)
{
	auto it = m_matchUnits.find(matchID);
	if(it != m_matchUnits.end())
	{
		return it->second;
	}
	return NULL;
}

void MatchMgrBase::Clear()
{
	std::vector<KMatchID> waitDel;
	for(auto it = m_matchUnits.begin(); it != m_matchUnits.end(); ++it)
	{
		waitDel.push_back(it->first);
	}
	Del(waitDel);
}

KMatchID MatchMgrBase::NewMatchID()
{
	static UINT32 MatchIDIndex = 1;
	return MatchIDIndex++;
}

void MatchMgrBase::MatchOneFight(MatchUnitMgrBase* pUnitMgr, std::list<KMatchID>& fightUnits, KKSG::KMatchFightData& data)
{
	UINT32 now = UINT32(time(NULL));
	if(NULL == pUnitMgr)
	{
		return;
	}
	if(!KKSG::KMatchType_IsValid(pUnitMgr->GetType()))
	{
		return;
	}
	KKSG::KMatchType type = (KKSG::KMatchType)(pUnitMgr->GetType());
	if(fightUnits.empty())
	{
		SSError << "  fight unit empty type = " << type << END;
		return;
	}

	std::vector<MatchUnit*> matchedUnit;

	data.set_type(type);
	//填充数据 公共数据
	for(auto i = fightUnits.begin(); i != fightUnits.end(); ++i)
	{
		KMatchID matchID = (*i);
		auto pUnit = Get(matchID);
		if(NULL == pUnit)
		{
			SSError << " get unit failed matchid = " << matchID << END;
			return;
		}
		data.add_units()->CopyFrom(pUnit->KKSGData());
		data.mutable_roles()->MergeFrom(pUnit->KKSGData().roles());

		matchedUnit.push_back(pUnit);
	}

	CreateSceneBegin(data, matchedUnit);

	Del(fightUnits, MDR_MATCHED);//删除	
	matchedUnit.clear();//指针无效 清除

	for(int i = 0; i < data.roles_size(); ++i)
	{
		UINT64 roleID = data.roles(i).roleid();
		UINT32 serverID = data.roles(i).serverid();
		SSInfo << " matched role = " << roleID << " server = " << serverID << END;
		AddRoleDelay(roleID, now);
	}
}

void MatchMgrBase::InfoPrint()
{
    SSInfo << " match unit count = " << m_matchUnits.size() << " key2matchid count = " << m_key2MatchID.size() << END;
	for(auto it = m_role2MatchID.begin(); it != m_role2MatchID.end(); ++it)
	{
		SSInfo << " roleid = " << it->first << " matchid = " << it->second << END;
	}
	for(auto it = m_matchUnits.begin(); it != m_matchUnits.end(); ++it)
	{
		(it->second)->InfoPrint();
	}
}

void MatchMgrBase::AddUnitMgr(int type, MatchUnitMgrBase* pMgr)
{
	m_type2Mgr[type] = pMgr;
}

MatchUnitMgrBase* MatchMgrBase::GetUnitMgr(int type)
{
	auto it = m_type2Mgr.find(type);
	if(it != m_type2Mgr.end())
	{
		return it->second;
	}
	return NULL;
}

KMatchID MatchMgrBase::GetMatchIDByKey(const KMatchKey& key)
{
	auto it = m_key2MatchID.find(key);
	if(it != m_key2MatchID.end())
	{
		return it->second;
	}
	return InvalidKMatchID;
}

bool MatchMgrBase::AddCond(const KKSG::KMatchUnit& kunit)
{
	if(kunit.roleid() && kunit.teamid())
	{
		return false;
	}
	if(!kunit.roleid() && !kunit.teamid())
	{
		return false;
	}
	std::unordered_set<UINT64> roleIDs;
	MatchUnit::FillRoles(roleIDs, kunit);
	if(roleIDs.size() != kunit.roles_size())
	{
		SSError << " role size err size1 = " << roleIDs.size() << " size2 = " << kunit.roles_size() << END;
		return false;
	}
	for(auto it = roleIDs.begin(); it != roleIDs.end(); ++it)
	{
		UINT64 roleID = (*it);
		if(m_role2MatchID.find(roleID) != m_role2MatchID.end())
		{
			SSError << " add failed role repeat roleid = " << roleID << END;
			return false;
		}
		if(m_delay.HaveRole(roleID))
		{
			SSWarn << " add failed role delay roleid = " << roleID << END;
			return false;
		}
	}

	KMatchKey key;
	MatchUnit::FillMatchKey(key, kunit);
	if(m_key2MatchID.find(key) != m_key2MatchID.end())
	{
		SSWarn << " match key repeat " << END;
		return false;
	}
	return true;
}

void MatchMgrBase::AddSyn(MatchUnit* pUnit)
{
	if(NULL == pUnit)
	{
		return;
	}
	std::unordered_set<UINT64> roleIDs;
    pUnit->FillRoles(roleIDs);
	for(auto it = roleIDs.begin(); it != roleIDs.end(); ++it)
	{
		UINT64 roleID = (*it);
		m_role2MatchID[roleID] = pUnit->GetMatchID();
	}

	KMatchKey key;
	pUnit->FillMatchKey(key);
	m_key2MatchID[key] = pUnit->GetMatchID();
}

void MatchMgrBase::DelSyn(MatchUnit* pUnit)
{
	if(NULL == pUnit)
	{
		return;
	}
	std::unordered_set<UINT64> roleIDs;
	pUnit->FillRoles(roleIDs);
	for(auto it = roleIDs.begin(); it != roleIDs.end(); ++it)
	{
		UINT64 roleID = (*it);
		if(0 == m_role2MatchID.erase(roleID))
		{
			SSError << " role2matchid erase failed matchid = " << pUnit->GetMatchID() << END; 
		}
	}

	KMatchKey key;
	pUnit->FillMatchKey(key);
	if(0 == m_key2MatchID.erase(key))
	{
		SSError << "key2matchid erase failed matchid = " << pUnit->GetMatchID() << END;
	}
}

void MatchMgrBase::AddRoleDelay(UINT64 roleID, UINT32 now)
{
	m_delay.AddRole(roleID, now + 5);//匹配成功的5秒以内不能在匹配
}
