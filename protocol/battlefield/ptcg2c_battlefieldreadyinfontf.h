#ifndef __PTCG2C_BATTLEFIELDREADYINFONTF_H__
#define __PTCG2C_BATTLEFIELDREADYINFONTF_H__

// generate by ProtoGen at date: 2017/9/4 15:41:40

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_BATTLEFIELDREADYINFONTF_TYPE 40392

class PtcG2C_BattleFieldReadyInfoNtf : public CProtocol
{
public:
    explicit PtcG2C_BattleFieldReadyInfoNtf() : CProtocol(PTCG2C_BATTLEFIELDREADYINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_BattleFieldReadyInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BattleFieldReadyInfo m_Data;
};

#endif