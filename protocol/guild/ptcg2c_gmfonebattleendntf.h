#ifndef __PTCG2C_GMFONEBATTLEENDNTF_H__
#define __PTCG2C_GMFONEBATTLEENDNTF_H__

// generate by ProtoGen at date: 2016/7/18 10:28:23

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GMFONEBATTLEENDNTF_TYPE 61740

class PtcG2C_GmfOneBattleEndNtf : public CProtocol
{
public:
    explicit PtcG2C_GmfOneBattleEndNtf() : CProtocol(PTCG2C_GMFONEBATTLEENDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GmfOneBattleEndNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GmfOneBattleEnd m_Data;
};

#endif