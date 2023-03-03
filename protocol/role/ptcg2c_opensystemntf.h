#ifndef __PTCG2C_OPENSYSTEMNTF_H__
#define __PTCG2C_OPENSYSTEMNTF_H__

// generate by ProtoGen at date: 2015/2/2 15:10:52

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_OPENSYSTEMNTF_TYPE 41168

class PtcG2C_OpenSystemNtf : public CProtocol
{
public:
    explicit PtcG2C_OpenSystemNtf() : CProtocol(PTCG2C_OPENSYSTEMNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_OpenSystemNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::Systems m_Data;
};

#endif