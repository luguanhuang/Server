#include "pch.h"
#include "titleconfig.h"
#include "table/ProfessionMgr.h"

INSTANCE_SINGLETON(TitleConfig)

TitleConfig::TitleConfig()
{

}

TitleConfig::~TitleConfig()
{

}

bool TitleConfig::Init()
{
	if (!m_oTitleTable.LoadFile("table/TitleTable.txt"))
	{
		LogWarn("Load TitleTable.txt failed!");
		return false;
	}

	m_maxTitle = 0;
	for(auto i = m_oTitleTable.Table.begin(); i != m_oTitleTable.Table.end(); i++)
	{
		m_title[(*i)->RankID].push_back(*i);
		if(m_maxTitle < (*i)->RankID)
		{
			m_maxTitle = (*i)->RankID;
		}
	}

	return true;
}

void TitleConfig::Uninit()
{

}

const TitleConf* TitleConfig::GetTitleTableData(UINT32 titleID, UINT32 prof)
{
	prof = ProfessionMgr::GetBasicProfession(prof);
	if(m_title.find(titleID) == m_title.end())
	{
		return NULL;
	}
	for(UINT32 i = 0; i < m_title[titleID].size(); i++)
	{
		if(m_title[titleID][i]->BasicProfession == prof)
		{
			return m_title[titleID][i];
		}
	}
	return NULL;
}