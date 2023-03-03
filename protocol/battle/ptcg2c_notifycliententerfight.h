#ifndef __PTCG2C_NOTIFYCLIENTENTERFIGHT_H__
#define __PTCG2C_NOTIFYCLIENTENTERFIGHT_H__

// generate by ProtoGen at date: 2016/7/25 16:29:24

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_NOTIFYCLIENTENTERFIGHT_TYPE 65191

class PtcG2C_NotifyClientEnterFight : public CProtocol
{
public:
    explicit PtcG2C_NotifyClientEnterFight() : CProtocol(PTCG2C_NOTIFYCLIENTENTERFIGHT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_NotifyClientEnterFight()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NotifyEnemyEnterFight m_Data;
};

#endif