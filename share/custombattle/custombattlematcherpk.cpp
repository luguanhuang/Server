#include "pch.h"
#include "time.h"
#include "util/gametime.h"
#include "table/globalconfig.h"
#include "custombattlematcherpk.h"
#include "custombattlematcher.h"

#define CUSTOMMATCH_LIMIT (GetGlobalConfig().CustomBattleMatcherLimit)

const KKSG::PkMatchStage* CustomBattlePkInfo::GetBack()
{
	int n = m_matchinfo.stages_size();
	if(0 == n)
	{
		return NULL;
	}
	return &(m_matchinfo.stages(n-1));
}

void CustomBattlePkInfo::PopBack()
{
	if(m_matchinfo.stages_size() > 0)
	{
		m_matchinfo.mutable_stages()->RemoveLast();
	}
}

bool CustomBattlePkInfo::CheckTimeout()
{
	const auto tmp = GetBack();
	if(NULL == tmp)
	{
		return true;
	}
	time_t timestamp = time_t(tmp->ctime());
	if (time(NULL) > timestamp)
	{
		PopBack();
	}
	return IsEmpty();
}

bool CustomBattlePkInfo::IsFitSpan(UINT32 temppoint)
{
	const auto tmp = GetBack();
	if(NULL == tmp)
	{
		return false;
	}
	UINT32 tmpspan = (Point() > temppoint)? (Point() - temppoint): (temppoint - Point());
	if(tmpspan <= tmp->span())
	{
		return true;
	}
	return false;
}

bool CustomBattlePkInfo::IsEmpty()
{
	return (0 == m_matchinfo.stages_size());
}

void CustomBattlePkInfo::ChangeDataByTimeDiff(UINT32 msTime, UINT32 wsTime)
{
	if(msTime == wsTime)
	{
		return;
	}
	for(int i = 0; i < m_matchinfo.stages_size(); ++i)
	{
		auto& data = *m_matchinfo.mutable_stages(i);
		UINT32 ctime = data.ctime();	
		if(wsTime > msTime)
		{
			ctime += (wsTime - msTime);
		}
		else
		{
			if(ctime > (msTime - wsTime))
			{
				ctime -= (msTime - wsTime);
			}
			else
			{
				ctime = 0;
			}
		}
		data.set_ctime(ctime);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
CustomBattleMatcherPk::CustomBattleMatcherPk()
{
	m_isofficial = true;
	m_matcher = NULL;
}

CustomBattleMatcherPk::CustomBattleMatcherPk(CustomBattleMatcherMgr* matcher, UINT64 battleuid, bool isfair)
{
	m_isofficial = true;
	m_matcher = matcher;
	m_sceneinfo.battleid = battleuid;
	m_sceneinfo.issystem = false;
	m_sceneinfo.isfaire = isfair;
	m_sceneinfo.mapid = CustomBattlePk_SceneId;
}

CustomBattleMatcherPk::~CustomBattleMatcherPk()
{

}

bool CustomBattleMatcherPk::Upate(UINT32& limit)
{
	bool islimit = false;
	if (CheckMatch(limit))
	{
		islimit = true;
	}
	CheckTimeout();
	return islimit;
}

void CustomBattleMatcherPk::CheckTimeout()
{
	for(auto it = m_allInfo.begin(); it != m_allInfo.end();)
	{
		if ((it->second).CheckTimeout())
		{
			SSWarn<<"time out, roleid:"<<it->first<<END;
			m_allInfo.erase(it++);
		}
		else
		{
			++it;
		}
	}
	m_limit.Update();
}

bool CustomBattleMatcherPk::CheckMatch(UINT32& limit)
{
	bool islimit = false;
	UINT32 MaxMatchCount = CUSTOMMATCH_LIMIT;
	std::priority_queue<CustomBattlePkInfo> matchQueue;
	for (auto it = m_allInfo.begin(); it != m_allInfo.end(); ++it)
	{
		matchQueue.push(it->second);
		if(matchQueue.size() >= MaxMatchCount)
		{
			break;
		}
	}
	if(matchQueue.empty())
	{
		return islimit;
	}

	std::vector<UINT64> waitDel;
	CustomBattlePkInfo last = matchQueue.top();
	matchQueue.pop();
	while(!matchQueue.empty())
	{
		CustomBattlePkInfo cur = matchQueue.top();
		matchQueue.pop();
		if(IsMatch(cur, last))
		{
			MatchPair(cur, last);

			waitDel.push_back(cur.RoleID());
			waitDel.push_back(last.RoleID());

			if ((int)(++limit) > CUSTOMMATCH_LIMIT)
			{
				islimit = true;
				break;
			}
			if(matchQueue.empty())
			{
				break;
			}
			else
			{
				last = matchQueue.top();
				matchQueue.pop();
			}
		}
		else
		{
			last = cur;
		}
	}

	for (size_t i = 0; i < waitDel.size(); ++i)
	{
		Remove(waitDel[i]);
	}

	return islimit;
}

bool CustomBattleMatcherPk::IsMatch(CustomBattlePkInfo& info11, CustomBattlePkInfo& info22)
{
	if(info11.IsEmpty() || info22.IsEmpty())
	{
		return false;
	}
	// is official
	if (m_isofficial)
	{
		if (m_limit.IsOne2OneLimit(info11.RoleID(), info11.Point(), info22.RoleID(), info22.Point()))
		{
			return false;
		}
	}

	if(info11.IsFitSpan(info22.Point()) || info22.IsFitSpan(info11.Point()))
	{
		return true;
	}
	return false;
}

bool CustomBattleMatcherPk::MatchPair(CustomBattlePkInfo& info11, CustomBattlePkInfo& info22)
{
	SSInfo<<"server:"<<info11.ServerID()<<" role11:"<<info11.RoleID()<<END;
	SSInfo<<"server:"<<info22.ServerID()<<" role22:"<<info22.RoleID()<<END;

	std::vector<UINT64> matchone;
	matchone.push_back(info11.RoleID());
	std::vector<UINT64> matchtwo;
	matchtwo.push_back(info22.RoleID());

	// limit record
	m_limit.Push(info11.RoleID(), info11.Point(), info22.RoleID(), info22.Point());

	m_matcher->DoBattlePair(m_sceneinfo, matchone, matchtwo);

	return true;
}

bool CustomBattleMatcherPk::Add(const KKSG::CustomBattleMatchRoleInfo& matchinfo, UINT32 msTime)
{
	SSInfo<<"serverid:"<<matchinfo.serverid()<<" roleid:"<<matchinfo.roleid()<<" mstime:"<<msTime<<END;
	CustomBattlePkInfo info;
	info.m_matchinfo = matchinfo;

	info.ChangeDataByTimeDiff(msTime, UINT32(GameTime::GetTime()));

	if (0 == info.ServerID() || 0 == info.RoleID())
	{
		SSError<<"serverid:"<<info.ServerID()<<" role:"<<info.RoleID()<<END;
		return false; 
	}

	if (IsIn(info.RoleID()))
	{
		SSWarn<<"have role:"<<info.RoleID()<<END;
		return false;
	}

	m_allInfo.insert(std::make_pair(info.RoleID(),info));

	return true;

}

void CustomBattleMatcherPk::Remove(UINT64 roleID)
{
	m_allInfo.erase(roleID);
}

bool CustomBattleMatcherPk::IsIn(UINT64 roleID)
{
	return (m_allInfo.find(roleID) != m_allInfo.end());
}
