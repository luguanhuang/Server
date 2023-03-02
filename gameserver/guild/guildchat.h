#ifndef __GUILDCHAT_H__
#define __GUILDCHAT_H__

#include "guild/guilddef.h"
#include "unit/roleserilizeinterface.h"
#include "pb/project.pb.h"

class CGuildChat : public IGuildSerializerInterface
{
public:
	CGuildChat();
	virtual ~CGuildChat();

	virtual bool IsChanged();
	virtual bool Load(const KKSG::GuildAllDBData & guilddata);
	virtual void Save(KKSG::GuildAllDBData & guilddata);
	virtual const std::string ToBinary();

	void Push(const KKSG::ChatInfo& chatinfo);
	
	const std::deque<KKSG::ChatInfo>& GetChatInfos();

private:
	std::deque<KKSG::ChatInfo> m_chats;	
};

#endif