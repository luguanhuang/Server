#ifndef __PTCG2M_SCENEENDTOMASTER_H__
#define __PTCG2M_SCENEENDTOMASTER_H__

// generate by ProtoGen at date: 2016/7/28 14:32:53

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_SCENEENDTOMASTER_TYPE 26131

class PtcG2M_SceneEndToMaster : public CProtocol
{
public:
    explicit PtcG2M_SceneEndToMaster() : CProtocol(PTCG2M_SCENEENDTOMASTER_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_SceneEndToMaster()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SceneEndData m_Data;
};

#endif