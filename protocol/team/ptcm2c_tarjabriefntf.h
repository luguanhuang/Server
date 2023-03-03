#ifndef __PTCM2C_TARJABRIEFNTF_H__
#define __PTCM2C_TARJABRIEFNTF_H__

// generate by ProtoGen at date: 2017/5/16 17:21:00

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_TARJABRIEFNTF_TYPE 35068

class PtcM2C_TarjaBriefNtf : public CProtocol
{
public:
    explicit PtcM2C_TarjaBriefNtf() : CProtocol(PTCM2C_TARJABRIEFNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_TarjaBriefNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TarjaBrief m_Data;
};

#endif