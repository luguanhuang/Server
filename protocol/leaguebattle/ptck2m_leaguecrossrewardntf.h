#ifndef __PTCK2M_LEAGUECROSSREWARDNTF_H__
#define __PTCK2M_LEAGUECROSSREWARDNTF_H__

// generate by ProtoGen at date: 2017/2/6 16:33:13

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2M_LEAGUECROSSREWARDNTF_TYPE 41543

class PtcK2M_LeagueCrossRewardNtf : public CProtocol
{
public:
    explicit PtcK2M_LeagueCrossRewardNtf() : CProtocol(PTCK2M_LEAGUECROSSREWARDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2M_LeagueCrossRewardNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LeagueCrossReward m_Data;
};

#endif