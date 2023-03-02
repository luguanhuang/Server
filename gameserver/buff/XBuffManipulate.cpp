#include "pch.h"
#include "XBuffManipulate.h"
#include "entity/XEntity.h"
#include "unit/unit.h"
#include "table/SkillReader.h"

bool XBuffManipulate::TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff)
{
	if (pBuffInfo->Manipulate.size() == 0)
		return false;

	pBuff->AddBuffEffect(new XBuffManipulate(pBuff));
	return true;
}

XBuffManipulate::XBuffManipulate(XBuff* pBuff)
	: m_pEntity(NULL)
	, m_pBuff(pBuff)
	, m_Token(0)
	, m_pData(NULL)
{

}

void XBuffManipulate::OnAdd(XEntity* pEntity, CombatEffectHelper* pEffectHelper)
{
	m_pEntity = pEntity;

	m_pData = new XManipulationData();
	m_pData->Degree = 360;
	m_pData->Force = _GetParam(0);
	m_pData->Radius = _GetParam(1);
	m_pData->TargetIsOpponent = ((int)_GetParam(2)) == 0;

	XManipulationOnArgs args(m_pData);
	m_Token = (int)(args.GetToken());
	m_pEntity->DispatchAction(&args);
}

void XBuffManipulate::OnRemove(XBuffRemoveReason reason)
{
	XManipulationOffArgs args;
	args.DenyToken = m_Token;
	m_pEntity->DispatchAction(&args);

	if (m_pData)
	{
		delete m_pData;
		m_pData = NULL;
	}
}

float XBuffManipulate::_GetParam(int index)
{
	if(m_pBuff->GetBuffInfo()->Manipulate.size() <= (size_t)index)
		return 0.0f;
	return m_pBuff->GetBuffInfo()->Manipulate[index];
}
