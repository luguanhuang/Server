#ifndef __PTCG2C_WEDDINGLOADINFONTF_H__
#define __PTCG2C_WEDDINGLOADINFONTF_H__

// generate by ProtoGen at date: 2017/7/26 16:27:47

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_WEDDINGLOADINFONTF_TYPE 61694

class PtcG2C_WeddingLoadInfoNtf : public CProtocol
{
public:
    explicit PtcG2C_WeddingLoadInfoNtf() : CProtocol(PTCG2C_WEDDINGLOADINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_WeddingLoadInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::WeddingLoadInfoNtf m_Data;
};

#endif