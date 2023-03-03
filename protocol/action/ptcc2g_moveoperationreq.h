#ifndef __PTCC2G_MOVEOPERATIONREQ_H__
#define __PTCC2G_MOVEOPERATIONREQ_H__

// generate by ProtoGen at date: 2015/12/16 14:56:27

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_MOVEOPERATIONREQ_TYPE 30732

class PtcC2G_MoveOperationReq : public CProtocol
{
public:
    explicit PtcC2G_MoveOperationReq() : CProtocol(PTCC2G_MOVEOPERATIONREQ_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_MoveOperationReq()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MoveInfo m_Data;
};

#endif