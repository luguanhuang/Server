#ifndef __PTCG2C_GUILDBOSSTIMEOUT_H__
#define __PTCG2C_GUILDBOSSTIMEOUT_H__

// generate by ProtoGen at date: 2016/7/18 21:02:01

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GUILDBOSSTIMEOUT_TYPE 56816

class PtcG2C_GuildBossTimeOut : public CProtocol
{
public:
    explicit PtcG2C_GuildBossTimeOut() : CProtocol(PTCG2C_GUILDBOSSTIMEOUT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GuildBossTimeOut()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildBossTimeOut m_Data;
};

#endif