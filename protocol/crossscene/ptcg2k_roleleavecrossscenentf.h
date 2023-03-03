#ifndef __PTCG2K_ROLELEAVECROSSSCENENTF_H__
#define __PTCG2K_ROLELEAVECROSSSCENENTF_H__

// generate by ProtoGen at date: 2017/8/3 11:51:44

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2K_ROLELEAVECROSSSCENENTF_TYPE 14595

class PtcG2K_RoleLeaveCrossSceneNtf : public CProtocol
{
public:
    explicit PtcG2K_RoleLeaveCrossSceneNtf() : CProtocol(PTCG2K_ROLELEAVECROSSSCENENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2K_RoleLeaveCrossSceneNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::RoleLeaveCrossSceneNtf m_Data;
};

#endif