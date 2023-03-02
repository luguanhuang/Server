#include "pch.h"
#include "guildchat.h"

#define MAXCHAT 10

CGuildChat::CGuildChat()
{

}

CGuildChat::~CGuildChat()
{

}

bool CGuildChat::IsChanged()
{
	return false;
}

bool CGuildChat::Load(const KKSG::GuildAllDBData & guilddata)
{
	return true;
}

void CGuildChat::Save(KKSG::GuildAllDBData & guilddata)
{
	
}

const std::string CGuildChat::ToBinary()
{
	return "";
}

void CGuildChat::Push(const KKSG::ChatInfo& chatinfo)
{
	m_chats.push_back(chatinfo);
	if (m_chats.size() > MAXCHAT)
	{
		m_chats.pop_front();
	}
}

const std::deque<KKSG::ChatInfo>& CGuildChat::GetChatInfos()
{
	return m_chats;
}

