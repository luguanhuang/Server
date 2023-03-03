#ifndef __PTCG2C_WATCHBATTLEINFONTF_H__
#define __PTCG2C_WATCHBATTLEINFONTF_H__

// generate by ProtoGen at date: 2016/7/13 20:17:39

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_WATCHBATTLEINFONTF_TYPE 23415

class PtcG2C_WatchBattleInfoNtf : public CProtocol
{
public:
    explicit PtcG2C_WatchBattleInfoNtf() : CProtocol(PTCG2C_WATCHBATTLEINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_WatchBattleInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::WatchBattleData m_Data;
};

#endif