#include "pch.h"
#include "doodaditemskillconfig.h"

CDoodadItemSkillConfig::CDoodadItemSkillConfig()
{

}

CDoodadItemSkillConfig::~CDoodadItemSkillConfig()
{

}

bool CDoodadItemSkillConfig::Init()
{
	if(!LoadFile())
	{
		return false;
	}
	return true;
}

void CDoodadItemSkillConfig::Uninit()
{

}

bool CDoodadItemSkillConfig::LoadFile()
{
	if(!CheckConfig())
	{
		return false;
	}

	ClearConfig();

	m_table.LoadFile("table/DoodadItemSkill.txt");

	return true;
}

bool CDoodadItemSkillConfig::CheckConfig()
{
	DoodadItemSkillTable tmptable;
	if(!tmptable.LoadFile("table/DoodadItemSkill.txt"))
	{
		SSError << " load file DoodadItemSkill.txt failed " << END;
		return false;
	}

	return true;
}

void CDoodadItemSkillConfig::ClearConfig()
{
	m_table.Clear();
}

UINT32 CDoodadItemSkillConfig::GetAllSkillCount(int sceneType)
{
	switch(sceneType)
	{
	case KKSG::SCENE_HORSE_RACE:
	case KKSG::SCENE_WEEKEND4V4_HORSERACING:
		{
			return 2;
		}
		break;
	case KKSG::SCENE_SURVIVE:
		{
			return 4;
		}
		break;
	default:
		break;
	}

	return 0;
}

UINT32 CDoodadItemSkillConfig::GetItemOverlapCount(UINT32 itemID, int sceneType)
{
	auto pConf = GetConf(sceneType, itemID);
	if(pConf)
	{
		return pConf->overlap;
	}
	return 1;//默认1 兼容原有玩法
}

DoodadItemSkillTable::RowData* CDoodadItemSkillConfig::GetConf(int sceneType, UINT32 itemID)
{
	for(size_t i = 0; i < m_table.Table.size(); ++i)
	{
		auto pConf = m_table.Table[i];
		if(pConf->sceneType == sceneType && pConf->itemId == itemID)
		{
			return pConf;
		}
	}
	return NULL;
}

bool CDoodadItemSkillConfig::CanHaveSameItem(int sceneType)
{
	if(KKSG::SCENE_SURVIVE == sceneType)
	{
		return false;
	}
	return true;
}

bool CDoodadItemSkillConfig::IsUseNotify(int sceneType)
{
	if(KKSG::SCENE_SURVIVE == sceneType)
	{
		return false;
	}
	return true;
}
