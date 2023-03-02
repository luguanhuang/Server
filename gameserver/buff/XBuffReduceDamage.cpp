#include "pch.h"
#include "XBuffReduceDamage.h"
#include "gamelogic/XCombat.h"
#include "entity/XEntity.h"
#include "unit/unit.h"
#include "foreach.h"

bool XBuffReduceDamage::OnBuffEffect(const HurtInfo &hurtInfo, ProjectDamageResult &result)
{
	double hp = hurtInfo.m_target->GetUnit()->GetPreciseHpPercent() / 100;
	result.ScaleValue(1.0 - m_Data.GetData(hp));
	//LogInfo("reduce damage from %d -> %d", oldvalue, (int)result.m_value);
	return true;
}

bool XBuffReduceDamage::TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff)
{
	if (pBuffInfo->DamageReduce.empty())
		return false;

	pBuff->AddBuffEffect(new XBuffReduceDamage(pBuffInfo->DamageReduce, pBuffInfo->BuffID));
	return true;
}

XBuffReduceDamage::XBuffReduceDamage(const vector<Sequence<float, 2>>& reduceDamages, int buffid)
{
	m_buffID = buffid;
	m_Entity = NULL;

	m_Data.SetRange(0, 1, 1, 0);
	m_Data.Init(reduceDamages);

}

XBuffReduceDamage::~XBuffReduceDamage()
{
}

void XBuffReduceDamage::OnAttributeChanged(CombatAttrDef attr)
{
	if (attr == BASIC_CurrentHP)
	{
		if (m_Data.GetData(m_Entity->GetUnit()->GetPreciseHpPercent() / 100) < 0.00001)
		{
			XRemoveBuffActionArgs arg;
			arg.nBuffId = m_buffID;
			arg.Caster = m_Entity->GetID();
			m_Entity->DispatchAction(&arg);
		}
	}
}

void XBuffReduceDamage::OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper)
{
	m_Entity = pEntity;
}

XBuffEffectPrioriy XBuffReduceDamage::GetPriority()
{
	return XBuffEffectPrioriy_ReduceDamage;
}

XPieceWiseDataMgr::XPieceWiseDataMgr()
	: bRangeIsSet(false)
	, bDataIsSet(false)
{

}

XPieceWiseDataMgr::~XPieceWiseDataMgr()
{
	m_Datas.clear();
}

void XPieceWiseDataMgr::SetRange(double xmin, double ymin, double xmax, double ymax)
{
	m_Min.first = xmin;
	m_Min.second = ymin;

	m_Max.first = xmax;
	m_Max.second = ymax;

	bRangeIsSet = true;

	if (bDataIsSet)
	{
		m_Datas.push_back(m_Min);
		m_Datas.push_back(m_Max);
		std::sort(m_Datas.begin(), m_Datas.end());
	}
}

void XPieceWiseDataMgr::Init(const vector<Sequence<float, 2>>& datas)
{
	bool bHasMin = false;
	bool bHasMax = false;
	m_Datas.clear();
	for(auto it = datas.begin(); it != datas.end(); ++it)
	{
		PieceWiseData data;
		data.first = (double)(*it)[0];
		data.second = (double)(*it)[1];
		m_Datas.push_back(data);

		if(bRangeIsSet)
		{
			if (data.first == m_Min.first)
				bHasMin = true;
			else if (data.first == m_Max.first)
				bHasMax = true;
		}
	}

	if(bRangeIsSet)
	{
		if (!bHasMin)
		{
			m_Datas.push_back(m_Min);
		}
		if (!bHasMax)
		{
			m_Datas.push_back(m_Max);
		}
	}

	std::sort(m_Datas.begin(), m_Datas.end());
	bDataIsSet = true;
}

double XPieceWiseDataMgr::GetData(double x)
{
	for(uint i = 1; i < m_Datas.size();++i)
	{
		if (m_Datas[i].first >= x)
		{
			PieceWiseData minPercent = m_Datas[i - 1];
			PieceWiseData maxPercent = m_Datas[i];

			double delta = maxPercent.first - minPercent.first;
			if(delta == 0)
			{
				delta = 1;
			}
			return minPercent.second + (maxPercent.second - minPercent.second) * (x - minPercent.first) / delta;
		}
	}
	return 0;
}
