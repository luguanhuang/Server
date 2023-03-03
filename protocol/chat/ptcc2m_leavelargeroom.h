#ifndef __PTCC2M_LEAVELARGEROOM_H__
#define __PTCC2M_LEAVELARGEROOM_H__

// generate by ProtoGen at date: 2016/11/24 20:19:34

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_LEAVELARGEROOM_TYPE 55577

class PtcC2M_LeaveLargeRoom : public CProtocol
{
public:
    explicit PtcC2M_LeaveLargeRoom() : CProtocol(PTCC2M_LEAVELARGEROOM_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_LeaveLargeRoom()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LeaveLargeRoomParam m_Data;
};

#endif