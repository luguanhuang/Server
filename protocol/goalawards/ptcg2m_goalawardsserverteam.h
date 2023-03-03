#ifndef __PTCG2M_GOALAWARDSSERVERTEAM_H__
#define __PTCG2M_GOALAWARDSSERVERTEAM_H__

// generate by ProtoGen at date: 2017/9/13 10:43:48

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_GOALAWARDSSERVERTEAM_TYPE 25101

class PtcG2M_GoalAwardsServerTeam : public CProtocol
{
public:
    explicit PtcG2M_GoalAwardsServerTeam() : CProtocol(PTCG2M_GOALAWARDSSERVERTEAM_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_GoalAwardsServerTeam()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GoalAwardsServerTeam_G2M m_Data;
};

#endif