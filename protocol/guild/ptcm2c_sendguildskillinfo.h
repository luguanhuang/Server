#ifndef __PTCM2C_SENDGUILDSKILLINFO_H__
#define __PTCM2C_SENDGUILDSKILLINFO_H__

// generate by ProtoGen at date: 2016/8/26 15:01:41

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_SENDGUILDSKILLINFO_TYPE 55907

class PtcM2C_SendGuildSkillInfo : public CProtocol
{
public:
    explicit PtcM2C_SendGuildSkillInfo() : CProtocol(PTCM2C_SENDGUILDSKILLINFO_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_SendGuildSkillInfo()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildSkillAllData m_Data;
};

#endif