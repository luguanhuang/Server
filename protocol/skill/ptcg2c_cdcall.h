#ifndef __PTCG2C_CDCALL_H__
#define __PTCG2C_CDCALL_H__

// generate by ProtoGen at date: 2016/11/22 15:04:10

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_CDCALL_TYPE 34744

class PtcG2C_CdCall : public CProtocol
{
public:
    explicit PtcG2C_CdCall() : CProtocol(PTCG2C_CDCALL_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_CdCall()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::CallData m_Data;
};

#endif