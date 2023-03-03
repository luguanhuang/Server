#ifndef __PTCG2M_SCENEDESTROYED_H__
#define __PTCG2M_SCENEDESTROYED_H__

// generate by ProtoGen at date: 2016/7/26 13:08:53

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_SCENEDESTROYED_TYPE 63408

class PtcG2M_SceneDestroyed : public CProtocol
{
public:
    explicit PtcG2M_SceneDestroyed() : CProtocol(PTCG2M_SCENEDESTROYED_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_SceneDestroyed()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SceneDestroyedData m_Data;
};

#endif