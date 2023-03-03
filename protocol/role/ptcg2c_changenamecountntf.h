#ifndef __PTCG2C_CHANGENAMECOUNTNTF_H__
#define __PTCG2C_CHANGENAMECOUNTNTF_H__

// generate by ProtoGen at date: 2016/11/29 16:34:27

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_CHANGENAMECOUNTNTF_TYPE 59287

class PtcG2C_ChangeNameCountNtf : public CProtocol
{
public:
    explicit PtcG2C_ChangeNameCountNtf() : CProtocol(PTCG2C_CHANGENAMECOUNTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_ChangeNameCountNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ChangeNameCountNtf m_Data;
};

#endif