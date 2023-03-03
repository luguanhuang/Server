#ifndef __PTCG2C_TOWERFIRSTPASSREWARDNTF_H__
#define __PTCG2C_TOWERFIRSTPASSREWARDNTF_H__

// generate by ProtoGen at date: 2016/9/19 12:33:14

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_TOWERFIRSTPASSREWARDNTF_TYPE 1039

class PtcG2C_TowerFirstPassRewardNtf : public CProtocol
{
public:
    explicit PtcG2C_TowerFirstPassRewardNtf() : CProtocol(PTCG2C_TOWERFIRSTPASSREWARDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_TowerFirstPassRewardNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TowerFirstPassRewardData m_Data;
};

#endif