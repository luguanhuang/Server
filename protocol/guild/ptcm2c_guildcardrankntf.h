#ifndef __PTCM2C_GUILDCARDRANKNTF_H__
#define __PTCM2C_GUILDCARDRANKNTF_H__

// generate by ProtoGen at date: 2016/10/1 20:10:40

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_GUILDCARDRANKNTF_TYPE 63693

class PtcM2C_GuildCardRankNtf : public CProtocol
{
public:
    explicit PtcM2C_GuildCardRankNtf() : CProtocol(PTCM2C_GUILDCARDRANKNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_GuildCardRankNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildCardRankNtf m_Data;
};

#endif