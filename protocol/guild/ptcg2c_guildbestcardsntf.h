#ifndef __PTCG2C_GUILDBESTCARDSNTF_H__
#define __PTCG2C_GUILDBESTCARDSNTF_H__

// generate by ProtoGen at date: 2015/10/11 17:33:06

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GUILDBESTCARDSNTF_TYPE 44473

class PtcG2C_GuildBestCardsNtf : public CProtocol
{
public:
    explicit PtcG2C_GuildBestCardsNtf() : CProtocol(PTCG2C_GUILDBESTCARDSNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GuildBestCardsNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildBestCardsNtf m_Data;
};

#endif