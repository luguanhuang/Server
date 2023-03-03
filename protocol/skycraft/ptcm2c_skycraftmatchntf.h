#ifndef __PTCM2C_SKYCRAFTMATCHNTF_H__
#define __PTCM2C_SKYCRAFTMATCHNTF_H__

// generate by ProtoGen at date: 2017/4/1 14:35:50

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_SKYCRAFTMATCHNTF_TYPE 4938

class PtcM2C_SkyCraftMatchNtf : public CProtocol
{
public:
    explicit PtcM2C_SkyCraftMatchNtf() : CProtocol(PTCM2C_SKYCRAFTMATCHNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_SkyCraftMatchNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SkyCraftMatchNtf m_Data;
};

#endif