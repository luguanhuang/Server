#ifndef __PTCG2C_WORLDBOSSGUILDADDATTRSYNCCLIENTNTF_H__
#define __PTCG2C_WORLDBOSSGUILDADDATTRSYNCCLIENTNTF_H__

// generate by ProtoGen at date: 2017/4/10 14:56:15

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_WORLDBOSSGUILDADDATTRSYNCCLIENTNTF_TYPE 65314

class PtcG2C_WorldBossGuildAddAttrSyncClientNtf : public CProtocol
{
public:
    explicit PtcG2C_WorldBossGuildAddAttrSyncClientNtf() : CProtocol(PTCG2C_WORLDBOSSGUILDADDATTRSYNCCLIENTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_WorldBossGuildAddAttrSyncClientNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::WorldBossGuildAddAttrSyncClient m_Data;
};

#endif