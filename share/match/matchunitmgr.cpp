#include "pch.h"
#include "matchunitmgr.h"
#include "matchunit.h"
#include "matchmgrbase.h"
#include "table/globalconfig.h"

#include <ctime>

MatchUnitMgrBase::MatchUnitMgrBase(int type, MatchMgrBase* pMgr)
	:m_type(type), m_pMgr(pMgr)
{

}

MatchUnitMgrBase::~MatchUnitMgrBase()
{

}
struct PkMKey
{
	PkMKey()
	{
		matchCount = 0;	
		serverID = 0;
		point = 0;
		matchID = InvalidKMatchID;
	}
	bool operator < (const PkMKey& other) const
	{
		if(matchCount != other.matchCount)
		{
			return matchCount < other.matchCount;
		}
		if(serverID != other.serverID)
		{
			return serverID < other.serverID;
		}
		if(point != other.point)
		{
			return point > other.point;
		}
		return matchID < other.matchID;
	}

	bool CanMatch(const PkMKey& other) const
	{
		if(matchCount != other.matchCount)
		{
			return false;
		}
		if(serverID != other.serverID)
		{
			return false;
		}
		return true;		
	}

	UINT32 matchCount;//������һ��(2.ƥ������� 4.ƥ������)
	UINT32 serverID;//���ƥ�丳ֵΪ0
	UINT32 point;
	KMatchID matchID;//Ψһ
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MatchUnitMgrPkTwo::DoMatch(std::vector<MatchUnit*>& matchUnits)
{
	++m_doMatchCount;//��ͬʱ��ѡ��ͬ��ƥ�䵥Ԫ
	UINT32 doMatchMod = m_doMatchCount % 3;//0.����ƥ�䵥Ԫ 1.ֻƥ�䵥�� 2.ֻƥ��˫��
	UINT32 now = UINT32(time(NULL));
	std::map<PkMKey, MatchUnitPkTwo*> mUnits;
	for(size_t i = 0; i < matchUnits.size(); ++i)
	{
		if(matchUnits[i]->GetType() != GetType())
		{//�����쳣
			continue;
		}
		if(matchUnits[i]->GetRoleSize() == 0)
		{//�����쳣
			continue;
		}
		if(doMatchMod && doMatchMod != matchUnits[i]->GetRoleSize())
		{//�����ϱ���ƥ����Ҫ������
			continue;
		}
		auto pUnit = (MatchUnitPkTwo*)matchUnits[i];
		PkMKey key;//��������
		key.point = pUnit->GetPoint();
		key.matchID = pUnit->GetMatchID();
		if(pUnit->MatchCross(now))//���ƥ��
		{
			key.serverID = 0;//Ϊ0
		}
		else
		{
			key.serverID = pUnit->GetServerID();
		}
		if(pUnit->MatchRobot(now))//ƥ�������
		{
			key.matchCount = MatchRobotNeedCount();
		}
		else
		{
			key.matchCount = MatchRoleNeedCount();
		}
        auto ret = mUnits.insert(std::make_pair(key, pUnit));
        if(!ret.second)
        {
            SSError << " insert failed mod " << m_doMatchCount << END;
            pUnit->InfoPrint();
			continue;
        }
        if(mUnits.size() >= 1000)
        {
            SSWarn << " match unit to much " << END;
            break;
        }
	}

	//ƥ��ֻ�ʺ�2v2
	std::list<std::list<KMatchID> > matchedVec;
	for(auto i = mUnits.begin(); i != mUnits.end(); ++i)
	{
		const auto& Ikey = i->first;
		UINT32 count = 0;
		std::list<KMatchID> matched;
		for(auto j = i; j != mUnits.end(); ++j)
		{
			const auto& Jkey = j->first;
			if(!Ikey.CanMatch(Jkey))
			{
				break;
			}
			auto pUnit = j->second;
			count += pUnit->GetRoleSize();
			//Ϊ�˱�֤2�˶�һ����ͬһ��
			if(pUnit->GetRoleSize() == 1)
			{
				matched.push_back(pUnit->GetMatchID());
			}
			else
			{
				matched.push_front(pUnit->GetMatchID());
			}
			if(Ikey.matchCount == count)
			{
				//ƥ������Ҫ�������Ƿ�ƥ��
				if(Ikey.matchCount == MatchRobotNeedCount() || 
					(i->second)->MatchOther(now, j->second))
				{
					matchedVec.push_back(matched);
					i = j;
				}
				break;
			}
			if(count > Ikey.matchCount)
			{
				break;
			}
		}
	}

	for(auto i = matchedVec.begin(); i != matchedVec.end(); ++i)
	{
		KKSG::KMatchFightData data;
		m_pMgr->MatchOneFight(this, (*i), data);
	}
}

void MatchUnitMgrPkTwo::AddMatchUnit(MatchUnit* pMatchUnit)
{
	AddMatchID(pMatchUnit->GetMatchID());
}

void MatchUnitMgrPkTwo::DelMatchUnit(MatchUnit* pMatchUnit)
{
	DelMatchID(pMatchUnit->GetMatchID());
}

void MatchUnitMgrPkTwo::AddMatchID(KMatchID matchID)
{
	m_matchIDs.insert(matchID);
}

void MatchUnitMgrPkTwo::DelMatchID(KMatchID matchID)
{
	m_matchIDs.erase(matchID);
}

void MatchUnitMgrPkTwo::Update()
{
	std::vector<MatchUnit*> waitMatch;
	std::vector<KMatchID> waitDel;
	for(auto it = m_matchIDs.begin(); it != m_matchIDs.end(); ++it)
	{
		KMatchID matchID = (*it);
		auto pUnit = m_pMgr->Get(matchID);
		if(NULL == pUnit)
		{
			SSError << " find matchunit failed matchid = " << matchID << END;
			waitDel.push_back(matchID);
			continue;
		}
		if(pUnit->GetType() != GetType())
		{
			SSError << " matchunit type err matchid = " << matchID << " type = " << pUnit->GetType() << " type = " << GetType() << END;
			continue;
		}
		waitMatch.push_back(pUnit);

		if(waitMatch.size() > 10000)//��ֹ����
		{
			SSWarn << " wait match to much type = " << GetType() << END;
			break;
		}
	}

	DoMatch(waitMatch);

	if(!waitDel.empty())
	{
		m_pMgr->Del(waitDel);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MatchUnitMgrCustomPkTwo::DoMatch(UINT64 uid, std::vector<MatchUnit*>& matchUnits)
{
	++m_doMatchCount;//��ͬʱ��ѡ��ͬ��ƥ�䵥Ԫ
	UINT32 doMatchMod = m_doMatchCount % 3;//0.����ƥ�䵥Ԫ 1.ֻƥ�䵥�� 2.ֻƥ��˫��
	UINT32 now = UINT32(time(NULL));
	std::map<PkMKey, MatchUnitPkTwo*> mUnits;
	for(size_t i = 0; i < matchUnits.size(); ++i)
	{
		if(matchUnits[i]->GetType() != GetType())
		{//�����쳣
			continue;
		}
		if(matchUnits[i]->GetRoleSize() == 0)
		{//�����쳣
			continue;
		}
		if(doMatchMod && doMatchMod != matchUnits[i]->GetRoleSize())
		{//�����ϱ���ƥ����Ҫ������
			continue;
		}
		auto pUnit = (MatchUnitPkTwo*)matchUnits[i];
		PkMKey key;//��������
		key.point = pUnit->GetPoint();
		key.matchID = pUnit->GetMatchID();
		if(pUnit->MatchCross(now))//���ƥ��
		{
			key.serverID = 0;//Ϊ0
		}
		else
		{
			key.serverID = pUnit->GetServerID();
		}
		if(pUnit->MatchRobot(now))//ƥ�������
		{
			key.matchCount = MatchRobotNeedCount();
		}
		else
		{
			key.matchCount = MatchRoleNeedCount();
		}
        auto ret = mUnits.insert(std::make_pair(key, pUnit));
        if(!ret.second)
        {
            SSError << " insert failed mod " << m_doMatchCount << END;
            pUnit->InfoPrint();
			continue;
        }
        if(mUnits.size() >= 1000)
        {
            SSWarn << " match unit to much " << END;
            break;
        }
	}

	//ƥ��ֻ�ʺ�2v2
	std::list<std::list<KMatchID> > matchedVec;
	for(auto i = mUnits.begin(); i != mUnits.end(); ++i)
	{
		const auto& Ikey = i->first;
		UINT32 count = 0;
		std::list<KMatchID> matched;
		for(auto j = i; j != mUnits.end(); ++j)
		{
			const auto& Jkey = j->first;
			if(!Ikey.CanMatch(Jkey))
			{
				break;
			}
			auto pUnit = j->second;
			count += pUnit->GetRoleSize();
			//Ϊ�˱�֤2�˶�һ����ͬһ��
			if(pUnit->GetRoleSize() == 1)
			{
				matched.push_back(pUnit->GetMatchID());
			}
			else
			{
				matched.push_front(pUnit->GetMatchID());
			}
			if(Ikey.matchCount == count)
			{
				//ƥ������Ҫ�������Ƿ�ƥ��
				if(Ikey.matchCount == MatchRobotNeedCount() || 
					(i->second)->MatchOther(now, j->second))
				{
					matchedVec.push_back(matched);
					i = j;
				}
				break;
			}
			if(count > Ikey.matchCount)
			{
				break;
			}
		}
	}

	for(auto i = matchedVec.begin(); i != matchedVec.end(); ++i)
	{
		KKSG::KMatchFightData data;
		data.set_uid(uid);
		m_pMgr->MatchOneFight(this, (*i), data);
	}
}

void MatchUnitMgrCustomPkTwo::AddMatchUnit(MatchUnit* pMatchUnit)
{
	m_matchIDs[pMatchUnit->KKSGData().param()].insert(pMatchUnit->GetMatchID());
}

void MatchUnitMgrCustomPkTwo::DelMatchUnit(MatchUnit* pMatchUnit)
{
	m_matchIDs[pMatchUnit->KKSGData().param()].erase(pMatchUnit->GetMatchID());
}

void MatchUnitMgrCustomPkTwo::Update()
{
	for (auto i = m_matchIDs.begin(); i != m_matchIDs.end(); ++i)
	{
		std::vector<MatchUnit*> waitMatch;
		std::vector<KMatchID> waitDel;
		for(auto it = i->second.begin(); it != i->second.end(); ++it)
		{
			KMatchID matchID = (*it);
			auto pUnit = m_pMgr->Get(matchID);
			if(NULL == pUnit)
			{
				SSError << " find matchunit failed matchid = " << matchID << END;
				waitDel.push_back(matchID);
				continue;
			}
			if(pUnit->GetType() != GetType())
			{
				SSError << " matchunit type err matchid = " << matchID << " type = " << pUnit->GetType() << " type = " << GetType() << END;
				continue;
			}
			waitMatch.push_back(pUnit);

			if(waitMatch.size() > 1000)//��ֹ����
			{
				SSWarn << " wait match to much type = " << GetType() << END;
				break;
			}
		}

		DoMatch(i->first, waitMatch);

		if(!waitDel.empty())
		{
			m_pMgr->Del(waitDel);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MatchUnitMgrWeekendAct::AddMatchUnit(MatchUnit* pMatchUnit)
{
	Count2MatchID tmp;
	tmp.count = pMatchUnit->GetRoleSize();
	tmp.matchID = pMatchUnit->GetMatchID();
	auto ret = m_units.insert(tmp);
	if(!ret.second)
	{
		SSError << " insert failed " << END;
		pMatchUnit->InfoPrint();
	}
}

void MatchUnitMgrWeekendAct::DelMatchUnit(MatchUnit* pMatchUnit)
{
	Count2MatchID tmp;
	tmp.count = pMatchUnit->GetRoleSize();
	tmp.matchID = pMatchUnit->GetMatchID();
	if(0 == m_units.erase(tmp))
	{
		SSError << " erase failed " << END;
		pMatchUnit->InfoPrint();
	}
}

void MatchUnitMgrWeekendAct::Update()
{
	std::list<KMatchRoom> rooms;
	for(auto i = m_units.begin(); i != m_units.end(); ++i)
	{
		bool addFlag = false;
		for(auto j = rooms.begin(); j != rooms.end(); ++j)
		{
			if(j->Add(i->matchID, i->count))
			{
				addFlag = true;
				break;
			}
		}
		if(!addFlag)
		{
			KMatchRoom room(m_fullCount);
			if(room.Add(i->matchID, i->count))
			{
				rooms.push_back(room);
			}
		}
	}

	for(auto it = rooms.begin(); it != rooms.end(); ++it)
	{
        if(false == it->IsFull())
        {
            continue;
        }
		std::list<KMatchID> matched;	
		it->FillUnits(matched);
		KKSG::KMatchFightData tmpData;
		m_pMgr->MatchOneFight(this, matched, tmpData);	
	}
}

void MatchUnitMgrSurvive::AddMatchUnit(MatchUnit* pMatchUnit)
{
	m_units.insert(pMatchUnit->GetMatchID());
}

void MatchUnitMgrSurvive::DelMatchUnit(MatchUnit* pMatchUnit)
{
	m_units.erase(pMatchUnit->GetMatchID());
}

void MatchUnitMgrSurvive::Update()
{
	typedef std::list<KMatchID> OneRoom;
	std::list<OneRoom> rooms;
	OneRoom oneRoom;
	for(auto it = m_units.begin(); it != m_units.end(); ++it)
	{
		oneRoom.push_back(*it);		
		if(oneRoom.size() == GetGlobalConfig().SurviveFightRoleCount)//��
		{
			rooms.push_back(oneRoom);
			oneRoom.clear();
		}
	}

	for(auto it = rooms.begin(); it != rooms.end(); ++it)
	{
		KKSG::KMatchFightData tmpData;
		m_pMgr->MatchOneFight(this, *it, tmpData);	
	}
}
