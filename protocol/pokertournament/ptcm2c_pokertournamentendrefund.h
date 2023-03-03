#ifndef __PTCM2C_POKERTOURNAMENTENDREFUND_H__
#define __PTCM2C_POKERTOURNAMENTENDREFUND_H__

// generate by ProtoGen at date: 2017/2/4 9:45:16

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_POKERTOURNAMENTENDREFUND_TYPE 50590

class PtcM2C_PokerTournamentEndReFund : public CProtocol
{
public:
    explicit PtcM2C_PokerTournamentEndReFund() : CProtocol(PTCM2C_POKERTOURNAMENTENDREFUND_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_PokerTournamentEndReFund()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PokerTournamentEndReFundMsg m_Data;
};

#endif