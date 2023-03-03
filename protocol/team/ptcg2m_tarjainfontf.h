#ifndef __PTCG2M_TARJAINFONTF_H__
#define __PTCG2M_TARJAINFONTF_H__

// generate by ProtoGen at date: 2017/5/16 14:11:11

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_TARJAINFONTF_TYPE 41859

class PtcG2M_TarjaInfoNtf : public CProtocol
{
public:
    explicit PtcG2M_TarjaInfoNtf() : CProtocol(PTCG2M_TARJAINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_TarjaInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TarjaInfo m_Data;
};

#endif