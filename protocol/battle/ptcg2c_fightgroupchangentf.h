#ifndef __PTCG2C_FIGHTGROUPCHANGENTF_H__
#define __PTCG2C_FIGHTGROUPCHANGENTF_H__

// generate by ProtoGen at date: 2016/9/2 19:55:54

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_FIGHTGROUPCHANGENTF_TYPE 2142

class PtcG2C_FightGroupChangeNtf : public CProtocol
{
public:
    explicit PtcG2C_FightGroupChangeNtf() : CProtocol(PTCG2C_FIGHTGROUPCHANGENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_FightGroupChangeNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FightGroupChangeNtf m_Data;
};

#endif