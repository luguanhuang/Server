#ifndef __PTCG2C_HEROBATTLEINCIRCLENTF_H__
#define __PTCG2C_HEROBATTLEINCIRCLENTF_H__

// generate by ProtoGen at date: 2016/12/17 13:15:14

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_HEROBATTLEINCIRCLENTF_TYPE 40409

class PtcG2C_HeroBattleInCircleNtf : public CProtocol
{
public:
    explicit PtcG2C_HeroBattleInCircleNtf() : CProtocol(PTCG2C_HEROBATTLEINCIRCLENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_HeroBattleInCircleNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HeroBattleInCircle m_Data;
};

#endif