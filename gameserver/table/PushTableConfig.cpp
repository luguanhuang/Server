#include "pch.h"
#include "PushTableConfig.h"
#include "util.h"

INSTANCE_SINGLETON(PushTableConfig)

PushTableConfig::PushTableConfig()
{

}

PushTableConfig::~PushTableConfig()
{

}

bool PushTableConfig::Init()
{
	return LoadFile();
}

void PushTableConfig::Uninit()
{
	ClearFile();
}

bool PushTableConfig::CheckFile()
{
	PushSetting tmp;
	if (!tmp.LoadFile("table/PushSetting.txt"))
	{
		SSError << "Load table/PushSetting.txt failed!" << END;
		return false;
	}
	return true;
}

bool PushTableConfig::LoadFile()
{
	if(!CheckFile())
	{
		return false;
	}

	ClearFile();
	m_table.LoadFile("table/PushSetting.txt");

	for (size_t i = 0; i < m_table.Table.size(); i++)
	{
		UINT32 type = m_table.Table[i]->Type;
		std::string key = Trim(m_table.Table[i]->ConfigKey);
		m_type2Key[type] = key;
		m_key2Type[key] = type;
	}

	return true;
}

void PushTableConfig::ClearFile()
{
	m_table.Clear();
	m_type2Key.clear();
	m_key2Type.clear();
}

bool PushTableConfig::IsConfigKey(const std::string& key)
{
	return m_key2Type.find(key) != m_key2Type.end();
}

UINT32 PushTableConfig::GetType(const std::string& key)
{
	auto it = m_key2Type.find(key);
	if (it != m_key2Type.end())
	{
		return it->second;
	}
	return 0;
}

const std::map<UINT32, std::string>& PushTableConfig::GetTypeKey()
{
	return m_type2Key;
}
