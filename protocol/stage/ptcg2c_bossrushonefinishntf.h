#ifndef __PTCG2C_BOSSRUSHONEFINISHNTF_H__
#define __PTCG2C_BOSSRUSHONEFINISHNTF_H__

// generate by ProtoGen at date: 2016/9/2 22:07:34

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_BOSSRUSHONEFINISHNTF_TYPE 21034

class PtcG2C_BossRushOneFinishNtf : public CProtocol
{
public:
    explicit PtcG2C_BossRushOneFinishNtf() : CProtocol(PTCG2C_BOSSRUSHONEFINISHNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_BossRushOneFinishNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BossRushPara m_Data;
};

#endif