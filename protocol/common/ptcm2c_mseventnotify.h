#ifndef __PTCM2C_MSEVENTNOTIFY_H__
#define __PTCM2C_MSEVENTNOTIFY_H__

// generate by ProtoGen at date: 2016/8/29 22:13:30

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_MSEVENTNOTIFY_TYPE 1415

class PtcM2C_MSEventNotify : public CProtocol
{
public:
    explicit PtcM2C_MSEventNotify() : CProtocol(PTCM2C_MSEVENTNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_MSEventNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::EventNotify m_Data;
};

#endif