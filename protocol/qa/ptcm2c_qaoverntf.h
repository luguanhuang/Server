#ifndef __PTCM2C_QAOVERNTF_H__
#define __PTCM2C_QAOVERNTF_H__

// generate by ProtoGen at date: 2017/2/7 3:14:18

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_QAOVERNTF_TYPE 29361

class PtcM2C_QAOverNtf : public CProtocol
{
public:
    explicit PtcM2C_QAOverNtf() : CProtocol(PTCM2C_QAOVERNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_QAOverNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::QAOverNtf m_Data;
};

#endif