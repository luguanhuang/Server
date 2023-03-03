#ifndef __PTCG2C_HEROBATTLEDATANTF_H__
#define __PTCG2C_HEROBATTLEDATANTF_H__

// generate by ProtoGen at date: 2016/12/25 18:48:34

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_HEROBATTLEDATANTF_TYPE 60769

class PtcG2C_HeroBattleDataNtf : public CProtocol
{
public:
    explicit PtcG2C_HeroBattleDataNtf() : CProtocol(PTCG2C_HEROBATTLEDATANTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_HeroBattleDataNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HeroBattleData m_Data;
};

#endif