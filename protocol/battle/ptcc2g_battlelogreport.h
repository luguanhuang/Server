#ifndef __PTCC2G_BATTLELOGREPORT_H__
#define __PTCC2G_BATTLELOGREPORT_H__

// generate by ProtoGen at date: 2016/11/2 20:50:37

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_BATTLELOGREPORT_TYPE 10382

class PtcC2G_BattleLogReport : public CProtocol
{
public:
    explicit PtcC2G_BattleLogReport() : CProtocol(PTCC2G_BATTLELOGREPORT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_BattleLogReport()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BattleLogReport m_Data;
};

#endif