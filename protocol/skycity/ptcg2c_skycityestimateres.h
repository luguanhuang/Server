#ifndef __PTCG2C_SKYCITYESTIMATERES_H__
#define __PTCG2C_SKYCITYESTIMATERES_H__

// generate by ProtoGen at date: 2016/10/1 22:05:11

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SKYCITYESTIMATERES_TYPE 36139

class PtcG2C_SkyCityEstimateRes : public CProtocol
{
public:
    explicit PtcG2C_SkyCityEstimateRes() : CProtocol(PTCG2C_SKYCITYESTIMATERES_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SkyCityEstimateRes()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SkyCityEstimateInfo m_Data;
};

#endif