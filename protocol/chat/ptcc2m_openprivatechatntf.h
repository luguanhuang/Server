#ifndef __PTCC2M_OPENPRIVATECHATNTF_H__
#define __PTCC2M_OPENPRIVATECHATNTF_H__

// generate by ProtoGen at date: 2017/6/6 18:14:48

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_OPENPRIVATECHATNTF_TYPE 23206

class PtcC2M_OpenPrivateChatNtf : public CProtocol
{
public:
    explicit PtcC2M_OpenPrivateChatNtf() : CProtocol(PTCC2M_OPENPRIVATECHATNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_OpenPrivateChatNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::OpenPrivateChat m_Data;
};

#endif