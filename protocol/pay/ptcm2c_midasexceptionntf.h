#ifndef __PTCM2C_MIDASEXCEPTIONNTF_H__
#define __PTCM2C_MIDASEXCEPTIONNTF_H__

// generate by ProtoGen at date: 2017/1/11 21:55:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_MIDASEXCEPTIONNTF_TYPE 22947

class PtcM2C_MidasExceptionNtf : public CProtocol
{
public:
    explicit PtcM2C_MidasExceptionNtf() : CProtocol(PTCM2C_MIDASEXCEPTIONNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_MidasExceptionNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MidasExceptionInfo m_Data;
};

#endif