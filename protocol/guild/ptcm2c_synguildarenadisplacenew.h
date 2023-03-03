#ifndef __PTCM2C_SYNGUILDARENADISPLACENEW_H__
#define __PTCM2C_SYNGUILDARENADISPLACENEW_H__

// generate by ProtoGen at date: 2016/9/8 15:18:41

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_SYNGUILDARENADISPLACENEW_TYPE 56166

class PtcM2C_synguildarenadisplaceNew : public CProtocol
{
public:
    explicit PtcM2C_synguildarenadisplaceNew() : CProtocol(PTCM2C_SYNGUILDARENADISPLACENEW_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_synguildarenadisplaceNew()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::guildarenadisplace m_Data;
};

#endif