#include "pch.h"
#include "dragonguild/ptcg2m_checkdragonguildtask.h"
#include "dragonguild/dragonguildmgr.h"
#include  "table/globalconfig.h"
// generate by ProtoGen at date: 2017/9/12 14:39:21

void PtcG2M_CheckDragonGuildTask::Process(UINT32 dwConnID)
{
	if(m_Data .roleid_size() == 0)
		return;
	std::map<UINT64, UINT32>roleCnt;
	roleCnt.clear();
	std::vector<UINT64>rolesid;
	rolesid.clear();

	for(size_t i = 0; i < m_Data.roleid_size(); ++i)
	{
		rolesid.push_back(m_Data.roleid(i));
		UINT64 tempId = DragonGuildMgr::Instance()->GetDragonGuildIdByRole(m_Data.roleid(i));
		if(tempId == 0)
			continue;
		if(roleCnt.find(tempId) != roleCnt.end())
			roleCnt[tempId] = roleCnt[tempId] + 1;
		else
			roleCnt[tempId] = 1;
	}

	for(auto it = roleCnt.begin(); it != roleCnt.end(); ++it)
	{
		if(it->second >= GetGlobalConfig().DragonGuildTaskNeedPartnerCount)
		{
			DragonGuildMgr::Instance()->PassSceneAndFinishTask(it->first, rolesid, m_Data.rolecount(), m_Data.sceneid(), m_Data.nodiecount(), m_Data.time());
		}
		//小分队三人通关，单人通关的特殊成就
		else
			DragonGuildMgr::Instance()->UnLockSpeicalAchieve(it->first, rolesid, m_Data.rolecount(), m_Data.sceneid(), m_Data.nodiecount(), m_Data.time());
	}

}
