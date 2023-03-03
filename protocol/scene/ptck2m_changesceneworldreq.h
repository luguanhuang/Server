#ifndef __PTCK2M_CHANGESCENEWORLDREQ_H__
#define __PTCK2M_CHANGESCENEWORLDREQ_H__

// generate by ProtoGen at date: 2017/4/21 15:17:15

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2M_CHANGESCENEWORLDREQ_TYPE 16478

class PtcK2M_ChangeSceneWorldReq : public CProtocol
{
public:
    explicit PtcK2M_ChangeSceneWorldReq() : CProtocol(PTCK2M_CHANGESCENEWORLDREQ_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2M_ChangeSceneWorldReq()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ChangeSceneWorldReq m_Data;
};

#endif