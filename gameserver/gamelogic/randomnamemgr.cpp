#include "pch.h"
#include "randomnamemgr.h"
#include "util/XRandom.h"

INSTANCE_SINGLETON(RandomNameMgr);


RandomNameMgr::RandomNameMgr()
{

}

RandomNameMgr::~RandomNameMgr()
{

}	

bool RandomNameMgr::Init()
{
	if (!m_table.LoadFile("table/RandomName.txt"))
	{
		SSWarn<<"load file failed table/RandomName.txt"<<END;
		return false;
	}
	for (auto i = m_table.Table.begin(); i != m_table.Table.end(); ++i)
	{
		RandomName::RowData* rowdata = *i;
		std::string name = rowdata->FirstName + " . " + rowdata->LastName;
		m_names.push_back(name);
	}
	if (!m_renametable.LoadFile("table/RenameList.txt"))
	{
		SSWarn<<"load file failed table/RenameList.txt"<<END;
		return false;
	}
	return true;
}

void RandomNameMgr::Uninit()
{

}

std::string& RandomNameMgr::Random(const std::string& name)
{
	UINT32 index = XRandom::randInt(0, m_names.size() - 1);
	std::string& tempname = m_names[index];
	if (name == tempname)
	{
		++index;
	}
	return m_names[index];
}

UINT32 RandomNameMgr::GetRenameConsume(UINT32 count)
{
	if (0 == count || m_renametable.Table.empty())
	{
		return 0;
	}
	if (count > m_renametable.Table.size())
	{
		count = m_renametable.Table.size();
	}	
	return m_renametable.Table[count - 1]->cost;
}
