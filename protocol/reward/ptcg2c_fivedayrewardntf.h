#ifndef __PTCG2C_FIVEDAYREWARDNTF_H__
#define __PTCG2C_FIVEDAYREWARDNTF_H__

// generate by ProtoGen at date: 2016/6/16 14:25:15

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_FIVEDAYREWARDNTF_TYPE 37452

class PtcG2C_FiveDayRewardNTF : public CProtocol
{
public:
    explicit PtcG2C_FiveDayRewardNTF() : CProtocol(PTCG2C_FIVEDAYREWARDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_FiveDayRewardNTF()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FiveRewardState m_Data;
};

#endif