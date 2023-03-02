#include "pch.h"
#include "mailconfig.h"

INSTANCE_SINGLETON(MailConfig);

MailConfig::MailConfig()
{

}

MailConfig::~MailConfig()
{

}

bool MailConfig::Init()
{
	return LoadFile();
}

void MailConfig::Uninit()
{
	ClearFile();
}

bool MailConfig::CheckFile()
{
	MailContent tempMailContent;
	if (!tempMailContent.LoadFile("table/MailContent.txt"))
	{
		SSWarn<<"load file table/MailContent.txt failed"<<END;
		return false;
	}
	return true;
}

bool MailConfig::LoadFile()
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
	return true;
}

void MailConfig::ClearFile()
{
	m_table.Clear();
	m_id2conf.clear();
}

const MailConf* MailConfig::GetMailConf(UINT32 id)
{
	auto i = m_id2conf.find(id);
	if (i == m_id2conf.end())
	{
		return NULL;
	}
	return &i->second;
}

const std::string MailConfig::ReplaceString(const std::string& src, const std::string& dest, const std::string& content)
{
	int start = content.find(src);
	if(std::string::npos == (size_t)start)
	{
		return content;
	}
	std::string temp = content;
	int end = src.size();
	temp.replace(start, end, dest);
	return temp;
}
