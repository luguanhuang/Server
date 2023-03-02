#include "pch.h"
#include "StringTableMgr.h"

INSTANCE_SINGLETON(StringTableMgr);

StringTableMgr::StringTableMgr()
{

}

StringTableMgr::~StringTableMgr()
{

}

bool StringTableMgr::Init()
{
	return LoadFile();
}

void StringTableMgr::Uninit()
{
	ClearFile();
}

bool StringTableMgr::CheckFile()
{
	StringTable tmpStringTable;
	if (!tmpStringTable.LoadFile("table/StringTable.txt"))
	{
		LogWarn("Load StringTable.txt failed!");
		return false;
	}

	return true;
}

bool StringTableMgr::LoadFile()
{
	if(!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_oTable.LoadFile("table/StringTable.txt");

	return true;
}

void StringTableMgr::ClearFile()
{
	m_oTable.Clear();
}

std::string StringTableMgr::GetText(std::string Enum)
{
	auto i = m_oTable.GetByEnum(Enum);
	if(NULL == i)
	{
		return "";
	}
	return i->Text;
}