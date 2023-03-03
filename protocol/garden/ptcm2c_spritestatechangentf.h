#ifndef __PTCM2C_SPRITESTATECHANGENTF_H__
#define __PTCM2C_SPRITESTATECHANGENTF_H__

// generate by ProtoGen at date: 2016/10/14 19:13:29

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_SPRITESTATECHANGENTF_TYPE 38584

class PtcM2C_SpriteStateChangeNtf : public CProtocol
{
public:
    explicit PtcM2C_SpriteStateChangeNtf() : CProtocol(PTCM2C_SPRITESTATECHANGENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_SpriteStateChangeNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SpriteState m_Data;
};

#endif