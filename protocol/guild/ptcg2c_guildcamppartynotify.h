#ifndef __PTCG2C_GUILDCAMPPARTYNOTIFY_H__
#define __PTCG2C_GUILDCAMPPARTYNOTIFY_H__

// generate by ProtoGen at date: 2017/5/5 16:57:06

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GUILDCAMPPARTYNOTIFY_TYPE 23338

class PtcG2C_guildcamppartyNotify : public CProtocol
{
public:
    explicit PtcG2C_guildcamppartyNotify() : CProtocol(PTCG2C_GUILDCAMPPARTYNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_guildcamppartyNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::guildcamppartyNotifyNtf m_Data;
};

#endif