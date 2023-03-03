#ifndef __PTCG2K_BMLEAVEFIGHTSCENENTF_H__
#define __PTCG2K_BMLEAVEFIGHTSCENENTF_H__

// generate by ProtoGen at date: 2017/6/22 8:18:44

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2K_BMLEAVEFIGHTSCENENTF_TYPE 45898

class PtcG2K_BMLeaveFightSceneNtf : public CProtocol
{
public:
    explicit PtcG2K_BMLeaveFightSceneNtf() : CProtocol(PTCG2K_BMLEAVEFIGHTSCENENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2K_BMLeaveFightSceneNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BMEnterSvrInfo m_Data;
};

#endif