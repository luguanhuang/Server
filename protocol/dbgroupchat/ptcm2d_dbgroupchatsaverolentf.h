#ifndef __PTCM2D_DBGROUPCHATSAVEROLENTF_H__
#define __PTCM2D_DBGROUPCHATSAVEROLENTF_H__

// generate by ProtoGen at date: 2017/7/28 15:43:41

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_DBGROUPCHATSAVEROLENTF_TYPE 43081

class PtcM2D_DBGroupChatSaveRoleNtf : public CProtocol
{
public:
    explicit PtcM2D_DBGroupChatSaveRoleNtf() : CProtocol(PTCM2D_DBGROUPCHATSAVEROLENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_DBGroupChatSaveRoleNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DBGroupChatSaveRole m_Data;
};

#endif