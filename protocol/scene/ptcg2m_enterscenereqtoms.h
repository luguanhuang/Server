#ifndef __PTCG2M_ENTERSCENEREQTOMS_H__
#define __PTCG2M_ENTERSCENEREQTOMS_H__

// generate by ProtoGen at date: 2016/11/5 1:39:16

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_ENTERSCENEREQTOMS_TYPE 14394

class PtcG2M_EnterSceneReqToMs : public CProtocol
{
public:
    explicit PtcG2M_EnterSceneReqToMs() : CProtocol(PTCG2M_ENTERSCENEREQTOMS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_EnterSceneReqToMs()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::EnterSceneReqToMsData m_Data;
};

#endif