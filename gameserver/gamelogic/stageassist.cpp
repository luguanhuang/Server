#include "pch.h"
#include "stageassist.h"
#include "math.h"
#include "unit/role.h"
#include "stagemgr.h"
#include "buff/XBuffTemplateManager.h"

INSTANCE_SINGLETON(StageAssistMgr);

StageAssistMgr::StageAssistMgr()
{

}

StageAssistMgr::~StageAssistMgr()
{

}

bool StageAssistMgr::Init()
{
	return LoadFile();
}

void StageAssistMgr::Uninit()
{
	ClearFile();
}

bool StageAssistMgr::CheckFile()
{
	BattleAssist tempassisttable;
	if (!tempassisttable.LoadFile("table/BattleAssist.txt"))
	{
		SSWarn<<"load file table/BattleAssist.txt failed"<<END;
		return false;
	}
	BattleBuff tempbufftable;
	if (!tempbufftable.LoadFile("table/BattleBuff.txt"))
	{
		SSWarn<<"load file table/BattleBuff.txt failed"<<END;
		return false;
	}
	return true;
}

bool StageAssistMgr::LoadFile()
{
	if (!CheckFile())	
	{
		return false;
	}
	ClearFile();

	m_assisttable.LoadFile("table/BattleAssist.txt");
	m_bufftable.LoadFile("table/BattleBuff.txt");

	return true;
}

void StageAssistMgr::ClearFile()
{
	m_assisttable.Clear();
	m_bufftable.Clear();
}

int StageAssistMgr::CalAssistPoint(const StageAssistInfo& info)
{
	int point = 0;
	BattleAssist::RowData* data = m_assisttable.GetBySceneId(info.stageid);
	if (NULL != data)
	{
		// time
		if (info.time > data->Time[0])
		{
			point += (int)data->Time[1];
		}
		// hp
		for (auto i = data->Hp.begin(); i != data->Hp.end(); ++i)
		{
			if (i->seq[0] >= info.hp)
			{
				point += (int)i->seq[1];
				break;
			}
		}
		// revive
		point += (int)(info.revive * data->Revive);
		// iswin
		point += (int)(info.iswin ? data->Stage[0] : data->Stage[1]);
		// ppt
		std::vector<Sequence<double, 2>> ppts;
		ppts = info.ppt >= 0.0l ? data->PPT1 : data->PPT2;
		for (auto j = ppts.begin(); j != ppts.end(); ++j)
		{
			if (j->seq[0] >= abs(info.ppt*100))
			{
				point = (int)(j->seq[1]*point);
				break;
			}
		}
	}
	return point;
}

BuffDesc StageAssistMgr::GetAssistBuff(Role* role, UINT32 scenetype)
{
	BuffDesc buff;
	int point = role->Get<StageMgr>()->GetStageAssistPoint();
	for (auto i = m_bufftable.Table.begin(); i != m_bufftable.Table.end(); ++i)
	{
		BattleBuff::RowData* data = *i;
		if (point <= (int)data->Point)
		{
			for (auto j = data->SceneType.begin(); j != data->SceneType.end(); ++j)
			{
				if (scenetype == *j)
				{
					buff.buffID = data->Buff[0];	
					buff.BuffLevel = data->Buff[1];	
					break;
				}
			}
			break;
		}
	}
	return buff;
}


