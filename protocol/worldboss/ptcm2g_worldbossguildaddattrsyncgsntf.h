#ifndef __PTCM2G_WORLDBOSSGUILDADDATTRSYNCGSNTF_H__
#define __PTCM2G_WORLDBOSSGUILDADDATTRSYNCGSNTF_H__

// generate by ProtoGen at date: 2017/4/10 14:54:22

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_WORLDBOSSGUILDADDATTRSYNCGSNTF_TYPE 23367

class PtcM2G_WorldBossGuildAddAttrSyncGsNtf : public CProtocol
{
public:
    explicit PtcM2G_WorldBossGuildAddAttrSyncGsNtf() : CProtocol(PTCM2G_WORLDBOSSGUILDADDATTRSYNCGSNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_WorldBossGuildAddAttrSyncGsNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::WorldBossGuildAddAttrSyncGs m_Data;
};

#endif