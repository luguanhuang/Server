#ifndef __PTCM2T_BROADCASTM2T_H__
#define __PTCM2T_BROADCASTM2T_H__

// generate by ProtoGen at date: 2016/10/11 15:38:14

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2T_BROADCASTM2T_TYPE 37061

class PtcM2T_BroadCastM2T : public CProtocol
{
public:
    explicit PtcM2T_BroadCastM2T() : CProtocol(PTCM2T_BROADCASTM2T_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2T_BroadCastM2T()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BroadCastDataM2T m_Data;
};

#endif