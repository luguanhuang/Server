#include "pch.h"
#include "SpriteConfigMgr.h"

INSTANCE_SINGLETON(CSpriteConfigMgr)

CSpriteConfigMgr::CSpriteConfigMgr()
{

}

CSpriteConfigMgr::~CSpriteConfigMgr()
{

}

bool CSpriteConfigMgr::Init()
{
	return LoadFile();
}

void CSpriteConfigMgr::Uninit()
{

}

bool CSpriteConfigMgr::CheckFile()
{
	SpriteTable m_tmpSpriteTable;
	if (!m_tmpSpriteTable.LoadFile("table/SpriteTable.txt"))
	{
		LogWarn("Load SpriteTable.txt failed!");
		return false;
	}
	return true;
}

bool CSpriteConfigMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_oSpriteTable.LoadFile("table/SpriteTable.txt");
	
	return true;
}

void CSpriteConfigMgr::ClearFile()
{
	m_oSpriteTable.Clear();
}