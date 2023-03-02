#include "pch.h"
#include "PlayerLevelManager.h"


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
		if (m_MaxLevel < row->Level)
		{
			m_MaxLevel = row->Level;
		}
		m_LevelIndex[row->Level] = row;
	}
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
	return NULL == row ? 1.0f : row->ExpAddition;
}
