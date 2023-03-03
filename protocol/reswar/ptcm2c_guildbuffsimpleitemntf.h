#ifndef __PTCM2C_GUILDBUFFSIMPLEITEMNTF_H__
#define __PTCM2C_GUILDBUFFSIMPLEITEMNTF_H__

// generate by ProtoGen at date: 2016/11/12 15:27:06

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_GUILDBUFFSIMPLEITEMNTF_TYPE 63964

class PtcM2C_GuildBuffSimpleItemNtf : public CProtocol
{
public:
    explicit PtcM2C_GuildBuffSimpleItemNtf() : CProtocol(PTCM2C_GUILDBUFFSIMPLEITEMNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_GuildBuffSimpleItemNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildBuffSimpleItem m_Data;
};

#endif