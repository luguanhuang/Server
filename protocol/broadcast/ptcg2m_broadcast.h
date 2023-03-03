#ifndef __PTCG2M_BROADCAST_H__
#define __PTCG2M_BROADCAST_H__

// generate by ProtoGen at date: 2016/10/11 14:12:22

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_BROADCAST_TYPE 7356

class PtcG2M_BroadCast : public CProtocol
{
public:
    explicit PtcG2M_BroadCast() : CProtocol(PTCG2M_BROADCAST_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_BroadCast()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BroadCastData m_Data;
};

#endif