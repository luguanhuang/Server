#ifndef __PTCG2C_GUILDSETTINGCHANGEDNOTIFY_H__
#define __PTCG2C_GUILDSETTINGCHANGEDNOTIFY_H__

// generate by ProtoGen at date: 2015/10/1 11:52:58

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GUILDSETTINGCHANGEDNOTIFY_TYPE 63721

class PtcG2C_GuildSettingChangedNotify : public CProtocol
{
public:
    explicit PtcG2C_GuildSettingChangedNotify() : CProtocol(PTCG2C_GUILDSETTINGCHANGEDNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GuildSettingChangedNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildSettingChanged m_Data;
};

#endif