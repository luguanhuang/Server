#ifndef __PTCG2C_INVFIGHTBEFENTERSCENENTF_H__
#define __PTCG2C_INVFIGHTBEFENTERSCENENTF_H__

// generate by ProtoGen at date: 2016/12/15 20:06:36

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_INVFIGHTBEFENTERSCENENTF_TYPE 7135

class PtcG2C_InvFightBefEnterSceneNtf : public CProtocol
{
public:
    explicit PtcG2C_InvFightBefEnterSceneNtf() : CProtocol(PTCG2C_INVFIGHTBEFENTERSCENENTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_InvFightBefEnterSceneNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::InvFightBefESpara m_Data;
};

#endif