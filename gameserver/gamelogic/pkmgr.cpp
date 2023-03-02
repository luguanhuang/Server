#include "pch.h"
#include "pkmgr.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "pkrecord.h"
#include "define/systemiddef.h"
#include "define/itemdef.h"

INSTANCE_SINGLETON(PkMgr);

#define PAIR_MAX 2

PkMgr::PkMgr()
{

}

PkMgr::~PkMgr()
{

}

bool PkMgr::Init()
{
	if (!m_pointtable.LoadFile("table/PkPointReward.txt"))
	{
		SSWarn<<"load file table/PkPointReward.txt failed"<<END;
		return false;
	}
	if (!m_paramtable.LoadFile("table/PkParam.txt"))
	{
		SSWarn<<"load file table/PkParam.txt failed"<<END;
		return false;
	}

	return true;
}

void PkMgr::Uninit()
{

}

bool PkMgr::CheckFile()
{
	PkParamTable temptable;
	if (!temptable.LoadFile("table/PkParam.txt"))
	{
		SSWarn<<"load file table/PkParam.txt failed"<<END;
		return false;
	}
	return true;
}

bool PkMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}
	ClearFile();
	if (!m_paramtable.LoadFile("table/PkParam.txt"))
	{
		SSWarn<<"load file table/PkParam.txt failed"<<END;
		return false;
	}
	return true;
}

void PkMgr::ClearFile()
{
	m_paramtable.Clear();
}

UINT32 PkMgr::GetPrepareTime(UINT32 point)
{
	UINT32 count = 0;
	for (auto i = RoleManager::Instance()->Begin(); i != RoleManager::Instance()->End(); ++i)
	{
		Role* role = *i;
		CPkRecord* record = role->Get<CPkRecord>();
		record->Update();
		UINT32 temppoint = record->GetPoint();
		if (abs((int)temppoint - (int)point) > (int) GetGlobalConfig().PkTime[0])
		{
			continue;
		}
		++count;
	}
	return CalculateTime(count);
}

UINT32 PkMgr::CalculateTime(UINT32 count)
{
	float percent = float(count)/float( GetGlobalConfig().PkTime[2]);
	if (percent >= 1.0f)
	{
		return  GetGlobalConfig().PkTime[4];
	}
	UINT32 sec =  GetGlobalConfig().PkTime[4] + (UINT32)((1 - percent)*( GetGlobalConfig().PkTime[3] -  GetGlobalConfig().PkTime[4]));
	return sec;
}

//bool PkMgr::InPrepare(UINT64 roleid)
//{
//	auto i = find_if(m_preparelist.begin(), m_preparelist.end(), EqualPkInfo(roleid));
//	if (i != m_preparelist.end())
//	{
//		return true;
//	}
//	auto j = find_if(m_preparerobotlist.begin(), m_preparerobotlist.end(), EqualPkInfo(roleid));
//	if (j != m_preparerobotlist.end())
//	{
//		return true;
//	}
//	return false;
//}
//
void PkMgr::GetPointReward(UINT32 point, UINT32 index, std::vector<ItemDesc>& items)
{
	if (index >= m_pointtable.Table.size())
	{
		return;
	}
	PkPointTable::RowData* rowdata = m_pointtable.Table[index];
	if (rowdata->point > point)
	{
		return;
	}
	for (auto i = rowdata->reward.begin(); i != rowdata->reward.end(); ++i)
	{
		items.push_back(ItemDesc((int)i->seq[0], (int)i->seq[1]));
	}
	return;
}

void PkMgr::HintNotify(Role* role)
{
	CPkRecord* record = role->Get<CPkRecord>();
	record->Update();
	UINT32 point = record->GetMaxPoint();
	for (UINT32 i = 0; i < m_pointtable.Table.size(); ++i)
	{
		std::vector<ItemDesc> items;
		GetPointReward(point, i, items);
		if (!items.empty() && !record->IsBoxTaken(i))
		{
			role->HintNotify(PkId, false);
			break;
		}
	}
}

UINT32 PkMgr::GetParam(UINT32 point, bool iswin)
{
	for (auto i = m_paramtable.Table.begin(); i != m_paramtable.Table.end(); ++i)
	{
		PkParamTable::RowData* rowdata = *i;
		if (point >= rowdata->min && rowdata->max >= point)
		{
			return iswin ? rowdata->winK : rowdata->loseK;
		}
	}
	return 40;
}

int PkMgr::GetPoint(UINT32 myPoint, int pointSpan, KKSG::PkResultType ret)
{
	if (ret == KKSG::PkResult_Draw)
	{
		return 0;
	}

	bool iswin = (ret == KKSG::PkResult_Win) ? true : false;
	float expect = 1/(1 + pow(10, (-(float)pointSpan/(float)400)));
	float W = iswin ? 1.0f : 0.0f;
	float K = (float)GetParam(myPoint, iswin);
	int point = (int)(K*(W - expect));
	return point;
}
