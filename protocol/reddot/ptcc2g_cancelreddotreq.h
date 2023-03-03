#ifndef __PTCC2G_CANCELREDDOTREQ_H__
#define __PTCC2G_CANCELREDDOTREQ_H__

// generate by ProtoGen at date: 2017/1/11 17:59:51

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_CANCELREDDOTREQ_TYPE 40873

class PtcC2G_CancelRedDotReq : public CProtocol
{
public:
    explicit PtcC2G_CancelRedDotReq() : CProtocol(PTCC2G_CANCELREDDOTREQ_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_CancelRedDotReq()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::CancelRedDot m_Data;
};

#endif