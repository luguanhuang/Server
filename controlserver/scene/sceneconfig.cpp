#include "pch.h"
#include "sceneconfig.h"


INSTANCE_SINGLETON(CSceneConfig)


CSceneConfig::CSceneConfig()
{
}

CSceneConfig::~CSceneConfig()
{
}

bool CSceneConfig::Init()
{
	return LoadFile();
}

void CSceneConfig::Uninit()
{
	ClearFile();
}

bool CSceneConfig::CheckFile()
{
	SceneTable tmpSceneTable;
	if (!tmpSceneTable.LoadFile("table/SceneList.txt"))
	{
		LogWarn("load scene config [table/SceneList.txt] failed!");
		return false;
	}
	return true;
}

bool CSceneConfig::LoadFile()
{
	if(!CheckFile())
	{
		return false;
	}

	//ClearFile();

	SceneTable tmpSceneTable;
	tmpSceneTable.LoadFile("table/SceneList.txt");
	m_oTable.CopyFrom(&tmpSceneTable);

	return true;
}

void CSceneConfig::ClearFile()
{
	m_oTable.Clear();
}

const SceneConf* CSceneConfig::GetSceneConf(UINT32 dwTemplateID)
{
	return m_oTable.GetBySceneID(dwTemplateID);
}
