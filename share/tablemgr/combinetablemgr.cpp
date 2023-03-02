#include "pch.h"
#include "combinetablemgr.h"
#include "table/globalconfig.h"

bool CombineTableMgr::Init()
{
	return LoadFile();
}

void CombineTableMgr::Uninit()
{
	ClearFile();
}

bool CombineTableMgr::CheckFile()
{
	MailContent tempMailContent;
	if (!tempMailContent.LoadFile("table/MailContent.txt"))
	{
		SSWarn<<"load file table/MailContent.txt failed"<<END;
		return false;
	}
	PlayerLevelTable tmpLevel;
	if (!tmpLevel.LoadFile("table/PlayerLevel.txt"))
	{
		LogWarn("Load file table/PlayerLevel.txt failed!");
		return false;
	}
	return true;
}

bool CombineTableMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}
	ClearFile();

	if (!m_table.LoadFile("table/MailContent.txt"))
	{
		return false;
	}
	for (auto i = m_table.Table.begin(); i != m_table.Table.end(); ++i)
	{
		MailConf conf;
		conf.m_title = (*i)->MailTitle;
		conf.m_content = (*i)->MailContent;
		conf.m_rewards = (*i)->MailReward;
		m_id2conf[(*i)->MailId] = conf;
	}

	if (!m_LevelTable.LoadFile("table/PlayerLevel.txt"))
	{
		LogWarn("Load file table/PlayerLevel.txt failed!");
		return false;
	}

	PlayerLevelTable::RowData *row = NULL;
	for (UINT32 i = 0; i < m_LevelTable.Table.size(); ++i)
	{
		row = m_LevelTable.Table[i];
		if (row->Level > MAX_LEVEL)
		{
			return false;
		}
		m_LevelIndex[row->Level] = row;
	}

	return true;
}

void CombineTableMgr::ClearFile()
{
	m_table.Clear();
	m_id2conf.clear();
	m_LevelTable.Clear();
	memset(m_LevelIndex, 0, sizeof(m_LevelIndex));
}

const MailConf* CombineTableMgr::GetMailConf(UINT32 id)
{
	auto i = m_id2conf.find(id);
	if (i == m_id2conf.end())
	{
		return NULL;
	}
	return &i->second;
}

const std::string CombineTableMgr::ReplaceString(const std::string& src, const std::string& dest, const std::string& content)
{
	int start = content.find(src);
	if(std::string::npos == (size_t)start)
	{
		return "";
	}
	std::string temp = content;
	int end = src.size();
	temp.replace(start, end, dest);
	return temp;
}

PlayerLevelTable::RowData * CombineTableMgr::GetLevelInfo(int level)
{
	if (level < 0 || level > MAX_LEVEL)
		return NULL;

	return m_LevelIndex[level];
}

UINT32 CombineTableMgr::GetNeedExp(UINT32 startLevel, UINT32 endLevel)
{
	if (startLevel >= endLevel)
	{
		return 0;
	}

	UINT32 sum = 0;
	for (int i = startLevel + 1; i <= endLevel; ++i)
	{
		PlayerLevelTable::RowData* pData = GetLevelInfo(i);
		UINT32 exp = (pData == NULL) ? m_LevelTable.Table.back()->Exp : pData->Exp;
		sum += exp;
	}
	sum = sum * GetGlobalConfig().CombineServerExpRate;
	return sum;
}