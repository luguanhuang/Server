#ifndef __PTCM2C_STARTBATTLEFAILEDM2CNTF_H__
#define __PTCM2C_STARTBATTLEFAILEDM2CNTF_H__

// generate by ProtoGen at date: 2017/8/7 17:07:48

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_STARTBATTLEFAILEDM2CNTF_TYPE 20444

class PtcM2C_StartBattleFailedM2CNtf : public CProtocol
{
public:
    explicit PtcM2C_StartBattleFailedM2CNtf() : CProtocol(PTCM2C_STARTBATTLEFAILEDM2CNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_StartBattleFailedM2CNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::StartBattleFailedRes m_Data;
};

#endif