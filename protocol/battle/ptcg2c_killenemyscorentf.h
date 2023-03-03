#ifndef __PTCG2C_KILLENEMYSCORENTF_H__
#define __PTCG2C_KILLENEMYSCORENTF_H__

// generate by ProtoGen at date: 2016/11/9 12:17:05

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_KILLENEMYSCORENTF_TYPE 50119

class PtcG2C_KillEnemyScoreNtf : public CProtocol
{
public:
    explicit PtcG2C_KillEnemyScoreNtf() : CProtocol(PTCG2C_KILLENEMYSCORENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_KillEnemyScoreNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::KillEnemyScoreData m_Data;
};

#endif