#include "pch.h"
#include "rolesummarymgr.h"
#include "dblink.h"
#include "timer.h"
#include "timermgr.h"
#include "unit/role.h"
#include "timeutil.h"
#include "globalconfig.h"
#include "time.h"
#include "util/XRandom.h"
#include "foreach.h"
#include "table/ProfessionMgr.h"
#include "db/mysqlmgr.h"
#include "network/mslink.h"
#include "role/ptcg2m_updaterolesummary.h"

#define SUMMARY_PARTSIZE 256*1024
#define SUMMARY_SINGLESIZE 10240
#define COUNT_UPDATE 100

class RoleSummarySaveTimer : public ITimer
{
public:

	RoleSummarySaveTimer()
	{
		m_handler = INVALID_HTIMER;
	}

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount)
	{
		RoleSummaryMgr::Instance()->SendChangesToMS();
	}

	void Start(int interval)
	{
		m_handler = CTimerMgr::Instance()->SetTimer(this, 0, interval, -1, __FILE__, __LINE__);
	}

	void Stop()
	{
		CTimerMgr::Instance()->KillTimer(m_handler);
	}

private:
	HTIMER m_handler;
};

INSTANCE_SINGLETON(RoleSummaryMgr)

RoleSummaryMgr::RoleSummaryMgr()
{
	m_pTimerHandler = NULL;
}

RoleSummaryMgr::~RoleSummaryMgr()
{

}

bool RoleSummaryMgr::Init()
{
	///> 不是跨服，并且是主城
	if (!GSConfig::Instance()->IsCrossGS() && GSConfig::Instance()->IsHallGS())
	{
		m_pTimerHandler = new RoleSummarySaveTimer();
		((RoleSummarySaveTimer *)m_pTimerHandler)->Start(GetGlobalConfig().RoleSummarySaveInterval * 1000);
	}

	return true;
}

void RoleSummaryMgr::Uninit()
{
	if (m_pTimerHandler)
	{
		((RoleSummarySaveTimer *)m_pTimerHandler)->Stop();
		delete m_pTimerHandler;
		m_pTimerHandler = NULL;
	}

	m_ChangedRoleSummarys.clear();
	for(auto it = m_oRoleSummarys.begin(); it != m_oRoleSummarys.end(); ++it)
	{
		delete it->second;
		it->second = NULL;
	}
	m_oRoleSummarys.clear();
}

void RoleSummaryMgr::UpdateRoleSummary(Role *pRole, int mask, void* arg)
{
	RoleSummary *pSummary = GetRoleSummary(pRole->GetID(), mask == SUMMARY_ALL);
	bool isadd = false;
	if (pSummary == NULL)
	{
		pSummary = new RoleSummary();
		isadd = true;
	}

	pSummary->Update(pRole, mask, arg);
	MarkChanged(pRole->GetID(), mask);

	if (isadd)
	{
		AddRoleSummary(pSummary);
	}
}

void RoleSummaryMgr::RemoveRoleSummary(UINT64 roleid)
{
	RoleSummary *pSummary = GetRoleSummary(roleid);
	if (pSummary == NULL)	
	{
		return;
	}
	delete pSummary;
	m_oRoleSummarys.erase(roleid);
}

void RoleSummaryMgr::SendChangesToMS()
{
	UINT64 begin = TimeUtil::GetMilliSecond();

	if (m_queuechanges.empty())
		return;

	int bytes = 0;
	UINT32 count = 0;
	PtcG2M_UpdateRoleSummary ptc;
	ptc.SetDontCompress();
	while (!m_queuechanges.empty())
	{
		UINT64 roleid = m_queuechanges.front();
		m_queuechanges.pop();

		UINT32 mask = 0;
		auto i = m_ChangedRoleSummarys.find(roleid);
		if (i != m_ChangedRoleSummarys.end())
		{
			mask = i->second;
			m_ChangedRoleSummarys.erase(i);
		}

		RoleSummary *pSummary = GetRoleSummary(roleid);
		if (pSummary == NULL)
			continue;

		KKSG::RoleSummaryStored* dbsummary = ptc.m_Data.add_datas();
		ptc.m_Data.add_masks(mask);
		ptc.m_Data.add_timestamp(pSummary->GetLastUpdateTime());
		SummaryToDBStored(pSummary, *dbsummary);
		UINT32 tempbytes = dbsummary->ByteSize();
		if (tempbytes >= SUMMARY_SINGLESIZE)
		{
			SSWarn<<"roleid:"<<pSummary->GetID()<<" size:"<<tempbytes<<END;
		}
		bytes += tempbytes;

		if (bytes >= SUMMARY_PARTSIZE)
		{
			SSWarn<<"send ptc:"<<ptc.m_Data.ByteSize()<<" count:"<<count<<" left count:"<<m_queuechanges.size()<<END;
			bytes = 0;
			MSLink::Instance()->SendTo(ptc);
			ptc.m_Data.Clear();
		}
		// leave gs to delete
		if (pSummary->istodelete)
		{
			auto i = m_oRoleSummarys.find(roleid);
			if (i != m_oRoleSummarys.end())
			{
				delete i->second;
				i->second = NULL;

				m_oRoleSummarys.erase(i);
			}
		}

		if (++count >= COUNT_UPDATE)
		{
			break;
		}
	}
	if (0 != ptc.m_Data.datas_size())
	{
		SSInfo<<"send ptc:"<<ptc.m_Data.ByteSize()<<" count:"<<count<<" left count:"<<m_queuechanges.size()<<END;
		MSLink::Instance()->SendTo(ptc);
	}

	UINT64 end = TimeUtil::GetMilliSecond();
	LogInfo("send to ms use time: %llu ms", end - begin);
}

RoleSummary* RoleSummaryMgr::GetRoleSummary(UINT64 qwRoleID, bool isnew)
{
	auto it = m_oRoleSummarys.find(qwRoleID);
	if (it == m_oRoleSummarys.end())
	{
		if (!isnew)
		{
			SSWarn << "role summary for roleid [" << qwRoleID << "] not found!" << END;
		}
		return NULL;
	}
	return it->second;
}

void RoleSummaryMgr::AddRoleSummary(RoleSummary* rolesummary)
{
	UINT64 roleid = rolesummary->GetID();

	m_oRoleSummarys[roleid] = rolesummary;
	SSDebug << "add role summary for roleid [" << roleid << "]" << END;
}

void RoleSummaryMgr::SummaryToDBStored(RoleSummary* pSummary, KKSG::RoleSummaryStored& oDBSummary)
{
	if(NULL == pSummary)
	{
		return ;
	}
	oDBSummary = pSummary->GetData();

	oDBSummary.clear_attribute();
	pSummary->oAttribute.SaveToAttributes(*oDBSummary.mutable_attribute());

	oDBSummary.clear_fashion();
	for (int i = 0; i < FashionCount; ++i)
	{
		oDBSummary.add_fashion(pSummary->oFashion.GetFashionPos(i));
	}
	oDBSummary.clear_equip();
	for (int i = 0; i < EquipCount; ++i)
	{
		KKSG::Item* item = oDBSummary.add_equip();
		XItem::Item2KKSGItem(pSummary->oEquip.GetEquip(i), item);
	}
	oDBSummary.clear_emblem();
	for (int i = 0; i < EmblemCount; ++i)
	{
		KKSG::Item* item = oDBSummary.add_emblem();
		XItem::Item2KKSGItem(pSummary->oEmblem.GetEmblem(i), item);
	}
	oDBSummary.clear_artifact();
	for (int i = 0; i < ArtifactCount; ++i)
	{
		KKSG::Item* item = oDBSummary.add_artifact();
		XItem::Item2KKSGItem(pSummary->oArtifact.GetArtifact(i), item);
	}

	oDBSummary.clear_skills();
	const std::vector<skillInfo>& skills = pSummary->oSkills.GetSkills();
	for (auto i = skills.begin(); i != skills.end(); ++i)
	{
		KKSG::SkillInfo *pskill = oDBSummary.add_skills();
		pskill->set_skillhash(i->skillhash);
		pskill->set_skilllevel(i->skilllevel);
		pskill->set_skillpoint(i->skillpoint);
	}
	oDBSummary.clear_bindskills();
	const std::vector<UINT32>& bindskills = pSummary->oSkills.GetBindSkills();
	for (auto i = bindskills.begin(); i != bindskills.end(); ++i)
	{
		oDBSummary.add_bindskills(*i);
	}

	oDBSummary.clear_sprites();
	for (UINT32 i = 0; i < pSummary->oSprites.size(); i++)
	{
		KKSG::SpriteInfo* sprite = oDBSummary.add_sprites();
		sprite->CopyFrom(pSummary->oSprites[i]);
	}

	oDBSummary.clear_pets();
	foreach (iterMap in pSummary->oPetInfo.m_rolePetMap)
	{
		KKSG::PetSingle* pPet = oDBSummary.add_pets();
		pPet->CopyFrom(iterMap->second);
	}
	foreach(it in pSummary->display_fashion)
	{
		oDBSummary.add_display_fashion(*it);
	}
}

void RoleSummaryMgr::DBStoredToSummary(const KKSG::RoleSummaryStored* pData, RoleSummary* pSummary)
{
	if (NULL == pData || NULL == pSummary)
	{
		return ;
	}
	
	pSummary->Init(*pData);

	pSummary->oAttribute.LoadFromAttributes(pData->attribute());

	for (int i = 0; i < pData->fashion_size(); ++i)
	{
		pSummary->oFashion.SetFashionPos(i, pData->fashion(i));
	}
	// 外显
	for (int i = 0; i < pData->display_fashion_size(); ++i)
	{
		pSummary->display_fashion.insert(pData->display_fashion(i));
	}

	for (int i = 0; i < pData->equip_size(); ++i)
	{
		pSummary->oEquip.SetEquip(i, pData->equip(i));
	}
	for (int i = 0; i < pData->emblem_size(); ++i)
	{
		pSummary->oEmblem.SetEmblem(i, pData->emblem(i));
	}
	for (int i = 0; i < pData->artifact_size(); ++i)
	{
		pSummary->oArtifact.SetArtifact(i, pData->artifact(i));
	}

	pSummary->oSkills.Clear();
	for (int j = 0; j < pData->skills_size(); ++j)
	{
		skillInfo info;
		info.skillhash = pData->skills(j).skillhash();
		info.skilllevel = pData->skills(j).skilllevel();
		pSummary->oSkills.Add(info);
	}
	for (int k = 0; k < pData->bindskills_size(); ++k)
	{
		pSummary->oSkills.AddBind(pData->bindskills(k));
	}

	if (pData->sprites_size() != 0)
	{
		for(int i = 0; i < pData->sprites_size(); i++)
		{
			pSummary->oSprites[i].CopyFrom(pData->sprites(i));
		}
	}

	for (int cnt = 0; cnt < pData->pets_size(); ++cnt)
	{
		pSummary->oPetInfo.m_rolePetMap[pData->pets(cnt).uid()].CopyFrom(pData->pets(cnt));
	}
}

void RoleSummaryMgr::MarkChanged(UINT64 roleid, UINT32 mask)
{
	if (GSConfig::Instance()->IsCrossGS() || !GSConfig::Instance()->IsHallGS())
	{
		return;
	}
	auto i = m_ChangedRoleSummarys.find(roleid);
	if (i == m_ChangedRoleSummarys.end())
	{
		m_queuechanges.push(roleid);
	}
	m_ChangedRoleSummarys[roleid] |= mask;
}

void RoleSummaryMgr::MarkToDelete(UINT64 roleid, bool isdelete)
{
	auto i = m_oRoleSummarys.find(roleid);
	if (i != m_oRoleSummarys.end())
	{
		i->second->istodelete = isdelete;
	}
}

void RoleSummaryMgr::SendSingleRoleSummary(UINT64 roleid)
{
	RoleSummary *pSummary = GetRoleSummary(roleid);
	if (pSummary == NULL)
	{
		return;
	}
	PtcG2M_UpdateRoleSummary ptc;
	ptc.SetDontCompress();

	KKSG::RoleSummaryStored* dbsummary = ptc.m_Data.add_datas();
	ptc.m_Data.add_masks(SUMMARY_ALL);
	ptc.m_Data.add_timestamp(pSummary->GetLastUpdateTime());
	SummaryToDBStored(pSummary, *dbsummary);
	UINT32 tempbytes = dbsummary->ByteSize();
	if (tempbytes >= SUMMARY_SINGLESIZE)
	{
		SSWarn<<"roleid:"<<pSummary->GetID()<<" size:"<<tempbytes<<END;
	}

	MSLink::Instance()->SendTo(ptc);
}
