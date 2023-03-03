#ifndef __PTCG2C_JOINROOMREPLY_H__
#define __PTCG2C_JOINROOMREPLY_H__

// generate by ProtoGen at date: 2016/8/9 15:58:59

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_JOINROOMREPLY_TYPE 23084

class PtcG2C_JoinRoomReply : public CProtocol
{
public:
    explicit PtcG2C_JoinRoomReply() : CProtocol(PTCG2C_JOINROOMREPLY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_JoinRoomReply()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::JoinRoomReply m_Data;
};

#endif