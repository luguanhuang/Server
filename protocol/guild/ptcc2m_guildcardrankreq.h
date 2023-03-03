#ifndef __PTCC2M_GUILDCARDRANKREQ_H__
#define __PTCC2M_GUILDCARDRANKREQ_H__

// generate by ProtoGen at date: 2016/10/1 20:09:28

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_GUILDCARDRANKREQ_TYPE 50768

class PtcC2M_GuildCardRankReq : public CProtocol
{
public:
    explicit PtcC2M_GuildCardRankReq() : CProtocol(PTCC2M_GUILDCARDRANKREQ_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_GuildCardRankReq()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildCardRankReq m_Data;
};

#endif