#ifndef __PTCG2C_WORLDBOSSSTATENTF_H__
#define __PTCG2C_WORLDBOSSSTATENTF_H__

// generate by ProtoGen at date: 2016/7/21 0:12:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_WORLDBOSSSTATENTF_TYPE 5473

class PtcG2C_WorldBossStateNtf : public CProtocol
{
public:
    explicit PtcG2C_WorldBossStateNtf() : CProtocol(PTCG2C_WORLDBOSSSTATENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_WorldBossStateNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::WorldBossStateNtf m_Data;
};

#endif