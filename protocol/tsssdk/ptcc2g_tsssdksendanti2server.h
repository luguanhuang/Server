#ifndef __PTCC2G_TSSSDKSENDANTI2SERVER_H__
#define __PTCC2G_TSSSDKSENDANTI2SERVER_H__

// generate by ProtoGen at date: 2016/6/22 13:23:31

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_TSSSDKSENDANTI2SERVER_TYPE 62305

class PtcC2G_TssSdkSendAnti2Server : public CProtocol
{
public:
    explicit PtcC2G_TssSdkSendAnti2Server() : CProtocol(PTCC2G_TSSSDKSENDANTI2SERVER_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_TssSdkSendAnti2Server()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TssSdkAntiData m_Data;
};

#endif