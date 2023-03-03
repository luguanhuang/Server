#ifndef __PTCM2C_NOTIFYGUILDSKILLDATA_H__
#define __PTCM2C_NOTIFYGUILDSKILLDATA_H__

// generate by ProtoGen at date: 2016/9/9 22:44:14

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_NOTIFYGUILDSKILLDATA_TYPE 2458

class PtcM2C_NotifyGuildSkillData : public CProtocol
{
public:
    explicit PtcM2C_NotifyGuildSkillData() : CProtocol(PTCM2C_NOTIFYGUILDSKILLDATA_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_NotifyGuildSkillData()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildSkillAllData m_Data;
};

#endif