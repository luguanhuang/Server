#ifndef __PTCG2C_SCENEDAMAGERANKNTF_H__
#define __PTCG2C_SCENEDAMAGERANKNTF_H__

// generate by ProtoGen at date: 2015/12/24 17:44:39

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SCENEDAMAGERANKNTF_TYPE 26864

class PtcG2C_SceneDamageRankNtf : public CProtocol
{
public:
    explicit PtcG2C_SceneDamageRankNtf() : CProtocol(PTCG2C_SCENEDAMAGERANKNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SceneDamageRankNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SceneDamageRankNtf m_Data;
};

#endif