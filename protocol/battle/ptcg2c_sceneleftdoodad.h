#ifndef __PTCG2C_SCENELEFTDOODAD_H__
#define __PTCG2C_SCENELEFTDOODAD_H__

// generate by ProtoGen at date: 2015/12/29 19:51:23

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SCENELEFTDOODAD_TYPE 18028

class PtcG2C_SceneLeftDoodad : public CProtocol
{
public:
    explicit PtcG2C_SceneLeftDoodad() : CProtocol(PTCG2C_SCENELEFTDOODAD_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SceneLeftDoodad()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SceneLeftDoodad m_Data;
};

#endif