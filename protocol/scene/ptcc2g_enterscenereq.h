#ifndef __PTCC2G_ENTERSCENEREQ_H__
#define __PTCC2G_ENTERSCENEREQ_H__

// generate by ProtoGen at date: 2017/3/15 10:56:11

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_ENTERSCENEREQ_TYPE 9036

class PtcC2G_EnterSceneReq : public CProtocol
{
public:
    explicit PtcC2G_EnterSceneReq() : CProtocol(PTCC2G_ENTERSCENEREQ_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_EnterSceneReq()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SceneRequest m_Data;
};

#endif