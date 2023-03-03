#ifndef __PTCG2M_SYNGUILDBOSSHURT_H__
#define __PTCG2M_SYNGUILDBOSSHURT_H__

// generate by ProtoGen at date: 2016/9/16 12:07:20

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_SYNGUILDBOSSHURT_TYPE 20810

class PtcG2M_SynGuildBossHurt : public CProtocol
{
public:
    explicit PtcG2M_SynGuildBossHurt() : CProtocol(PTCG2M_SYNGUILDBOSSHURT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_SynGuildBossHurt()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynGuildBossHurt m_Data;
};

#endif