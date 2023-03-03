#ifndef __PTCG2C_SPECIALSTATENTF_H__
#define __PTCG2C_SPECIALSTATENTF_H__

// generate by ProtoGen at date: 2016/12/26 17:55:52

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SPECIALSTATENTF_TYPE 11703

class PtcG2C_SpecialStateNtf : public CProtocol
{
public:
    explicit PtcG2C_SpecialStateNtf() : CProtocol(PTCG2C_SPECIALSTATENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SpecialStateNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SpecialStateNtf m_Data;
};

#endif