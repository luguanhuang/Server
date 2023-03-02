#include "pch.h"
#include "prerogativemgr.h"
#include "pb/enum.pb.h"
#include "util/gametime.h"
#include "table/globalconfig.h"

INSTANCE_SINGLETON(CPreprogativeConfig)


CPreprogativeConfig::CPreprogativeConfig()
{

}

CPreprogativeConfig::~CPreprogativeConfig()
{

}

bool CPreprogativeConfig::Init()
{
	if (!m_oTable.LoadFile("table/PrerogativeContent.txt"))
	{
		LogWarn("Load table %s failed!", "table/PrerogativeContent.txt");
		return false;
	}
	return CheckFile();
}

bool CPreprogativeConfig::CheckFile()
{
	return true;
}

void CPreprogativeConfig::Uninit()
{

}

bool CPreprogativeConfig::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}
	ClearFile();
	m_oTable.LoadFile("table/PrerogativeContent.txt");
	return true;
}

void CPreprogativeConfig::ClearFile()
{
	m_oTable.Clear();
}


