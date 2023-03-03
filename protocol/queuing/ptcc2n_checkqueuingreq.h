#ifndef __PTCC2N_CHECKQUEUINGREQ_H__
#define __PTCC2N_CHECKQUEUINGREQ_H__

// generate by ProtoGen at date: 2016/11/16 16:37:20

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2N_CHECKQUEUINGREQ_TYPE 28232

class PtcC2N_CheckQueuingReq : public CProtocol
{
public:
    explicit PtcC2N_CheckQueuingReq() : CProtocol(PTCC2N_CHECKQUEUINGREQ_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2N_CheckQueuingReq()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::CheckQueuingReq m_Data;
};

#endif