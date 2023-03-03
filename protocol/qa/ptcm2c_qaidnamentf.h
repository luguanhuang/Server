#ifndef __PTCM2C_QAIDNAMENTF_H__
#define __PTCM2C_QAIDNAMENTF_H__

// generate by ProtoGen at date: 2016/10/9 11:11:33

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_QAIDNAMENTF_TYPE 987

class PtcM2C_QAIDNameNtf : public CProtocol
{
public:
    explicit PtcM2C_QAIDNameNtf() : CProtocol(PTCM2C_QAIDNAMENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_QAIDNameNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::QAIDNameList m_Data;
};

#endif