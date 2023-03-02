#include "pch.h"
#include <time.h>
#include "designationRecord.h"
#include "pb/project.pb.h"
#include "util/XRandom.h"
#include "globalconfig.h"
#include "util/XCommon.h"
#include "unit/role.h"
#include "gamelogic/designationMgr.h"
#include "rolesummary/rolesummarybase.h"
#include "unit/roleoutlook.h"
#include "gamelogic/rolesummarymgr.h"
#include "utility/tlogger.h"
#include "define/tlogenum.h"
#include "config.h"
#include "tablemgr/backflowtablemgr.h"
#include "util/gametime.h"


DesignationRecord::DesignationRecord(Role* role)
{
	m_pRole = role;
	m_dwCoverDesignationID = 0; 
	m_dwAbilityDesignationID = 0;
	m_dwMaxAbilityDesignationID = 0;
	m_mapDesignationInfo.clear();
}

DesignationRecord::~DesignationRecord()
{

}

bool DesignationRecord::Illegal(UINT32 nDesID)
{
	return false;
}

void DesignationRecord::FirstInit(UINT32 roleNum)
{
	if (GSConfig::Instance()->IsBackFlowServer() && roleNum == 1)
	{
		BackFlowServerConfig::RowData* pData = BackFlowTableMgr::Instance()->GetServerConfig(m_pRole->GetProfession(), GSConfig::Instance()->GetBackFlowLevel());
		if (pData)
		{
			for (auto it = pData->Designation.begin(); it != pData->Designation.end(); ++it)
			{
				DesignationTable::RowData* pDes = DesignationMgr::Instance()->GetTable()->GetByID(*it);
				if (NULL == pDes)
				{
					continue;
				}
				stDesignationInfo stc; 
				stc.bIsNew = true; 
				stc.dwDesignationID = *it;
				stc.dwReachTimestamp = GameTime::GetTime();
				AddDesignation(stc);
			}
		}
	}
}

bool DesignationRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	m_mapDesignationInfo.clear();
	if (poRoleAllInfo->has_designatinorecord())
	{
		const KKSG::Designation2DB* record = &poRoleAllInfo->designatinorecord();
		m_dwAbilityDesignationID = record->abilitydesignationid();
		m_dwCoverDesignationID = record->coverdesignationid();
		m_dwMaxAbilityDesignationID = record->maxabilitydesignationid();
		for (int i = 0; i < record->designationdata_size(); ++i)
		{
			UINT32 nDesID =  record->designationdata(i).designationid();
			if(Illegal(nDesID))
			{
				LogInfo("Load filter invalid designation--nRoleID=%llu  nDesID=%d",m_pRole->GetID(),nDesID);
				continue;
			}
			stDesignationInfo stc;  
			stc.dwDesignationID = record->designationdata(i).designationid();
			stc.dwReachTimestamp = record->designationdata(i).reachtimestamp();
			stc.bIsNew = record->designationdata(i).isnew();
			stc.szName = record->designationdata(i).name();
			stc.dwType = record->designationdata(i).type();
			LogDebug("Load DesId=%u  DesName=%s", stc.dwDesignationID, stc.szName.c_str());
			m_mapDesignationInfo.insert( std::make_pair(stc.dwDesignationID, stc) );
		}
	}
	///>SSWarn << "Update Player Cover Designation, RoleID:" << (*poRoleAllInfo).brief().roleid() << ", CoverID:" << m_dwCoverDesignationID << END;
	return true;
}

void DesignationRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if(!m_ismodify.TestAndReset())
	{
		return; 
	}
	KKSG::Designation2DB* record = poRoleAllInfo->mutable_designatinorecord();
	record->Clear();
	record->set_coverdesignationid(m_dwCoverDesignationID);
	record->set_abilitydesignationid(m_dwAbilityDesignationID);
	record->set_maxabilitydesignationid(m_dwMaxAbilityDesignationID);
	for (auto iter = m_mapDesignationInfo.begin(); iter != m_mapDesignationInfo.end();)
	{
		UINT32 nDesID =  iter->second.dwDesignationID;
		if(Illegal(nDesID))
		{
			LogInfo("Save filter invalid designation--nRoleID=%llu  nDesID=%d",m_pRole->GetID(),nDesID);
			m_mapDesignationInfo.erase(iter++);
			continue;
		}
		KKSG::StcDesignationInfo* pInfo = record->add_designationdata();
		pInfo->set_designationid(iter->second.dwDesignationID);
		pInfo->set_reachtimestamp(iter->second.dwReachTimestamp);
		pInfo->set_isnew(iter->second.bIsNew);
		pInfo->set_name(iter->second.szName);
		pInfo->set_type(iter->second.dwType);
		LogDebug("Save DesId=%u  DesName=%s", iter->second.dwDesignationID, iter->second.szName.c_str());
		iter++;
	}

	roChanged.insert(record);
}

void DesignationRecord::Init()
{
	m_dwCoverDesignationID = 0; 
	m_dwAbilityDesignationID = 0; 
	m_mapDesignationInfo.clear();
	m_ismodify.Set();
}

stDesignationInfo* DesignationRecord::GetDesignationInfo(UINT32 designationID)
{
	auto iter = m_mapDesignationInfo.find(designationID);
	if (iter==m_mapDesignationInfo.end())
	{
		return NULL;
	}
	return &iter->second;
}

bool DesignationRecord::AddDesignation(stDesignationInfo& info)
{
	m_mapDesignationInfo[info.dwDesignationID] = info;
	Change();
	CheckUpdateMaxDesignationID(info.dwDesignationID);
	DoLog(info.dwDesignationID, TX_ADD);
	return true;
}

bool DesignationRecord::SetName(UINT32 designationID,std::string &name)
{
	if(0==designationID)
		return false;
	auto iter = m_mapDesignationInfo.find(designationID);
	if (iter != m_mapDesignationInfo.end())
	{
		iter->second.szName = name;
		Change();
	}
	if (m_dwCoverDesignationID == designationID)
	{
		SetCoverDesignation(designationID);
	}
	return true;
}

std::string DesignationRecord::GetName()
{
	std::string szName = "";
	stDesignationInfo* pInfo = GetDesignationInfo(m_dwCoverDesignationID);
	if(pInfo&&m_dwCoverDesignationID==226)
	{
		szName = GetGlobalConfig().GetCoverName(pInfo->szName,pInfo->dwType);
	}
	if (pInfo&&m_dwCoverDesignationID==262)
	{
		szName = pInfo->szName + GetGlobalConfig().DragonGuildDesignation;
	}
	return szName;
}

std::string DesignationRecord::GetName(UINT32 nDesID)
{
	std::string szName = "";
	stDesignationInfo* pInfo = GetDesignationInfo(nDesID);
	if(pInfo&&nDesID==226)
	{
		szName = GetGlobalConfig().GetCoverName(pInfo->szName,pInfo->dwType);
	}
	if (pInfo&&m_dwCoverDesignationID==262)
	{
		szName = pInfo->szName + GetGlobalConfig().DragonGuildDesignation;
	}
	return szName;
}



bool DesignationRecord::DelDesignation(UINT32 designationID)
{
	if(!designationID)
		return false;
	bool deleteCover = false;
	bool deleteAbility = false;
	m_mapDesignationInfo.erase(designationID);
	if (designationID == m_dwCoverDesignationID)
	{
		m_dwCoverDesignationID = 0;
		deleteCover = true;
	}
	if (designationID == m_dwAbilityDesignationID)
	{
		m_dwAbilityDesignationID = 0;
		deleteAbility = true;
	}
	Change();
	if (deleteCover||designationID==226||designationID==262)
	{
		SetCoverDesignation(0);
	}
	if (deleteAbility)
	{
		SetAbilityDesignation(0);
	}
	if (designationID == m_dwMaxAbilityDesignationID)
	{
		CalcMaxDesignation();
	}
	DoLog(designationID, TX_REDUCE);
	return deleteCover;
}

bool DesignationRecord::SetAbilityDesignation(UINT32 designationID)
{
	bool recalc = false;
	if(designationID && m_mapDesignationInfo.find(designationID) != m_mapDesignationInfo.end())
	{
		m_dwAbilityDesignationID = designationID;
		Change();
		recalc = true;
	}

	if (!designationID)
	{
		m_dwAbilityDesignationID = 0;
		Change();
		recalc = true;
	}

	if (recalc)
	{
		m_pRole->RecalcuateAttributeAndNotify(RecalAttr_Designation);
		RoleSummaryMgr::Instance()->UpdateRoleSummary(m_pRole, SUMMARY_ATTR);
		RoleOutLook outlook(m_pRole);
		outlook.Fill(SUMMARY_ATTR, NULL);
		outlook.BroadCast();
	}

	return true;
}

std::string  DesignationRecord::SetCoverDesignation(UINT32 designationID)
{
	std::string szName = "";
	bool bFlag = false;
	if(designationID && m_mapDesignationInfo.find(designationID) != m_mapDesignationInfo.end())
	{
		m_dwCoverDesignationID = designationID;
		Change();
		bFlag = true;
	}

	if (!designationID)
	{
		m_dwCoverDesignationID = 0;
		Change();
		bFlag = true;
	}

	if (bFlag)
	{
		RoleSummaryMgr::Instance()->UpdateRoleSummary(m_pRole, SUMMARY_DESIGNATION, (void*)&designationID);
		RoleOutLook outlook(m_pRole);
		outlook.Fill(SUMMARY_DESIGNATION, NULL);
		outlook.BroadCast();
		DesignationMgr::Instance()->SendCoverDesignationNotify(m_pRole);
		szName = GetName();
	}
	return szName;
}

UINT32 DesignationRecord::GetMaxDesignationID()
{
	return m_dwMaxAbilityDesignationID;
}

bool DesignationRecord::IsCurAbilityDesignationMax()
{
	if(m_dwAbilityDesignationID == m_dwMaxAbilityDesignationID)
		return true;
	else
	{
		UINT32 tempPPt = DesignationMgr::Instance()->GetDesignationPower(m_pRole->GetProfession(), m_dwAbilityDesignationID);
		UINT32 maxPPt = DesignationMgr::Instance()->GetDesignationPower(m_pRole->GetProfession(), m_dwMaxAbilityDesignationID);
		if (tempPPt >= maxPPt)
		{
			return true;
		}

		return false;
	}
}

bool DesignationRecord::CheckUpdateMaxDesignationID(UINT32 designationID /*= 0*/)
{
	UINT32 tempPPt = DesignationMgr::Instance()->GetDesignationPower(m_pRole->GetProfession(), designationID);
	UINT32 maxPPt = DesignationMgr::Instance()->GetDesignationPower(m_pRole->GetProfession(), m_dwMaxAbilityDesignationID);
	if (tempPPt > maxPPt)
	{
		m_dwMaxAbilityDesignationID = designationID;
		Change();
		return true;
	}

	return false;
}

UINT32 DesignationRecord::CalcMaxDesignation()
{
	UINT32 maxppt = 0; 
	UINT32 maxDesignationID = 0;
	for (auto iter = m_mapDesignationInfo.begin(); iter != m_mapDesignationInfo.end(); ++iter)
	{
		UINT32 curPPt = DesignationMgr::Instance()->GetDesignationPower(m_pRole->GetProfession(), iter->first);
		if(curPPt > maxppt)
		{
			maxppt = curPPt;
			maxDesignationID = iter->second.dwDesignationID;
		}
	}
	if (m_dwMaxAbilityDesignationID != maxDesignationID)
	{
		m_dwMaxAbilityDesignationID = maxDesignationID;
		Change();
	}

	return m_dwMaxAbilityDesignationID;
}

void DesignationRecord::Change()
{
	m_ismodify.Set();
}

void DesignationRecord::DoLog(UINT32 designationID, INT32 iOperType)
{
	TDesignationFlow oLog(m_pRole);
	oLog.m_uDesignationID = designationID;
	oLog.m_iOperType = iOperType;
	oLog.Do();
}

bool DesignationRecord::HasDes(UINT32 nDesID)
{
	auto iter = m_mapDesignationInfo.find(nDesID);
	if (iter!=m_mapDesignationInfo.end())
	{
		return true;
	}
	return false;
}
