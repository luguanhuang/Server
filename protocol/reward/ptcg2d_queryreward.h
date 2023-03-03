#ifndef __PTCG2D_QUERYREWARD_H__
#define __PTCG2D_QUERYREWARD_H__

// generate by ProtoGen at date: 2017/3/15 10:55:54

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2D_QUERYREWARD_TYPE 41805

class PtcG2D_QueryReward : public CProtocol
{
public:
    explicit PtcG2D_QueryReward() : CProtocol(PTCG2D_QUERYREWARD_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2D_QueryReward()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::QueryReward m_Data;
};

#endif