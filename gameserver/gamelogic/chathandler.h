#ifndef __CHATHANDLER_H__
#define __CHATHANDLER_H__

#include "table/ChatTable.h"

class Role;
class ChatCondition;
struct ChatConditionParam;
class ChatHandler
{
public:
	ChatHandler(ChatTable::RowData* rowdata)
	{
		m_config = *rowdata;
	}
	virtual ~ChatHandler() {}

	virtual UINT32 CheckChatInfo(ChatConditionParam& param, const KKSG::ChatInfo& chatinfo);
	virtual UINT32 CheckChatInfo(ChatConditionParam& param);
	//virtual void PackChatInfo(Role* role, UINT32 channel, const KKSG::ChatInfo& chatinfo, const std::vector<UINT64>* destroleids = NULL);
	virtual void FowardChatInfo(Role* role, const std::vector<UINT64>* destroleids = NULL) = 0;

	void PushCondition(ChatCondition* condition);

protected:
	ChatTable::RowData m_config;
	std::vector<ChatCondition*> m_chatconditions;
};

/////////////////////////////////////////////////////////////////
class ChatWorldHandler : public ChatHandler
{
public:
	ChatWorldHandler(ChatTable::RowData* rowdata)
	:ChatHandler(rowdata)
	{

	}
	virtual ~ChatWorldHandler() {}

	virtual void FowardChatInfo(Role* role, const std::vector<UINT64>* destroleids = NULL);

};

/////////////////////////////////////////////////////////////////
class ChatGuildHandler : public ChatHandler
{
public:
	ChatGuildHandler(ChatTable::RowData* rowdata)
	:ChatHandler(rowdata)
	{

	}
	virtual ~ChatGuildHandler() {}

	virtual void FowardChatInfo(Role* role, const std::vector<UINT64>* destroleids = NULL);

};

/////////////////////////////////////////////////////////////////
class ChatPrivateHandler : public ChatHandler
{
public:
	ChatPrivateHandler(ChatTable::RowData* rowdata)
	:ChatHandler(rowdata)
	{

	}
	virtual ~ChatPrivateHandler() {}	

	virtual void FowardChatInfo(Role* role, const std::vector<UINT64>* destroleids = NULL);

};

/////////////////////////////////////////////////////////////////
class ChatTeamHandler : public ChatHandler
{
public:
	ChatTeamHandler(ChatTable::RowData* rowdata)
	:ChatHandler(rowdata)
	{

	}
	virtual ~ChatTeamHandler() {}	

	virtual void FowardChatInfo(Role* role, const std::vector<UINT64>* destroleids = NULL);

};

#endif