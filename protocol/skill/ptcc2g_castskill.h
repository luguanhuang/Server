#ifndef __PTCC2G_CASTSKILL_H__
#define __PTCC2G_CASTSKILL_H__

// generate by ProtoGen at date: 2014/12/3 19:53:01

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_CASTSKILL_TYPE 49584

class PtcC2G_CastSkill : public CProtocol
{
public:
    explicit PtcC2G_CastSkill() : CProtocol(PTCC2G_CASTSKILL_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_CastSkill()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SkillDataUnit m_Data;
};

#endif