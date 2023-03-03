#ifndef __PTCM2D_DBGROUPCHATDELROLENTF_H__
#define __PTCM2D_DBGROUPCHATDELROLENTF_H__

// generate by ProtoGen at date: 2017/7/28 15:44:02

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_DBGROUPCHATDELROLENTF_TYPE 24442

class PtcM2D_DBGroupChatDelRoleNtf : public CProtocol
{
public:
    explicit PtcM2D_DBGroupChatDelRoleNtf() : CProtocol(PTCM2D_DBGROUPCHATDELROLENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_DBGroupChatDelRoleNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DBGroupChatDelRole m_Data;
};

#endif