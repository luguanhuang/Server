#ifndef __PTCG2C_HEROBATTLEANCIENTPOWER_H__
#define __PTCG2C_HEROBATTLEANCIENTPOWER_H__

// generate by ProtoGen at date: 2017/7/15 15:15:19

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_HEROBATTLEANCIENTPOWER_TYPE 37102

class PtcG2C_HeroBattleAncientPower : public CProtocol
{
public:
    explicit PtcG2C_HeroBattleAncientPower() : CProtocol(PTCG2C_HEROBATTLEANCIENTPOWER_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_HeroBattleAncientPower()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HeroBattleAncientPowerData m_Data;
};

#endif