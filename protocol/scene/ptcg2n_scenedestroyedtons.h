#ifndef __PTCG2N_SCENEDESTROYEDTONS_H__
#define __PTCG2N_SCENEDESTROYEDTONS_H__

// generate by ProtoGen at date: 2016/11/17 22:30:47

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2N_SCENEDESTROYEDTONS_TYPE 2719

class PtcG2N_SceneDestroyedToNs : public CProtocol
{
public:
    explicit PtcG2N_SceneDestroyedToNs() : CProtocol(PTCG2N_SCENEDESTROYEDTONS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2N_SceneDestroyedToNs()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SceneDestroyedData m_Data;
};

#endif