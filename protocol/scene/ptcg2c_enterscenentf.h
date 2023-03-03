#ifndef __PTCG2C_ENTERSCENENTF_H__
#define __PTCG2C_ENTERSCENENTF_H__

// generate by ProtoGen at date: 2017/3/15 10:56:14

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_ENTERSCENENTF_TYPE 63366

class PtcG2C_EnterSceneNtf : public CProtocol
{
public:
    explicit PtcG2C_EnterSceneNtf() : CProtocol(PTCG2C_ENTERSCENENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_EnterSceneNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SceneCfg m_Data;
};

#endif