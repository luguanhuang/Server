#ifndef __PTCG2M_ROLECHANGESCENENTF_H__
#define __PTCG2M_ROLECHANGESCENENTF_H__

// generate by ProtoGen at date: 2016/11/4 16:15:51

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_ROLECHANGESCENENTF_TYPE 15275

class PtcG2M_RoleChangeSceneNtf : public CProtocol
{
public:
    explicit PtcG2M_RoleChangeSceneNtf() : CProtocol(PTCG2M_ROLECHANGESCENENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_RoleChangeSceneNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RoleChangeSceneNtfData m_Data;
};

#endif