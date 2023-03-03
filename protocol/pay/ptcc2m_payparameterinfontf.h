#ifndef __PTCC2M_PAYPARAMETERINFONTF_H__
#define __PTCC2M_PAYPARAMETERINFONTF_H__

// generate by ProtoGen at date: 2016/12/19 22:30:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_PAYPARAMETERINFONTF_TYPE 1181

class PtcC2M_PayParameterInfoNtf : public CProtocol
{
public:
    explicit PtcC2M_PayParameterInfoNtf() : CProtocol(PTCC2M_PAYPARAMETERINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_PayParameterInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PayParameterInfo m_Data;
};

#endif