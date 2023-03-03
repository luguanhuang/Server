#ifndef __PTCM2C_GUILDSETINGNOTIFY_H__
#define __PTCM2C_GUILDSETINGNOTIFY_H__

// generate by ProtoGen at date: 2016/9/15 22:39:17

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_GUILDSETINGNOTIFY_TYPE 21944

class PtcM2C_GuildSetingNotify : public CProtocol
{
public:
    explicit PtcM2C_GuildSetingNotify() : CProtocol(PTCM2C_GUILDSETINGNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_GuildSetingNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildSettingChanged m_Data;
};

#endif