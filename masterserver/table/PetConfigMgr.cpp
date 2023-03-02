#include "pch.h"
#include "PetConfigMgr.h"

INSTANCE_SINGLETON(CPetConfigMgr)

CPetConfigMgr::CPetConfigMgr()
{

}

CPetConfigMgr::~CPetConfigMgr()
{

}

bool CPetConfigMgr::Init()
{
	return LoadFile();
}

void CPetConfigMgr::Uninit()
{

}

static bool HandlerTableData(PetInfoTable& table)
{
	if (!table.LoadFile("table/PetInfo.txt"))
	{
		SSWarn<<"load file table/PetInfo.txt failed"<<END;
		return false;
	}
	return true;
}

bool CPetConfigMgr::CheckFile()
{
	PetInfoTable table;
	return HandlerTableData(table);
}

bool CPetConfigMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	HandlerTableData(m_table);
	
	return true;
}

void CPetConfigMgr::ClearFile()
{
	m_table.Clear();
}