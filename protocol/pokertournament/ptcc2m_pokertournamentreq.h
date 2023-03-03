#ifndef __PTCC2M_POKERTOURNAMENTREQ_H__
#define __PTCC2M_POKERTOURNAMENTREQ_H__

// generate by ProtoGen at date: 2016/12/10 21:11:26

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_POKERTOURNAMENTREQ_TYPE 3685

class PtcC2M_PokerTournamentReq : public CProtocol
{
public:
    explicit PtcC2M_PokerTournamentReq() : CProtocol(PTCC2M_POKERTOURNAMENTREQ_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_PokerTournamentReq()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildCardMatchReq m_Data;
};

#endif