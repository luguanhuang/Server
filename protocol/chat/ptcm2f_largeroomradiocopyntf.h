#ifndef __PTCM2F_LARGEROOMRADIOCOPYNTF_H__
#define __PTCM2F_LARGEROOMRADIOCOPYNTF_H__

// generate by ProtoGen at date: 2017/2/19 21:05:46

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2F_LARGEROOMRADIOCOPYNTF_TYPE 8019

class PtcM2F_LargeRoomRadioCopyNtf : public CProtocol
{
public:
    explicit PtcM2F_LargeRoomRadioCopyNtf() : CProtocol(PTCM2F_LARGEROOMRADIOCOPYNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2F_LargeRoomRadioCopyNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LargeRoomRoleParam m_Data;
};

#endif