#ifndef __PTCG2C_ONTRANSFERWALL_H__
#define __PTCG2C_ONTRANSFERWALL_H__

// generate by ProtoGen at date: 2016/7/30 13:18:54

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_ONTRANSFERWALL_TYPE 37585

class PtcG2C_OnTransferWall : public CProtocol
{
public:
    explicit PtcG2C_OnTransferWall() : CProtocol(PTCG2C_ONTRANSFERWALL_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_OnTransferWall()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NotifyTransferWall m_Data;
};

#endif