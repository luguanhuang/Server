#include "pch.h"
#include "scenepreparelist.h"
#include "sceneconfig.h"
#include "scene.h"
#include "foreach.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/ptcg2c_sceneprepareinfontf.h"

class SyncScenePrepareList : public IScenePrepareList
{
public:
	SyncScenePrepareList()
	{
		m_isPrepared = false;
		m_isstop= false;
		m_Name = "SyncScenePrepareList";
		m_time = 0;
	}

	~SyncScenePrepareList()
	{

	}

	void Update() 
	{
		if (m_isPrepared && !m_isstop)
		{
			foreach(i in m_prepareClients)
			{
				if (i->second.eState == Loaded)
				{
					NotifyClient(&i->second);
				}
			}
			m_isstop = true;
			EnterSceneFinish();
			return;
		}
		time_t now = time(NULL);
		// time out loaded
		bool hasdelayenter = false;
		foreach(i in m_prepareClients)
		{
			if (i->second.eState == Loaded && m_time <= now)
			{
				NotifyClient(&i->second);
				hasdelayenter = true;
			}
		}
		if (hasdelayenter)
		{
			EnterSceneFinish();
		}
	}
	///> 场景进入结束，给scene回调
	void EnterSceneFinish()
	{
		if (!m_isenterscenefinish)
		{
			m_pListener->OnEnterSceneFinish();
			m_isenterscenefinish = true;
		}
	}

	void NotifyClient(ClientLoadingInfo* loading)
	{
		if (m_pListener->OnClientLoaded(loading->qwRoleID, loading->delayid))
		{
			loading->eState = EnterState;
		}
	}

	virtual void AddPrepareRole(UINT64 qwRoleID, const std::string &RoleName, bool iswatcher)
	{
		if (m_isPrepared && !iswatcher)
		{
			m_isPrepared = false;
			m_isstop = false;
		}
		IScenePrepareList::AddPrepareRole(qwRoleID, RoleName, iswatcher);
		return;
	}

	virtual bool RemovePrepareRole(UINT64 qwRoleID)
	{
		if (IScenePrepareList::RemovePrepareRole(qwRoleID))
		{
			CheckAllClientState();
			return true;
		}
		return false;
	}

	virtual bool ChangeRoleState(UINT64 qwRoleID, LoadingState eNewState, UINT32 delayid)
	{
		auto i = m_prepareClients.find(qwRoleID);
		if (i == m_prepareClients.end())
		{
			SSWarn<<"roleid:"<<qwRoleID<<", state:"<<eNewState<<"not find"<<END;
			return false;
		}

		LoadingState laststate = i->second.eState;
		i->second.eState = eNewState;
		i->second.delayid = delayid;

		if(Loaded != laststate && Loaded == eNewState)
		{
			i->second.time = time(NULL);
			if (!i->second.iswatcher)
			{
				///> 记录第一个loading成功的人
				if (0 == m_time)
				{
					m_time = (UINT32)time(NULL) + PREPARE_TIMEOUT;
				}
				///> 通知loading好了的人，哪些人没loading好
				NotifyUnReadyRoles();		
			}
		}

		///> 如果是观战者不需要相互等待
		if (i->second.iswatcher)
		{
			if(Loaded == eNewState)
			{
				if (m_pListener->OnClientLoaded(qwRoleID, delayid))
				{
					i->second.eState = EnterState;
				}
			}
		}
		else
		{
			CheckAllClientState();
		}

		return laststate != eNewState;
	}

	void CheckAllClientState()
	{
		if (m_isPrepared)
		{
			return;
		}
		foreach(i in m_prepareClients)
		{
			if (!i->second.iswatcher && i->second.eState == Loading)
			{
				return;
			}
		}
		m_isPrepared = true;
	}

	void NotifyUnReadyRoles()
	{
		std::vector<UINT64> roleids;
		// pack
		PtcG2C_ScenePrepareInfoNtf ntf;
		for (auto i = m_prepareClients.begin(); i != m_prepareClients.end(); ++i)
		{
			if (!i->second.iswatcher)
			{
				if (Loading == i->second.eState)
				{
					ntf.m_Data.add_unreadyroles(i->second.strRoleName);
				}
				else if (Loaded == i->second.eState)
				{
					roleids.push_back(i->first);
				}
			}
		}
		// notify
		for (auto j = roleids.begin(); j != roleids.end(); ++j)
		{
			Role* role = RoleManager::Instance()->FindByRoleID(*j);
			if (NULL != role)
			{
				role->Send(ntf);
			}
		}
	}

	virtual std::vector<std::string> GetNames()
	{
		std::vector<std::string> names;
		for (auto i = m_prepareClients.begin(); i != m_prepareClients.end(); ++i)
		{
			names.push_back(i->second.strRoleName);
		}
		return names;
	}

	virtual void Clear()
	{
		m_isPrepared = false;
		m_prepareClients.clear();
	}

private:
	bool m_isPrepared;
	bool m_isstop;
	UINT32 m_time;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
class SoloScenePrepareList : public IScenePrepareList
{
public:
	SoloScenePrepareList()
	{
		m_Name = "SoloScenePrepareList";
	}

	virtual std::vector<std::string> GetNames()
	{
		std::vector<std::string> names;
		return names;
	}

	bool ChangeRoleState(UINT64 qwRoleID, LoadingState eNewState, UINT32 delayid)
	{
		auto i = m_prepareClients.find(qwRoleID);
		if (i == m_prepareClients.end())
		{
			SSWarn<<"roleid:"<<qwRoleID<<", state:"<<eNewState<<"not find"<<END;
			return false;
		}

		LoadingState laststate = i->second.eState;
		i->second.eState = eNewState;
		i->second.delayid = delayid;

		if(Loaded == eNewState)
		{
			if (m_pListener->OnClientLoaded(qwRoleID, delayid))
			{
				i->second.eState = EnterState;
			}
		}
		return laststate != eNewState;
	}

};

////////////////////////////////////////////////////////////////////////////////////////////////////////
IScenePrepareList * IScenePrepareList::CreateScenePrepareList(SceneInfo *pSceneInfo)
{
	if (pSceneInfo->m_pConf->syncMode != SERVERMODE)
	{
		return new SoloScenePrepareList();
	}

	UINT32 type = pSceneInfo->m_pConf->type;
	if (SceneConfig::Instance()->IsPrepareExcept(type))
	{
		return new SoloScenePrepareList();
	}

	return new SyncScenePrepareList();
}

IScenePrepareList * IScenePrepareList::CreateSoloPrepareList()
{
	return new SoloScenePrepareList();
}

ClientLoadingInfo* IScenePrepareList::GetLoadingInfo(UINT64 roleid)
{
	auto i = m_prepareClients.find(roleid);
	if (i == m_prepareClients.end())
	{
		return NULL;
	}
	return &i->second;
}

void IScenePrepareList::AddPrepareRole(UINT64 qwRoleID, const std::string &RoleName, bool iswatcher)
{
	ClientLoadingInfo oInfo;
	oInfo.qwRoleID = qwRoleID;
	oInfo.strRoleName = RoleName;
	oInfo.eState = Loading;
	oInfo.time = time(NULL);
	oInfo.iswatcher = iswatcher;
	m_prepareClients[qwRoleID] = oInfo;
	return;
}

bool IScenePrepareList::RemovePrepareRole(UINT64 qwRoleID)
{
	auto i = m_prepareClients.find(qwRoleID);
	if (i != m_prepareClients.end())
	{
		m_prepareClients.erase(i);
		return true;
	}
	return false;
}
