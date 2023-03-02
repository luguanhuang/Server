#include "pch.h"
#include "unit/role.h"
#include "perioddesi.h"
#include "util/gametime.h"
#include "define/systemiddef.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "unit/systeminfo.h"
#include "define/itemdef.h"
#include "bagtransition.h"
#include "gamelogic/stagemgr.h"
#include "gamelogic/designationMgr.h"



INSTANCE_SINGLETON(PeriodDesiMgr);

PeriodDesiMgr::PeriodDesiMgr(){}

PeriodDesiMgr::~PeriodDesiMgr(){}

bool PeriodDesiMgr::Init()
{
	bool bRet = LoadFile();
	return bRet;
}

void PeriodDesiMgr::Uninit()
{
	ClearFile();
}

void PeriodDesiMgr::ClearFile()
{
	m_Table.Clear();
}

bool PeriodDesiMgr::LoadFile()
{
	ClearFile();
	bool bRet = m_Table.LoadFile("table/PeriodDesignation.txt");
	return true;
}


void PeriodDesiMgr::OnTriger(Role *pRole,UINT32 nDesID)
{
	PeriodDesignation::RowData *pRowData =  m_Table.GetByDesID(nDesID);
	if (pRowData == NULL)
		return;
	UINT32 nTime = GameTime::GetTime();
	UINT32 starttime	= XCommon::ParseTime(pRowData->Time[0]);
	UINT32 endtime	= XCommon::ParseTime(pRowData->Time[1]);
	if (nTime >= starttime&&nTime<=endtime)
	{
		StageMgr *pStage =  pRole->Get<StageMgr>();
		UINT32 nCount	 =  pStage->GetKidHelperCount();
		if (nCount>=pRowData->Param[0])
			return;
		nCount++;
		pStage->SetKidHelperCount(nCount);
		if (nCount>=pRowData->Param[0])
			DesignationMgr::Instance()->OnUseItem(pRole,pRowData->DesID);
	}
}

