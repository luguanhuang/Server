#ifndef __PTCM2D_DELROLENTF_H__
#define __PTCM2D_DELROLENTF_H__

// generate by ProtoGen at date: 2016/10/15 14:33:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_DELROLENTF_TYPE 64495

class PtcM2D_DelRoleNtf : public CProtocol
{
public:
    explicit PtcM2D_DelRoleNtf() : CProtocol(PTCM2D_DELROLENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_DelRoleNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DelRoleNtf m_Data;
};

#endif