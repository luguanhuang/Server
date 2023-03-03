#ifndef __PTCG2C_FASTMBDISMISSNTF_H__
#define __PTCG2C_FASTMBDISMISSNTF_H__

// generate by ProtoGen at date: 2016/7/4 16:09:01

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_FASTMBDISMISSNTF_TYPE 49087

class PtcG2C_fastMBDismissNtf : public CProtocol
{
public:
    explicit PtcG2C_fastMBDismissNtf() : CProtocol(PTCG2C_FASTMBDISMISSNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_fastMBDismissNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FMDArg m_Data;
};

#endif