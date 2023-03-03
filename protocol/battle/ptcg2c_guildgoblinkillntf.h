#ifndef __PTCG2C_GUILDGOBLINKILLNTF_H__
#define __PTCG2C_GUILDGOBLINKILLNTF_H__

// generate by ProtoGen at date: 2015/12/16 16:38:20

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GUILDGOBLINKILLNTF_TYPE 9436

class PtcG2C_GuildGoblinKillNtf : public CProtocol
{
public:
    explicit PtcG2C_GuildGoblinKillNtf() : CProtocol(PTCG2C_GUILDGOBLINKILLNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GuildGoblinKillNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildGoblinSceneInfo m_Data;
};

#endif