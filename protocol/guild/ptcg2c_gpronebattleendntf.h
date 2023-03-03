#ifndef __PTCG2C_GPRONEBATTLEENDNTF_H__
#define __PTCG2C_GPRONEBATTLEENDNTF_H__

// generate by ProtoGen at date: 2016/10/29 11:04:22

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GPRONEBATTLEENDNTF_TYPE 39421

class PtcG2C_GprOneBattleEndNtf : public CProtocol
{
public:
    explicit PtcG2C_GprOneBattleEndNtf() : CProtocol(PTCG2C_GPRONEBATTLEENDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GprOneBattleEndNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GprOneBattleEnd m_Data;
};

#endif