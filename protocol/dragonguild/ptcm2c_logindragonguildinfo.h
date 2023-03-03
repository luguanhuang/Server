#ifndef __PTCM2C_LOGINDRAGONGUILDINFO_H__
#define __PTCM2C_LOGINDRAGONGUILDINFO_H__

// generate by ProtoGen at date: 2017/9/7 11:35:52

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_LOGINDRAGONGUILDINFO_TYPE 21856

class PtcM2C_LoginDragonGuildInfo : public CProtocol
{
public:
    explicit PtcM2C_LoginDragonGuildInfo() : CProtocol(PTCM2C_LOGINDRAGONGUILDINFO_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_LoginDragonGuildInfo()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::MyDragonGuild m_Data;
};

#endif