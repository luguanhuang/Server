#ifndef __PTCM2C_GUILDAUCTITEMTIMEFRESH_H__
#define __PTCM2C_GUILDAUCTITEMTIMEFRESH_H__

// generate by ProtoGen at date: 2017/2/1 18:04:18

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_GUILDAUCTITEMTIMEFRESH_TYPE 49239

class PtcM2C_GuildAuctItemTimeFresh : public CProtocol
{
public:
    explicit PtcM2C_GuildAuctItemTimeFresh() : CProtocol(PTCM2C_GUILDAUCTITEMTIMEFRESH_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_GuildAuctItemTimeFresh()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildAuctItemTime m_Data;
};

#endif