#ifndef __PTCM2C_FMSYSTEMNTF_H__
#define __PTCM2C_FMSYSTEMNTF_H__

// generate by ProtoGen at date: 2016/11/29 16:26:51

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_FMSYSTEMNTF_TYPE 22361

class PtcM2C_FMSystemNtf : public CProtocol
{
public:
    explicit PtcM2C_FMSystemNtf() : CProtocol(PTCM2C_FMSYSTEMNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_FMSystemNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FMSystem m_Data;
};

#endif