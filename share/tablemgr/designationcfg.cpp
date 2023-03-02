#include "pch.h"
#include "designationcfg.h"
#include "table/globalconfig.h"

DesignationTableMgr::DesignationTableMgr()
{

}

DesignationTableMgr::~DesignationTableMgr()
{

}

bool DesignationTableMgr::Init()
{
	if (!LoadFile())
	{
		return false;
	}
	return true;
}

void DesignationTableMgr::Uninit()
{
	ClearFile();
}

bool DesignationTableMgr::CheckFile()
{
	DesignationTable tmp1;
	if (!tmp1.LoadFile("table/Designation.txt"))
	{
		LogError("load table/Designation.txt failed");
		return false;
	}
	return true;
}

bool DesignationTableMgr::LoadFile()
{
	if (!CheckFile())	
	{
		return false;
	}
	m_Table.LoadFile("table/Designation.txt");
	return true;
}

void DesignationTableMgr::ClearFile()
{
	m_Table.Clear();
}



