#ifndef __PTCM2C_FASTMBDISMISSM2CNTF_H__
#define __PTCM2C_FASTMBDISMISSM2CNTF_H__

// generate by ProtoGen at date: 2016/8/19 14:29:28

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_FASTMBDISMISSM2CNTF_TYPE 38301

class PtcM2C_fastMBDismissM2CNtf : public CProtocol
{
public:
    explicit PtcM2C_fastMBDismissM2CNtf() : CProtocol(PTCM2C_FASTMBDISMISSM2CNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_fastMBDismissM2CNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FMDArg m_Data;
};

#endif