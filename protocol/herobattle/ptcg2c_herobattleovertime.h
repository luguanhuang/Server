#ifndef __PTCG2C_HEROBATTLEOVERTIME_H__
#define __PTCG2C_HEROBATTLEOVERTIME_H__

// generate by ProtoGen at date: 2016/12/19 17:28:19

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_HEROBATTLEOVERTIME_TYPE 2950

class PtcG2C_HeroBattleOverTime : public CProtocol
{
public:
    explicit PtcG2C_HeroBattleOverTime() : CProtocol(PTCG2C_HEROBATTLEOVERTIME_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_HeroBattleOverTime()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HeroBattleOverTimeData m_Data;
};

#endif