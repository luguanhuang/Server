#ifndef __PTCM2G_NPCFLNOTIFYM2G_H__
#define __PTCM2G_NPCFLNOTIFYM2G_H__

// generate by ProtoGen at date: 2017/9/12 10:33:44

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_NPCFLNOTIFYM2G_TYPE 20387

class PtcM2G_NpcFlNotifyM2G : public CProtocol
{
public:
    explicit PtcM2G_NpcFlNotifyM2G() : CProtocol(PTCM2G_NPCFLNOTIFYM2G_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_NpcFlNotifyM2G()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NpcFlPara m_Data;
};

#endif