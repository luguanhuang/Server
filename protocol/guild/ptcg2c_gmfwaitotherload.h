#ifndef __PTCG2C_GMFWAITOTHERLOAD_H__
#define __PTCG2C_GMFWAITOTHERLOAD_H__

// generate by ProtoGen at date: 2016/8/2 0:33:11

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GMFWAITOTHERLOAD_TYPE 1133

class PtcG2C_GmfWaitOtherLoad : public CProtocol
{
public:
    explicit PtcG2C_GmfWaitOtherLoad() : CProtocol(PTCG2C_GMFWAITOTHERLOAD_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GmfWaitOtherLoad()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GmfWaitOtherArg m_Data;
};

#endif