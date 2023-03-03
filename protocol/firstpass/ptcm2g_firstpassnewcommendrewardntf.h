#ifndef __PTCM2G_FIRSTPASSNEWCOMMENDREWARDNTF_H__
#define __PTCM2G_FIRSTPASSNEWCOMMENDREWARDNTF_H__

// generate by ProtoGen at date: 2016/8/26 19:37:21

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_FIRSTPASSNEWCOMMENDREWARDNTF_TYPE 40039

class PtcM2G_FirstPassNewCommendRewardNtf : public CProtocol
{
public:
    explicit PtcM2G_FirstPassNewCommendRewardNtf() : CProtocol(PTCM2G_FIRSTPASSNEWCOMMENDREWARDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_FirstPassNewCommendRewardNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FirstPassNewCommendRewardData m_Data;
};

#endif