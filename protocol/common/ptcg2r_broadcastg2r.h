#ifndef __PTCG2R_BROADCASTG2R_H__
#define __PTCG2R_BROADCASTG2R_H__

// generate by ProtoGen at date: 2016/12/27 21:12:21

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2R_BROADCASTG2R_TYPE 4891

class PtcG2R_BroadCastG2R : public CProtocol
{
public:
    explicit PtcG2R_BroadCastG2R() : CProtocol(PTCG2R_BROADCASTG2R_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2R_BroadCastG2R()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BroadCastG2R m_Data;
};

#endif