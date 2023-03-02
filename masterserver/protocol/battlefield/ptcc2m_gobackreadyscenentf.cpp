#include "pch.h"
#include "battlefield/ptcc2m_gobackreadyscenentf.h"
#include "battlefield/battlefieldmgr.h"
#include "role/rolemanager.h"
#include "table/MultActivityMgr.h"
#include "battlefield/battlefieldmgr.h"


// generate by ProtoGen at date: 2017/9/1 15:43:50

void PtcC2M_GoBackReadySceneNtf::Process(UINT32 dwConnID)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == pRole)
		return;	
	bool bRet	= MultActivityMgr::Instance()->IsActivityTimeOpen(MULT_ACTIVITY_BATTLEFIELD);
	if (!bRet)
		return;	
	bRet = BattleFieldMgr::Instance()->IsLevelOk(pRole);
	if (!bRet)
		return;	
	BattleFieldMgr::Instance()->EnterReadyScene(pRole);
}
