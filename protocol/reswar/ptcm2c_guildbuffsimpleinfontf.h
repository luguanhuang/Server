#ifndef __PTCM2C_GUILDBUFFSIMPLEINFONTF_H__
#define __PTCM2C_GUILDBUFFSIMPLEINFONTF_H__

// generate by ProtoGen at date: 2016/11/11 21:50:29

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_GUILDBUFFSIMPLEINFONTF_TYPE 57161

class PtcM2C_GuildBuffSimpleInfoNtf : public CProtocol
{
public:
    explicit PtcM2C_GuildBuffSimpleInfoNtf() : CProtocol(PTCM2C_GUILDBUFFSIMPLEINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_GuildBuffSimpleInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GuildBuffSimpleAllInfo m_Data;
};

#endif