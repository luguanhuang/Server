#ifndef __PTCG2C_SCENESTATENTF_H__
#define __PTCG2C_SCENESTATENTF_H__

// generate by ProtoGen at date: 2016/12/28 11:09:05

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SCENESTATENTF_TYPE 4376

class PtcG2C_SceneStateNtf : public CProtocol
{
public:
    explicit PtcG2C_SceneStateNtf() : CProtocol(PTCG2C_SCENESTATENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SceneStateNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SceneStateNtf m_Data;
};

#endif