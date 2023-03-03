#ifndef __PTCR2G_NOTIFYSERVERCLOSE_H__
#define __PTCR2G_NOTIFYSERVERCLOSE_H__

// generate by ProtoGen at date: 2017/3/31 17:14:57

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCR2G_NOTIFYSERVERCLOSE_TYPE 39937

class PtcR2G_NotifyServerClose : public CProtocol
{
public:
    explicit PtcR2G_NotifyServerClose() : CProtocol(PTCR2G_NOTIFYSERVERCLOSE_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcR2G_NotifyServerClose()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NotifyServerClose m_Data;
};

#endif