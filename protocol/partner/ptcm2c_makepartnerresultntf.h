#ifndef __PTCM2C_MAKEPARTNERRESULTNTF_H__
#define __PTCM2C_MAKEPARTNERRESULTNTF_H__

// generate by ProtoGen at date: 2017/3/15 10:56:36

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_MAKEPARTNERRESULTNTF_TYPE 49652

class PtcM2C_MakePartnerResultNtf : public CProtocol
{
public:
    explicit PtcM2C_MakePartnerResultNtf() : CProtocol(PTCM2C_MAKEPARTNERRESULTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_MakePartnerResultNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MakePartnerResult m_Data;
};

#endif