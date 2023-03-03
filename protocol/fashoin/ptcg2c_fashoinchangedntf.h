#ifndef __PTCG2C_FASHOINCHANGEDNTF_H__
#define __PTCG2C_FASHOINCHANGEDNTF_H__

// generate by ProtoGen at date: 2015/9/14 15:23:58

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_FASHOINCHANGEDNTF_TYPE 12350

class PtcG2C_FashoinChangedNtf : public CProtocol
{
public:
    explicit PtcG2C_FashoinChangedNtf() : CProtocol(PTCG2C_FASHOINCHANGEDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_FashoinChangedNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FashionChangedData m_Data;
};

#endif