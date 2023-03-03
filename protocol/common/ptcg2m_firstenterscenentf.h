#ifndef __PTCG2M_FIRSTENTERSCENENTF_H__
#define __PTCG2M_FIRSTENTERSCENENTF_H__

// generate by ProtoGen at date: 2016/8/4 23:18:29

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_FIRSTENTERSCENENTF_TYPE 45896

class PtcG2M_FirstEnterSceneNtf : public CProtocol
{
public:
    explicit PtcG2M_FirstEnterSceneNtf() : CProtocol(PTCG2M_FIRSTENTERSCENENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_FirstEnterSceneNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::FirstEnterSceneNtf m_Data;
};

#endif