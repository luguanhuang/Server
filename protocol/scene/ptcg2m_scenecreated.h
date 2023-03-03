#ifndef __PTCG2M_SCENECREATED_H__
#define __PTCG2M_SCENECREATED_H__

// generate by ProtoGen at date: 2016/11/3 23:44:10

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_SCENECREATED_TYPE 43003

class PtcG2M_SceneCreated : public CProtocol
{
public:
    explicit PtcG2M_SceneCreated() : CProtocol(PTCG2M_SCENECREATED_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_SceneCreated()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SceneCreatedData m_Data;
};

#endif