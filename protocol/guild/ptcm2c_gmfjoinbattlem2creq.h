#ifndef __PTCM2C_GMFJOINBATTLEM2CREQ_H__
#define __PTCM2C_GMFJOINBATTLEM2CREQ_H__

// generate by ProtoGen at date: 2016/9/20 17:04:26

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_GMFJOINBATTLEM2CREQ_TYPE 63969

class PtcM2C_GmfJoinBattleM2CReq : public CProtocol
{
public:
    explicit PtcM2C_GmfJoinBattleM2CReq() : CProtocol(PTCM2C_GMFJOINBATTLEM2CREQ_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_GmfJoinBattleM2CReq()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::GmfJoinBattleArg m_Data;
};

#endif