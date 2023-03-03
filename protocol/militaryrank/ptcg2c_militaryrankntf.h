#ifndef __PTCG2C_MILITARYRANKNTF_H__
#define __PTCG2C_MILITARYRANKNTF_H__

// generate by ProtoGen at date: 2017/4/11 16:08:13

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_MILITARYRANKNTF_TYPE 64945

class PtcG2C_MilitaryrankNtf : public CProtocol
{
public:
    explicit PtcG2C_MilitaryrankNtf() : CProtocol(PTCG2C_MILITARYRANKNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_MilitaryrankNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MilitaryRecord m_Data;
};

#endif