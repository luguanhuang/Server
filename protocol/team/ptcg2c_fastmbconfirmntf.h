#ifndef __PTCG2C_FASTMBCONFIRMNTF_H__
#define __PTCG2C_FASTMBCONFIRMNTF_H__

// generate by ProtoGen at date: 2016/7/4 11:50:20

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_FASTMBCONFIRMNTF_TYPE 51623

class PtcG2C_FastMBConfirmNtf : public CProtocol
{
public:
    explicit PtcG2C_FastMBConfirmNtf() : CProtocol(PTCG2C_FASTMBCONFIRMNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_FastMBConfirmNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FMBArg m_Data;
};

#endif