#ifndef __PTCM2C_FASTMBCONFIRMM2CNTF_H__
#define __PTCM2C_FASTMBCONFIRMM2CNTF_H__

// generate by ProtoGen at date: 2016/8/19 14:28:47

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_FASTMBCONFIRMM2CNTF_TYPE 58099

class PtcM2C_FastMBConfirmM2CNtf : public CProtocol
{
public:
    explicit PtcM2C_FastMBConfirmM2CNtf() : CProtocol(PTCM2C_FASTMBCONFIRMM2CNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_FastMBConfirmM2CNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FMBArg m_Data;
};

#endif