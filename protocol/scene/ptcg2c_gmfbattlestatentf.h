#ifndef __PTCG2C_GMFBATTLESTATENTF_H__
#define __PTCG2C_GMFBATTLESTATENTF_H__

// generate by ProtoGen at date: 2016/10/10 23:24:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GMFBATTLESTATENTF_TYPE 21747

class PtcG2C_GmfBattleStateNtf : public CProtocol
{
public:
    explicit PtcG2C_GmfBattleStateNtf() : CProtocol(PTCG2C_GMFBATTLESTATENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GmfBattleStateNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GmfBatlleStatePara m_Data;
};

#endif