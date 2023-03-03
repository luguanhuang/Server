#ifndef __PTCC2G_DELAYNOTIFY_H__
#define __PTCC2G_DELAYNOTIFY_H__

// generate by ProtoGen at date: 2015/12/5 17:13:46

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_DELAYNOTIFY_TYPE 46829

class PtcC2G_DelayNotify : public CProtocol
{
public:
    explicit PtcC2G_DelayNotify() : CProtocol(PTCC2G_DELAYNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_DelayNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DelayInfo m_Data;
};

#endif