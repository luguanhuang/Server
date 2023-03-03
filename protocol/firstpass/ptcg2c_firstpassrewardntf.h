#ifndef __PTCG2C_FIRSTPASSREWARDNTF_H__
#define __PTCG2C_FIRSTPASSREWARDNTF_H__

// generate by ProtoGen at date: 2016/8/24 16:28:22

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_FIRSTPASSREWARDNTF_TYPE 19007

class PtcG2C_FirstPassRewardNtf : public CProtocol
{
public:
    explicit PtcG2C_FirstPassRewardNtf() : CProtocol(PTCG2C_FIRSTPASSREWARDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_FirstPassRewardNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FirstPassRewardNtfData m_Data;
};

#endif