#include "pch.h"
#include "sceneleisureconfig.h"
#include "util/XRandom.h"

INSTANCE_SINGLETON(SceneLeisureConfig);

bool SceneLeisureConfig::Init()
{
	return LoadFile();
}

void SceneLeisureConfig::Uninit()
{

}

bool SceneLeisureConfig::CheckFile()
{
	YorozuyaTable temptable;
	if (!temptable.LoadFile("table/Yorozuya.txt"))
	{
		SSWarn<<"load file table Yorozuya.txt failed"<<END;
		return false;
	}
	return true;
}

bool SceneLeisureConfig::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}
	ClearFile();
	m_table.LoadFile("table/Yorozuya.txt");

	for (auto i = m_table.Table.begin(); i != m_table.Table.end(); ++i)
	{
		YorozuyaTable::RowData* data = *i;
		if (data->IsOpen)
		{
			for (auto j = data->SceneIDs.begin(); j != data->SceneIDs.end(); ++j)
			{
				m_notopen.insert(j->seq[0]);
			}
		}
	}
	return true;
}

void SceneLeisureConfig::ClearFile()
{
	m_table.Clear();
	m_notopen.clear();
}

UINT32 SceneLeisureConfig::GetSceneId(UINT32 index, UINT32 exceptsceneid)
{
	YorozuyaTable::RowData* data = m_table.GetByID(index);
	if (NULL == data)
	{
		return 0;
	}
	if (data->IsOpen)
	{
		return (UINT32)(-1);
	}
	UINT32 all = 0;
	for (auto i = data->SceneIDs.begin(); i != data->SceneIDs.end(); ++i)
	{
		if (i->seq[0] == exceptsceneid)
		{
			continue;
		}
		if (!IsOpen(i->seq[0]))
		{
			continue;
		}
		all += i->seq[1];
	}
	UINT32 random = XRandom::randInt(0, all + 1);
	all = 0;
	for (auto i = data->SceneIDs.begin(); i != data->SceneIDs.end(); ++i)
	{
		if (i->seq[0] == exceptsceneid)
		{
			continue;
		}
		if (!IsOpen(i->seq[0]))
		{
			continue;
		}
		if (all <= i->seq[1])
		{
			return i->seq[0];
		}
	}
	return 0;
}

bool SceneLeisureConfig::IsOpen(UINT32 sceneid)
{
	return m_notopen.end() == m_notopen.find(sceneid);
}
