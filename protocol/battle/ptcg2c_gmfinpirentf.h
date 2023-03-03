#ifndef __PTCG2C_GMFINPIRENTF_H__
#define __PTCG2C_GMFINPIRENTF_H__

// generate by ProtoGen at date: 2016/10/3 16:25:41

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GMFINPIRENTF_TYPE 16122

class PtcG2C_GmfInpireNtf : public CProtocol
{
public:
    explicit PtcG2C_GmfInpireNtf() : CProtocol(PTCG2C_GMFINPIRENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GmfInpireNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GmfInspirePara m_Data;
};

#endif