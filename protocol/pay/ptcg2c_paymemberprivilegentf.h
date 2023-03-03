#ifndef __PTCG2C_PAYMEMBERPRIVILEGENTF_H__
#define __PTCG2C_PAYMEMBERPRIVILEGENTF_H__

// generate by ProtoGen at date: 2016/11/5 17:11:03

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_PAYMEMBERPRIVILEGENTF_TYPE 33306

class PtcG2C_PayMemberPrivilegeNtf : public CProtocol
{
public:
    explicit PtcG2C_PayMemberPrivilegeNtf() : CProtocol(PTCG2C_PAYMEMBERPRIVILEGENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_PayMemberPrivilegeNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PayMemberPrivilege m_Data;
};

#endif