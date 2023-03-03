#ifndef __PTCC2G_QUITROOM_H__
#define __PTCC2G_QUITROOM_H__

// generate by ProtoGen at date: 2016/8/9 15:30:47

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_QUITROOM_TYPE 44925

class PtcC2G_QuitRoom : public CProtocol
{
public:
    explicit PtcC2G_QuitRoom() : CProtocol(PTCC2G_QUITROOM_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_QuitRoom()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::QuitRoom m_Data;
};

#endif