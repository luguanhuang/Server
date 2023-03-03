#ifndef __PTCG2C_NEXTDAYREWARDNTF_H__
#define __PTCG2C_NEXTDAYREWARDNTF_H__

// generate by ProtoGen at date: 2015/10/20 18:37:39

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_NEXTDAYREWARDNTF_TYPE 50036

class PtcG2C_NextDayRewardNtf : public CProtocol
{
public:
    explicit PtcG2C_NextDayRewardNtf() : CProtocol(PTCG2C_NEXTDAYREWARDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_NextDayRewardNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NextDayRewardNtf m_Data;
};

#endif