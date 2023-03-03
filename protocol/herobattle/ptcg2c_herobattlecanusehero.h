#ifndef __PTCG2C_HEROBATTLECANUSEHERO_H__
#define __PTCG2C_HEROBATTLECANUSEHERO_H__

// generate by ProtoGen at date: 2017/1/11 12:08:00

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_HEROBATTLECANUSEHERO_TYPE 20354

class PtcG2C_HeroBattleCanUseHero : public CProtocol
{
public:
    explicit PtcG2C_HeroBattleCanUseHero() : CProtocol(PTCG2C_HEROBATTLECANUSEHERO_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_HeroBattleCanUseHero()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HeroBattleCanUseHeroData m_Data;
};

#endif