#include "pch.h"
#include "scenetransconfig.h"
#include "util/XCommon.h"

INSTANCE_SINGLETON(SceneTransConfig);

bool SceneTransConfig::Init()
{
	return LoadFile();
}

void SceneTransConfig::Uninit()
{

}

bool SceneTransConfig::CheckFile()
{
	SceneTrans temptable;
	if (!temptable.LoadFile("table/SceneTrans.txt"))
	{
		SSWarn<<"load table table/SceneTrans.txt failed"<<END;
		return false;
	}
	return true;
}

bool SceneTransConfig::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_table.LoadFile("table/SceneTrans.txt");

	for (auto i = m_table.Table.begin(); i != m_table.Table.end(); ++i)
	{
		SceneTrans::RowData* data = *i;
		TransConfig conf;
		conf.interval = data->Interval;
		conf.randombuff = data->Buff;

		std::string string_begin = data->Time[0];
		std::string string_end = data->Time[1];
		
		UINT32 begin = (UINT32)XCommon::ParseTime(string_begin);
		UINT32 end = (UINT32)XCommon::ParseTime(string_end);
		conf.timespan[0] = begin;
		conf.timespan[1] = end;

		m_id2config[data->SceneId] = conf;
	}
	return true;
}

void SceneTransConfig::ClearFile()
{
	m_table.Clear();
	//m_id2config.clear();
}

TransConfig* SceneTransConfig::GetConfig(UINT32 sceneid)
{
	auto i = m_id2config.find(sceneid);
	if (i == m_id2config.end())
	{
		return NULL;
	}
	return &i->second;
}
