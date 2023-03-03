#ifndef __PTCM2K_BATTLEFIELDEVENTREQ_H__
#define __PTCM2K_BATTLEFIELDEVENTREQ_H__

// generate by ProtoGen at date: 2017/8/29 20:20:21

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2K_BATTLEFIELDEVENTREQ_TYPE 1769

class PtcM2K_BattleFieldEventReq : public CProtocol
{
public:
    explicit PtcM2K_BattleFieldEventReq() : CProtocol(PTCM2K_BATTLEFIELDEVENTREQ_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2K_BattleFieldEventReq()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BattleFieldEvent m_Data;
};

#endif