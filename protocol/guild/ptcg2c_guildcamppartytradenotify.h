#ifndef __PTCG2C_GUILDCAMPPARTYTRADENOTIFY_H__
#define __PTCG2C_GUILDCAMPPARTYTRADENOTIFY_H__

// generate by ProtoGen at date: 2017/5/6 22:16:17

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GUILDCAMPPARTYTRADENOTIFY_TYPE 62988

class PtcG2C_GuildCampPartyTradeNotify : public CProtocol
{
public:
    explicit PtcG2C_GuildCampPartyTradeNotify() : CProtocol(PTCG2C_GUILDCAMPPARTYTRADENOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GuildCampPartyTradeNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildCampPartyTradeNotifyArg m_Data;
};

#endif