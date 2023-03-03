#ifndef __PTCG2C_SCENEFINISHSTATECHANGED_H__
#define __PTCG2C_SCENEFINISHSTATECHANGED_H__

// generate by ProtoGen at date: 2015/12/16 16:44:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SCENEFINISHSTATECHANGED_TYPE 60400

class PtcG2C_SceneFinishStateChanged : public CProtocol
{
public:
    explicit PtcG2C_SceneFinishStateChanged() : CProtocol(PTCG2C_SCENEFINISHSTATECHANGED_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SceneFinishStateChanged()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FinishStateInfo m_Data;
};

#endif