#ifndef __PTCG2C_DEATHNOTIFY_H__
#define __PTCG2C_DEATHNOTIFY_H__

// generate by ProtoGen at date: 2014/12/25 11:55:39

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_DEATHNOTIFY_TYPE 2319

class PtcG2C_DeathNotify : public CProtocol
{
public:
    explicit PtcG2C_DeathNotify() : CProtocol(PTCG2C_DEATHNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_DeathNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DeathInfo m_Data;
};

#endif