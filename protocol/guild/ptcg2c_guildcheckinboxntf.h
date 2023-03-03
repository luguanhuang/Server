#ifndef __PTCG2C_GUILDCHECKINBOXNTF_H__
#define __PTCG2C_GUILDCHECKINBOXNTF_H__

// generate by ProtoGen at date: 2015/10/8 14:13:16

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GUILDCHECKINBOXNTF_TYPE 5114

class PtcG2C_GuildCheckinBoxNtf : public CProtocol
{
public:
    explicit PtcG2C_GuildCheckinBoxNtf() : CProtocol(PTCG2C_GUILDCHECKINBOXNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GuildCheckinBoxNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildCheckinBoxNtf m_Data;
};

#endif