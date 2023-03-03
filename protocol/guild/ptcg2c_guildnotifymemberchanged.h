#ifndef __PTCG2C_GUILDNOTIFYMEMBERCHANGED_H__
#define __PTCG2C_GUILDNOTIFYMEMBERCHANGED_H__

// generate by ProtoGen at date: 2015/10/1 11:49:30

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GUILDNOTIFYMEMBERCHANGED_TYPE 5957

class PtcG2C_GuildNotifyMemberChanged : public CProtocol
{
public:
    explicit PtcG2C_GuildNotifyMemberChanged() : CProtocol(PTCG2C_GUILDNOTIFYMEMBERCHANGED_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GuildNotifyMemberChanged()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildMemberInfo m_Data;
};

#endif