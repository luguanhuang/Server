#ifndef __PTCM2C_LARGEROOMROLENTF_H__
#define __PTCM2C_LARGEROOMROLENTF_H__

// generate by ProtoGen at date: 2016/11/24 20:19:24

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_LARGEROOMROLENTF_TYPE 36333

class PtcM2C_LargeRoomRoleNtf : public CProtocol
{
public:
    explicit PtcM2C_LargeRoomRoleNtf() : CProtocol(PTCM2C_LARGEROOMROLENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_LargeRoomRoleNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LargeRoomRoleParam m_Data;
};

#endif