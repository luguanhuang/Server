#ifndef __PTCM2C_LOGINGUILDINFO_H__
#define __PTCM2C_LOGINGUILDINFO_H__

// generate by ProtoGen at date: 2016/8/26 14:29:59

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_LOGINGUILDINFO_TYPE 29049

class PtcM2C_LoginGuildInfo : public CProtocol
{
public:
    explicit PtcM2C_LoginGuildInfo() : CProtocol(PTCM2C_LOGINGUILDINFO_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_LoginGuildInfo()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MyGuild m_Data;
};

#endif