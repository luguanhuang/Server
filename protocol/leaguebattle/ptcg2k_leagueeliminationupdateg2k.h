#ifndef __PTCG2K_LEAGUEELIMINATIONUPDATEG2K_H__
#define __PTCG2K_LEAGUEELIMINATIONUPDATEG2K_H__

// generate by ProtoGen at date: 2017/2/6 15:13:32

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2K_LEAGUEELIMINATIONUPDATEG2K_TYPE 34910

class PtcG2K_LeagueEliminationUpdateG2K : public CProtocol
{
public:
    explicit PtcG2K_LeagueEliminationUpdateG2K() : CProtocol(PTCG2K_LEAGUEELIMINATIONUPDATEG2K_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2K_LeagueEliminationUpdateG2K()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LeagueEliminationUpdateData m_Data;
};

#endif