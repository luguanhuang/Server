#ifndef __PTCG2C_REWARDCHANGEDNTF_H__
#define __PTCG2C_REWARDCHANGEDNTF_H__

// generate by ProtoGen at date: 2015/7/9 20:54:38

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_REWARDCHANGEDNTF_TYPE 57873

class PtcG2C_RewardChangedNtf : public CProtocol
{
public:
    explicit PtcG2C_RewardChangedNtf() : CProtocol(PTCG2C_REWARDCHANGEDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_RewardChangedNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RewardChanged m_Data;
};

#endif