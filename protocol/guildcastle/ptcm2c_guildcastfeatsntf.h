#ifndef __PTCM2C_GUILDCASTFEATSNTF_H__
#define __PTCM2C_GUILDCASTFEATSNTF_H__

// generate by ProtoGen at date: 2017/10/13 18:27:15

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_GUILDCASTFEATSNTF_TYPE 32885

class PtcM2C_GuildCastFeatsNtf : public CProtocol
{
public:
    explicit PtcM2C_GuildCastFeatsNtf() : CProtocol(PTCM2C_GUILDCASTFEATSNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_GuildCastFeatsNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildCastFeats m_Data;
};

#endif