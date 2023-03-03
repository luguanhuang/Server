#ifndef __PTCR2G_NOTIFYSERVERCONNECT_H__
#define __PTCR2G_NOTIFYSERVERCONNECT_H__

// generate by ProtoGen at date: 2016/12/27 18:11:17

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCR2G_NOTIFYSERVERCONNECT_TYPE 13122

class PtcR2G_NotifyServerConnect : public CProtocol
{
public:
    explicit PtcR2G_NotifyServerConnect() : CProtocol(PTCR2G_NOTIFYSERVERCONNECT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcR2G_NotifyServerConnect()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NotifyServerConnect m_Data;
};

#endif