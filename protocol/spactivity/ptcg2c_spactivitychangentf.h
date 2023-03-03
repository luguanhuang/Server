#ifndef __PTCG2C_SPACTIVITYCHANGENTF_H__
#define __PTCG2C_SPACTIVITYCHANGENTF_H__

// generate by ProtoGen at date: 2016/10/28 16:43:33

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SPACTIVITYCHANGENTF_TYPE 24832

class PtcG2C_SpActivityChangeNtf : public CProtocol
{
public:
    explicit PtcG2C_SpActivityChangeNtf() : CProtocol(PTCG2C_SPACTIVITYCHANGENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SpActivityChangeNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SpActivityChange m_Data;
};

#endif