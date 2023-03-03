#ifndef __PTCC2G_JOINROOM_H__
#define __PTCC2G_JOINROOM_H__

// generate by ProtoGen at date: 2016/8/22 21:17:19

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_JOINROOM_TYPE 8517

class PtcC2G_JoinRoom : public CProtocol
{
public:
    explicit PtcC2G_JoinRoom() : CProtocol(PTCC2G_JOINROOM_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_JoinRoom()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::JoinRoom m_Data;
};

#endif