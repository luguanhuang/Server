#ifndef __PTCG2C_NOTIFYGUILDSKILLALLDATA_H__
#define __PTCG2C_NOTIFYGUILDSKILLALLDATA_H__

// generate by ProtoGen at date: 2016/7/23 17:31:30

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_NOTIFYGUILDSKILLALLDATA_TYPE 9911

class PtcG2C_NotifyGuildSkillAllData : public CProtocol
{
public:
    explicit PtcG2C_NotifyGuildSkillAllData() : CProtocol(PTCG2C_NOTIFYGUILDSKILLALLDATA_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_NotifyGuildSkillAllData()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildSkillAllData m_Data;
};

#endif