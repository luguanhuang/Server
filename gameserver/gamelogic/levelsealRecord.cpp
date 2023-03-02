#include "pch.h"
#include <time.h>
#include "levelsealRecord.h"
#include "pb/project.pb.h"
#include "globalconfig.h"
#include "unit/role.h"
#include "gamelogic/levelsealMgr.h"
#include "config.h"

CLevelSealRecord::CLevelSealRecord(Role* role)
{
	m_pRole = role;
	m_uType = 0;
	m_uSelfCollectCount = 0;
	m_iSelfAwardCountIndex = -1;
	m_uLevelSealButtonStatus = 0;
	m_bLastLevelSealStatus = false;
}
CLevelSealRecord::~CLevelSealRecord()
{

}

void CLevelSealRecord::FirstInit(UINT32 roleNum)
{
	if (GSConfig::Instance()->IsBackFlowServer() && roleNum == 1)
	{
		SetButtonStatus(KKSG::BUTTON_STATUS_UNLOCKED);//½â·âÎ´µã»÷
	}
}
bool CLevelSealRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_levelsealdata())
	{
		m_uType = poRoleAllInfo->levelsealdata().type();
		m_uSelfCollectCount = poRoleAllInfo->levelsealdata().selfcollectcount();
		m_iSelfAwardCountIndex = poRoleAllInfo->levelsealdata().selfawardcountindex();
		m_uLevelSealButtonStatus = poRoleAllInfo->levelsealdata().levelsealbuttonstatus();
		m_bLastLevelSealStatus = poRoleAllInfo->levelsealdata().lastlevelsealstatus();
	}
	return true;
}

void CLevelSealRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_ismodify.TestAndReset())
	{
		KKSG::LevelSealRecord* pData = poRoleAllInfo->mutable_levelsealdata();
		pData->set_type(m_uType);
		pData->set_selfcollectcount(m_uSelfCollectCount);
		pData->set_selfawardcountindex(m_iSelfAwardCountIndex);
		pData->set_levelsealbuttonstatus(m_uLevelSealButtonStatus);
		pData->set_lastlevelsealstatus(m_bLastLevelSealStatus);
		roChanged.insert(pData);
	}
}

void CLevelSealRecord::Change()
{
	m_ismodify.Set();
}

UINT32 CLevelSealRecord::GetButtonStatus()
{
	return m_uLevelSealButtonStatus;
}
void CLevelSealRecord::SetButtonStatus(UINT32 uStatus)
{
	m_uLevelSealButtonStatus = uStatus;
	Change();
}
bool CLevelSealRecord::GetLastStatus()
{
	return m_bLastLevelSealStatus;
}
void CLevelSealRecord::SetLastStatus(bool bStatus)
{
	m_bLastLevelSealStatus = bStatus;
	Change();
}
UINT32 CLevelSealRecord::GetSelfCollectCount()
{
	return m_uSelfCollectCount;
}

INT32 CLevelSealRecord::GetSelfAwardCountIndex()
{
	return m_iSelfAwardCountIndex;
}

UINT32 CLevelSealRecord::GetType()
{
	return m_uType;
}

void CLevelSealRecord::CheckType(INT32 iType)
{
	if (iType && iType != m_uType)
	{
		m_uType = iType;
		m_uSelfCollectCount = 0;
		m_iSelfAwardCountIndex = -1;
		Change();
	}
}
void CLevelSealRecord::AddSelfCollectCount(UINT32 Count)
{
	m_uSelfCollectCount += Count;
	Change();
}

void CLevelSealRecord::SetSelfAwardCountIndex(INT32 iIndex)
{
	m_iSelfAwardCountIndex = iIndex;
	Change();
}
