#include "pch.h"
#include "PlayerLevelManager.h"
#include "table/globalconfig.h"


INSTANCE_SINGLETON(PlayerLevelManager)

	PlayerLevelManager::PlayerLevelManager()
{
	m_MaxLevel = 0;
	memset(m_LevelIndex, 0, sizeof(m_LevelIndex));
}

PlayerLevelManager::~PlayerLevelManager()
{

}

bool PlayerLevelManager::Init()
{
	return LoadFile();
}

void PlayerLevelManager::Uninit()
{
	ClearFile();
}

bool PlayerLevelManager::CheckFile()
{
	PlayerLevelTable tmpPlayerLevelTable;
	if (!tmpPlayerLevelTable.LoadFile("table/PlayerLevel.txt"))
	{
		LogWarn("Load file table/PlayerLevel.txt failed!");
		return false;
	}

	return true;
}

bool PlayerLevelManager::LoadFile()
{
	if(!CheckFile())
	{
		return false;
	}

	ClearFile();

	LoadFile("table/PlayerLevel.txt");

	if (!catchup_exp_.LoadFile("table/StageCatchUpExp.txt"))
	{
		SSWarn<<"load table/StageCatchUpExp.txt failed"<<END;
		return false;
	}
	return true;
}

void PlayerLevelManager::ClearFile()
{
	m_oTable.Clear();
	memset(m_LevelIndex, 0, sizeof(m_LevelIndex));
}

PlayerLevelTable::RowData * PlayerLevelManager::GetLevelInfo(int level)
{
	if (level < 0 || level > m_MaxLevel)
		return NULL;

	return m_LevelIndex[level];
}

bool PlayerLevelManager::LoadFile(const char *fileName)
{
	if (!m_oTable.LoadFile(fileName))
	{
		LogWarn("Load file %s failed!", fileName);
		return false;
	}

	PlayerLevelTable::RowData *row = NULL;
	for (UINT32 i = 0; i < m_oTable.Table.size(); ++i)
	{
		row = m_oTable.Table[i];
		if (row->Level > MAX_LEVEL_COUNT)
		{
			LogError("Load %s error, Level %d is big than MAX_ROLE_LEVEL[%d]",
				fileName, row->Level, MAX_LEVEL_COUNT);
			return false;
		}

		m_LevelIndex[row->Level] = row;
	}

	m_MaxLevel =  GetGlobalConfig().GetInt("MaxLevel", 60); //m_oTable.Table.back()->Level;

	return true;
}

UINT64 PlayerLevelManager::GetLevelupExp(int lvl)
{
	auto row = PlayerLevelManager::Instance()->GetLevelInfo(lvl + 1);
	if (row != NULL)
	{
		return row->Exp;
	}
	else
	{
		return m_oTable.Table.back()->Exp;
	}
}

double PlayerLevelManager::GetExpAddition(int level)
{
	auto row = PlayerLevelManager::Instance()->GetLevelInfo(level);
	return NULL == row ? 1.0 : row->ExpAddition;
}

UINT32 PlayerLevelManager::GetCatchUpRate(const UINT32 days,const UINT32 level)
{
	if ( level < GetGlobalConfig().StageCatchUpExpMinLevel)
	{
		return 0;
	}
	auto cfg = catchup_exp_.GetByServerDays(days);
	if (NULL == cfg)
	{
		SSWarn<<"no cfg ,the ServerDasys is :" << days <<END;
		return 0;
	}
	if (cfg->level01 > level)
	{
		return cfg->ExpRate01;
	}
	if (cfg->level02 > level)
	{
		return cfg->ExpRate02;
	}
	if (cfg->level03 > level)
	{
		return cfg->ExpRate03;
	}
	if (cfg->level04 > level)
	{
		return cfg->ExpRate04;
	}

	return 0;
}

UINT32 PlayerLevelManager::GetEnhanceMaxLvl(int level)
{
	auto row = PlayerLevelManager::Instance()->GetLevelInfo(level);
	return row == NULL ? 0 : row->MaxEnhanceLevel;
}

int PlayerLevelManager::GetSkillPoint(int level, bool init)
{
	int point = 0;
	for (int i = 2; i <= level; ++i)
	{
		PlayerLevelTable::RowData* data = GetLevelInfo(i);
		if (data)
		{
			point += data->AddSkillPoint;
		}
	}
	///> 角色初始学习的技能占5个技能点
	return point + (init ? 0 : 5);
}
