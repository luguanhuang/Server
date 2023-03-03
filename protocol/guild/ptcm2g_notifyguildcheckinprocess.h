#ifndef __PTCM2G_NOTIFYGUILDCHECKINPROCESS_H__
#define __PTCM2G_NOTIFYGUILDCHECKINPROCESS_H__

// generate by ProtoGen at date: 2016/9/18 16:18:04

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_NOTIFYGUILDCHECKINPROCESS_TYPE 42460

class PtcM2G_NotifyGuildCheckinProcess : public CProtocol
{
public:
    explicit PtcM2G_NotifyGuildCheckinProcess() : CProtocol(PTCM2G_NOTIFYGUILDCHECKINPROCESS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_NotifyGuildCheckinProcess()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NotifyGuildCheckinProcess m_Data;
};

#endif