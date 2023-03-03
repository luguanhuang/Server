#ifndef __PTCG2C_TOWERSCENEINFONTF_H__
#define __PTCG2C_TOWERSCENEINFONTF_H__

// generate by ProtoGen at date: 2016/6/2 15:51:51

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_TOWERSCENEINFONTF_TYPE 14948

class PtcG2C_TowerSceneInfoNtf : public CProtocol
{
public:
    explicit PtcG2C_TowerSceneInfoNtf() : CProtocol(PTCG2C_TOWERSCENEINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_TowerSceneInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TowerSceneInfoData m_Data;
};

#endif