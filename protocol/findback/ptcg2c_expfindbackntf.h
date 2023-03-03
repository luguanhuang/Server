#ifndef __PTCG2C_EXPFINDBACKNTF_H__
#define __PTCG2C_EXPFINDBACKNTF_H__

// generate by ProtoGen at date: 2016/5/10 10:51:35

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_EXPFINDBACKNTF_TYPE 4933

class PtcG2C_ExpFindBackNtf : public CProtocol
{
public:
    explicit PtcG2C_ExpFindBackNtf() : CProtocol(PTCG2C_EXPFINDBACKNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ExpFindBackNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ExpFindBackData m_Data;
};

#endif