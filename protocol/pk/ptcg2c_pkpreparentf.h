#ifndef __PTCG2C_PKPREPARENTF_H__
#define __PTCG2C_PKPREPARENTF_H__

// generate by ProtoGen at date: 2015/12/10 11:13:19

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_PKPREPARENTF_TYPE 41409

class PtcG2C_PkPrepareNtf : public CProtocol
{
public:
    explicit PtcG2C_PkPrepareNtf() : CProtocol(PTCG2C_PKPREPARENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_PkPrepareNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PkPrepareNtf m_Data;
};

#endif