#ifndef __PTCK2M_HEROWORLDROOMNTF_H__
#define __PTCK2M_HEROWORLDROOMNTF_H__

// generate by ProtoGen at date: 2017/2/4 23:04:27

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2M_HEROWORLDROOMNTF_TYPE 35021

class PtcK2M_HeroWorldRoomNtf : public CProtocol
{
public:
    explicit PtcK2M_HeroWorldRoomNtf() : CProtocol(PTCK2M_HEROWORLDROOMNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2M_HeroWorldRoomNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::HeroWorldRoomData m_Data;
};

#endif