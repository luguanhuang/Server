#ifndef __PTCG2C_MOBASIGNALBROADCAST_H__
#define __PTCG2C_MOBASIGNALBROADCAST_H__

// generate by ProtoGen at date: 2017/6/13 17:07:59

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_MOBASIGNALBROADCAST_TYPE 6250

class PtcG2C_MobaSignalBroadcast : public CProtocol
{
public:
    explicit PtcG2C_MobaSignalBroadcast() : CProtocol(PTCG2C_MOBASIGNALBROADCAST_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_MobaSignalBroadcast()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MobaSignalBroadcastData m_Data;
};

#endif