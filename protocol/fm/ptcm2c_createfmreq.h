#ifndef __PTCM2C_CREATEFMREQ_H__
#define __PTCM2C_CREATEFMREQ_H__

// generate by ProtoGen at date: 2016/11/23 11:37:50

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_CREATEFMREQ_TYPE 60545

class PtcM2C_CreateFmReq : public CProtocol
{
public:
    explicit PtcM2C_CreateFmReq() : CProtocol(PTCM2C_CREATEFMREQ_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_CreateFmReq()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::CreateFM m_Data;
};

#endif