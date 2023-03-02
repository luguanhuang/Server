#include "pch.h"
#include "XBuffClear.h"
#include "entity/XEntity.h"
#include "unit/unit.h"
#include "foreach.h"
#include "component/XBuffComponent.h"

bool XBuffClear::TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff)
{
	if (pBuffInfo->ClearTypes.empty())
		return false;

	pBuff->AddBuffEffect(new XBuffClear(pBuffInfo->ClearTypes));
	return true;
}

XBuffClear::XBuffClear(const vector<int>& _types)
{
	m_Types.clear();
	for(uint i = 0; i < _types.size(); ++i)
	{
		if(_types[i] == 0)
			continue;
		m_Types.insert(_types[i]);
	}
}


void XBuffClear::OnAdd(XEntity* pEntity, CombatEffectHelper* pEffectHelper)
{
	m_Entity = pEntity;

	XBuffComponent* buffComponent = pEntity->BuffComponent();
	if (buffComponent == NULL)
		return;
	const std::list<XBuff*>& bufflist = buffComponent->GetBuffList();

	foreach(buff in bufflist)
	{
		XBuff* pBuff = (*buff);
		if (pBuff->IsValid() && m_Types.count(pBuff->GetClearType()) > 0)
		{
			XRemoveBuffActionArgs args;
			args.nBuffId = pBuff->GetID();
			pEntity->DispatchAction(&args);
		}
	}

}

bool XBuffClear::CanBuffAdd(BuffTable::RowData * pBuffData)
{
	if(m_Types.count(pBuffData->BuffClearType) > 0)
		return false;
	return true;
}
