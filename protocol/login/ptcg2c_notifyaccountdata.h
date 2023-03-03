#ifndef __PTCG2C_NOTIFYACCOUNTDATA_H__
#define __PTCG2C_NOTIFYACCOUNTDATA_H__

// generate by ProtoGen at date: 2017/2/6 23:18:28

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2C_NOTIFYACCOUNTDATA_TYPE 29137

class PtcG2C_NotifyAccountData : public CProtocol
{
public:
    explicit PtcG2C_NotifyAccountData() : CProtocol(PTCG2C_NOTIFYACCOUNTDATA_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2C_NotifyAccountData()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LoadAccountData m_Data;
};

#endif