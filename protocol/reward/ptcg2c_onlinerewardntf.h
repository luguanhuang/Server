#ifndef __PTCG2C_ONLINEREWARDNTF_H__
#define __PTCG2C_ONLINEREWARDNTF_H__

// generate by ProtoGen at date: 2015/10/20 17:54:11

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_ONLINEREWARDNTF_TYPE 1895

class PtcG2C_OnlineRewardNtf : public CProtocol
{
public:
    explicit PtcG2C_OnlineRewardNtf() : CProtocol(PTCG2C_ONLINEREWARDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_OnlineRewardNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::OnlineRewardNtf m_Data;
};

#endif