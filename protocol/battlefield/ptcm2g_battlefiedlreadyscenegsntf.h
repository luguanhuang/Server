#ifndef __PTCM2G_BATTLEFIEDLREADYSCENEGSNTF_H__
#define __PTCM2G_BATTLEFIEDLREADYSCENEGSNTF_H__

// generate by ProtoGen at date: 2017/9/6 13:31:30

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2G_BATTLEFIEDLREADYSCENEGSNTF_TYPE 15032

class PtcM2G_BattleFiedlReadySceneGsNtf : public CProtocol
{
public:
    explicit PtcM2G_BattleFiedlReadySceneGsNtf() : CProtocol(PTCM2G_BATTLEFIEDLREADYSCENEGSNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2G_BattleFiedlReadySceneGsNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BattleFieldReadyScene m_Data;
};

#endif