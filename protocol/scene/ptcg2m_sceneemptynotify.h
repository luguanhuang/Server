#ifndef __PTCG2M_SCENEEMPTYNOTIFY_H__
#define __PTCG2M_SCENEEMPTYNOTIFY_H__

// generate by ProtoGen at date: 2016/11/30 23:36:19

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_SCENEEMPTYNOTIFY_TYPE 59605

class PtcG2M_SceneEmptyNotify : public CProtocol
{
public:
    explicit PtcG2M_SceneEmptyNotify() : CProtocol(PTCG2M_SCENEEMPTYNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_SceneEmptyNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SceneEmptyNotify m_Data;
};

#endif