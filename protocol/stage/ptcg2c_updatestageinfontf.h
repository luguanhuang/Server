#ifndef __PTCG2C_UPDATESTAGEINFONTF_H__
#define __PTCG2C_UPDATESTAGEINFONTF_H__

// generate by ProtoGen at date: 2017/11/8 14:10:06

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_UPDATESTAGEINFONTF_TYPE 21189

class PtcG2C_UpdateStageInfoNtf : public CProtocol
{
public:
    explicit PtcG2C_UpdateStageInfoNtf() : CProtocol(PTCG2C_UPDATESTAGEINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_UpdateStageInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateStageInfoNtf m_Data;
};

#endif