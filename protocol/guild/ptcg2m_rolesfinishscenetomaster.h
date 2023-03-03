#ifndef __PTCG2M_ROLESFINISHSCENETOMASTER_H__
#define __PTCG2M_ROLESFINISHSCENETOMASTER_H__

// generate by ProtoGen at date: 2016/7/28 10:47:24

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_ROLESFINISHSCENETOMASTER_TYPE 57908

class PtcG2M_RolesFinishSceneToMaster : public CProtocol
{
public:
    explicit PtcG2M_RolesFinishSceneToMaster() : CProtocol(PTCG2M_ROLESFINISHSCENETOMASTER_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_RolesFinishSceneToMaster()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RolesFinishScene m_Data;
};

#endif