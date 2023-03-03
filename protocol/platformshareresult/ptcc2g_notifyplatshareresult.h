#ifndef __PTCC2G_NOTIFYPLATSHARERESULT_H__
#define __PTCC2G_NOTIFYPLATSHARERESULT_H__

// generate by ProtoGen at date: 2017/5/31 21:33:22

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_NOTIFYPLATSHARERESULT_TYPE 8480

class PtcC2G_NotifyPlatShareResult : public CProtocol
{
public:
    explicit PtcC2G_NotifyPlatShareResult() : CProtocol(PTCC2G_NOTIFYPLATSHARERESULT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_NotifyPlatShareResult()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NotifyPlatShareResultArg m_Data;
};

#endif