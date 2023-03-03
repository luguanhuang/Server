#ifndef __PTCC2M_GUILDCARDMATCHREQ_H__
#define __PTCC2M_GUILDCARDMATCHREQ_H__

// generate by ProtoGen at date: 2016/10/2 17:38:51

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_GUILDCARDMATCHREQ_TYPE 21904

class PtcC2M_GuildCardMatchReq : public CProtocol
{
public:
    explicit PtcC2M_GuildCardMatchReq() : CProtocol(PTCC2M_GUILDCARDMATCHREQ_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_GuildCardMatchReq()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildCardMatchReq m_Data;
};

#endif