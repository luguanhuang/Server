#ifndef __PTCG2C_SELECTROLENTF_H__
#define __PTCG2C_SELECTROLENTF_H__

// generate by ProtoGen at date: 2016/7/10 13:44:02

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_SELECTROLENTF_TYPE 19493

class PtcG2C_SelectRoleNtf : public CProtocol
{
public:
    explicit PtcG2C_SelectRoleNtf() : CProtocol(PTCG2C_SELECTROLENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_SelectRoleNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SelectRoleNtfData m_Data;
};

#endif