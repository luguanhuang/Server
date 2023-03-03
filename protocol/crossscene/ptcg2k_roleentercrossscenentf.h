#ifndef __PTCG2K_ROLEENTERCROSSSCENENTF_H__
#define __PTCG2K_ROLEENTERCROSSSCENENTF_H__

// generate by ProtoGen at date: 2016/11/1 11:20:16

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2K_ROLEENTERCROSSSCENENTF_TYPE 23973

class PtcG2K_RoleEnterCrossSceneNtf : public CProtocol
{
public:
    explicit PtcG2K_RoleEnterCrossSceneNtf() : CProtocol(PTCG2K_ROLEENTERCROSSSCENENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2K_RoleEnterCrossSceneNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RoleEnterCrossSceneNtf m_Data;
};

#endif