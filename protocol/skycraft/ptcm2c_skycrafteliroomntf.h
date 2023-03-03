#ifndef __PTCM2C_SKYCRAFTELIROOMNTF_H__
#define __PTCM2C_SKYCRAFTELIROOMNTF_H__

// generate by ProtoGen at date: 2017/4/11 17:59:40

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_SKYCRAFTELIROOMNTF_TYPE 6761

class PtcM2C_SkyCraftEliRoomNtf : public CProtocol
{
public:
    explicit PtcM2C_SkyCraftEliRoomNtf() : CProtocol(PTCM2C_SKYCRAFTELIROOMNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_SkyCraftEliRoomNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SkyCraftEliRoomNtf m_Data;
};

#endif