#ifndef __PTCG2C_HORSEFINALNTF_H__
#define __PTCG2C_HORSEFINALNTF_H__

// generate by ProtoGen at date: 2016/12/9 23:10:49

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_HORSEFINALNTF_TYPE 57969

class PtcG2C_HorseFinalNtf : public CProtocol
{
public:
    explicit PtcG2C_HorseFinalNtf() : CProtocol(PTCG2C_HORSEFINALNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_HorseFinalNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HorseFinal m_Data;
};

#endif