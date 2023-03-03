#ifndef __PTCG2D_REMOVEREWARD_H__
#define __PTCG2D_REMOVEREWARD_H__

// generate by ProtoGen at date: 2015/8/12 11:12:37

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2D_REMOVEREWARD_TYPE 22212

class PtcG2D_RemoveReward : public CProtocol
{
public:
    explicit PtcG2D_RemoveReward() : CProtocol(PTCG2D_REMOVEREWARD_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2D_RemoveReward()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RemoveReward m_Data;
};

#endif