#ifndef __PTCG2D_ADDREWARD_H__
#define __PTCG2D_ADDREWARD_H__

// generate by ProtoGen at date: 2015/8/11 16:21:31

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2D_ADDREWARD_TYPE 28040

class PtcG2D_AddReward : public CProtocol
{
public:
    explicit PtcG2D_AddReward() : CProtocol(PTCG2D_ADDREWARD_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2D_AddReward()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AddReward m_Data;
};

#endif