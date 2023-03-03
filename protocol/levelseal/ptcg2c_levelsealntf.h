#ifndef __PTCG2C_LEVELSEALNTF_H__
#define __PTCG2C_LEVELSEALNTF_H__

// generate by ProtoGen at date: 2016/6/16 14:25:26

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_LEVELSEALNTF_TYPE 40338

class PtcG2C_LevelSealNtf : public CProtocol
{
public:
    explicit PtcG2C_LevelSealNtf() : CProtocol(PTCG2C_LEVELSEALNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_LevelSealNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LevelSealInfo m_Data;
};

#endif