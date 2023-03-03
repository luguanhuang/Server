#ifndef __PTCG2C_GMFJOINBATTLEREQ_H__
#define __PTCG2C_GMFJOINBATTLEREQ_H__

// generate by ProtoGen at date: 2016/7/18 10:26:23

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_GMFJOINBATTLEREQ_TYPE 19954

class PtcG2C_GmfJoinBattleReq : public CProtocol
{
public:
    explicit PtcG2C_GmfJoinBattleReq() : CProtocol(PTCG2C_GMFJOINBATTLEREQ_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_GmfJoinBattleReq()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GmfJoinBattleArg m_Data;
};

#endif