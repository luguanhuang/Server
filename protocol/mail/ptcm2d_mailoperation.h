#ifndef __PTCM2D_MAILOPERATION_H__
#define __PTCM2D_MAILOPERATION_H__

// generate by ProtoGen at date: 2016/7/21 16:26:51

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_MAILOPERATION_TYPE 19226

class PtcM2D_MailOperation : public CProtocol
{
public:
    explicit PtcM2D_MailOperation() : CProtocol(PTCM2D_MAILOPERATION_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_MailOperation()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MailOperationData m_Data;
};

#endif