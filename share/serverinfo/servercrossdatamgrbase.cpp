#include "pch.h"
#include "servercrossdatamgrbase.h"

void ServerCrossDataMgrBase::AddData(const KKSG::ServerCrossData& data)
{
	m_serverData[data.serverid()] = data;
}

KKSG::ServerCrossData* ServerCrossDataMgrBase::GetData(UINT32 serverID)
{
	auto it = m_serverData.find(serverID);
	if (it != m_serverData.end())
	{
		return &(it->second);
	}
	SSError << " get cross data failed serverid = " << serverID << END;
	return NULL;
}

void ServerCrossDataMgrBase::InfoPrint()
{
	for (auto it = m_serverData.begin(); it != m_serverData.end(); ++it)
	{
		InfoPrint(it->second);
	}
}

void ServerCrossDataMgrBase::InfoPrint(const KKSG::ServerCrossData& data)
{
	SSInfo << " serverid = " << data.serverid() << " opentime = " << data.serveropentime() << END;
	SSInfo << " worldlevel = " << data.worldlevel() << END;
	SSInfo << " sealtype = " << data.sealtype() << " seallevel = " << data.seallevel() << END;
	SSInfo << " isbackflow = " << data.isbackflow() << " backflowlevel = " << data.backflowlevel() << END;
	for(int i = 0; i < data.idipclosesys_size(); ++i)
	{
		SSInfo << " idip closed sys = " << data.idipclosesys(i) << END;
	}
	InfoPrint(data.teamdata());
}

void ServerCrossDataMgrBase::InfoPrint(const KKSG::ServerCrossTeamData& data)
{
	SSInfo << " team data:" << END;
	SSInfo << " weeknestexpid = " << data.weeknextexpid() << END;
	SSInfo << " dragon scene finish size = " << data.globaldragoninfo().sceneid_size() << END;
	//for(int i = 0; i < data.globaldragoninfo().sceneid_size(); ++i)
	//{
	//	SSInfo << " dragon scene = " << data.globaldragoninfo().sceneid(i) << END;
	//}
}

bool ServerCrossDataMgrBase::IsDragonSceneFinished(const KKSG::ServerCrossTeamData& data, UINT32 sceneID)
{
	for(int i = 0; i < data.globaldragoninfo().sceneid_size(); ++i)
	{
		if(data.globaldragoninfo().sceneid(i) == sceneID)
		{
			return true;
		}
	}
	return false;
}

bool ServerCrossDataMgrBase::IsServerDragonSceneFinished(UINT32 serverID, UINT32 sceneID)
{
	auto pData = GetData(serverID);
	if(pData)
	{
		return IsDragonSceneFinished(pData->teamdata(), sceneID);
	}
	SSError << " get server data failed serverid = " << serverID << END;
	return false;
}

