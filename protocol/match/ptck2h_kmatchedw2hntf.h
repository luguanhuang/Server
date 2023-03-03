#ifndef __PTCK2H_KMATCHEDW2HNTF_H__
#define __PTCK2H_KMATCHEDW2HNTF_H__

// generate by ProtoGen at date: 2017/5/5 21:21:26

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2H_KMATCHEDW2HNTF_TYPE 29657

class PtcK2H_KMatchedw2hNtf : public CProtocol
{
public:
    explicit PtcK2H_KMatchedw2hNtf() : CProtocol(PTCK2H_KMATCHEDW2HNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2H_KMatchedw2hNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::KMatchedPara m_Data;
};

#endif