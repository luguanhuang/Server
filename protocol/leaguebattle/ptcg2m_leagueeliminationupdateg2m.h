#ifndef __PTCG2M_LEAGUEELIMINATIONUPDATEG2M_H__
#define __PTCG2M_LEAGUEELIMINATIONUPDATEG2M_H__

// generate by ProtoGen at date: 2017/1/14 14:05:14

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_LEAGUEELIMINATIONUPDATEG2M_TYPE 42184

class PtcG2M_LeagueEliminationUpdateG2M : public CProtocol
{
public:
    explicit PtcG2M_LeagueEliminationUpdateG2M() : CProtocol(PTCG2M_LEAGUEELIMINATIONUPDATEG2M_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_LeagueEliminationUpdateG2M()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LeagueEliminationUpdateData m_Data;
};

#endif