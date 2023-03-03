#ifndef __PTCM2C_GUILDBESTCARDSNTFMS_H__
#define __PTCM2C_GUILDBESTCARDSNTFMS_H__

// generate by ProtoGen at date: 2016/9/18 20:39:06

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_GUILDBESTCARDSNTFMS_TYPE 31828

class PtcM2C_GuildBestCardsNtfMs : public CProtocol
{
public:
    explicit PtcM2C_GuildBestCardsNtfMs() : CProtocol(PTCM2C_GUILDBESTCARDSNTFMS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_GuildBestCardsNtfMs()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildBestCardsNtf m_Data;
};

#endif