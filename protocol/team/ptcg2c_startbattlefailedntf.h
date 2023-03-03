#ifndef __PTCG2C_STARTBATTLEFAILEDNTF_H__
#define __PTCG2C_STARTBATTLEFAILEDNTF_H__

// generate by ProtoGen at date: 2016/7/8 21:51:09

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_STARTBATTLEFAILEDNTF_TYPE 54098

class PtcG2C_StartBattleFailedNtf : public CProtocol
{
public:
    explicit PtcG2C_StartBattleFailedNtf() : CProtocol(PTCG2C_STARTBATTLEFAILEDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_StartBattleFailedNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::StartBattleFailedRes m_Data;
};

#endif