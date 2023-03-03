#ifndef __PTCM2C_GUILDCARDMATCHNTF_H__
#define __PTCM2C_GUILDCARDMATCHNTF_H__

// generate by ProtoGen at date: 2016/10/2 17:39:33

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_GUILDCARDMATCHNTF_TYPE 64513

class PtcM2C_GuildCardMatchNtf : public CProtocol
{
public:
    explicit PtcM2C_GuildCardMatchNtf() : CProtocol(PTCM2C_GUILDCARDMATCHNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_GuildCardMatchNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildCardMatchNtf m_Data;
};

#endif