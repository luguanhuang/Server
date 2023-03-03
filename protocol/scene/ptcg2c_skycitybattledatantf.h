#ifndef __PTCG2C_SKYCITYBATTLEDATANTF_H__
#define __PTCG2C_SKYCITYBATTLEDATANTF_H__

// generate by ProtoGen at date: 2016/10/9 18:08:06

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SKYCITYBATTLEDATANTF_TYPE 51753

class PtcG2C_SkyCityBattleDataNtf : public CProtocol
{
public:
    explicit PtcG2C_SkyCityBattleDataNtf() : CProtocol(PTCG2C_SKYCITYBATTLEDATANTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SkyCityBattleDataNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SkyCityAllInfo m_Data;
};

#endif