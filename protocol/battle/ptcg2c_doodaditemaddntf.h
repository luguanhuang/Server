#ifndef __PTCG2C_DOODADITEMADDNTF_H__
#define __PTCG2C_DOODADITEMADDNTF_H__

// generate by ProtoGen at date: 2016/12/14 21:27:09

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_DOODADITEMADDNTF_TYPE 16613

class PtcG2C_DoodadItemAddNtf : public CProtocol
{
public:
    explicit PtcG2C_DoodadItemAddNtf() : CProtocol(PTCG2C_DOODADITEMADDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_DoodadItemAddNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DoodadItemAddNtf m_Data;
};

#endif