#ifndef __PTCG2C_TELEPORTNOTICE_H__
#define __PTCG2C_TELEPORTNOTICE_H__

// generate by ProtoGen at date: 2016/3/12 17:47:16

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_TELEPORTNOTICE_TYPE 27305

class PtcG2C_TeleportNotice : public CProtocol
{
public:
    explicit PtcG2C_TeleportNotice() : CProtocol(PTCG2C_TELEPORTNOTICE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_TeleportNotice()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TeleportNoticeState m_Data;
};

#endif