#ifndef __PTCG2T_BROADCASTG2T_H__
#define __PTCG2T_BROADCASTG2T_H__

// generate by ProtoGen at date: 2017/2/28 15:32:00

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2T_BROADCASTG2T_TYPE 57877

class PtcG2T_BroadCastG2T : public CProtocol
{
public:
    explicit PtcG2T_BroadCastG2T() : CProtocol(PTCG2T_BROADCASTG2T_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2T_BroadCastG2T()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BroadCastG2TData m_Data;
};

#endif