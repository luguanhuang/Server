#ifndef __PTCG2C_PKROLEINFONTF_H__
#define __PTCG2C_PKROLEINFONTF_H__

// generate by ProtoGen at date: 2015/12/17 17:03:48

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_PKROLEINFONTF_TYPE 8937

class PtcG2C_PkRoleInfoNtf : public CProtocol
{
public:
    explicit PtcG2C_PkRoleInfoNtf() : CProtocol(PTCG2C_PKROLEINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_PkRoleInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PkRoleInfoNtf m_Data;
};

#endif