#ifndef __PTCM2G_SYNGUILDBOSSHP_H__
#define __PTCM2G_SYNGUILDBOSSHP_H__

// generate by ProtoGen at date: 2017/1/13 20:19:45

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_SYNGUILDBOSSHP_TYPE 54597

class PtcM2G_SynGuildBossHp : public CProtocol
{
public:
    explicit PtcM2G_SynGuildBossHp() : CProtocol(PTCM2G_SYNGUILDBOSSHP_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_SynGuildBossHp()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynGuildBossHp m_Data;
};

#endif