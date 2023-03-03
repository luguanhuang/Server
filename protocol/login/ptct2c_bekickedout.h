#ifndef __PTCT2C_BEKICKEDOUT_H__
#define __PTCT2C_BEKICKEDOUT_H__

// generate by ProtoGen at date: 2016/7/13 22:28:56

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCT2C_BEKICKEDOUT_TYPE 38944

class PtcT2C_BeKickedOut : public CProtocol
{
public:
    explicit PtcT2C_BeKickedOut() : CProtocol(PTCT2C_BEKICKEDOUT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcT2C_BeKickedOut()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::KickAccountData m_Data;
};

#endif