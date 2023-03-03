#ifndef __PTCC2M_LOADOFFLINECHATNTF_H__
#define __PTCC2M_LOADOFFLINECHATNTF_H__

// generate by ProtoGen at date: 2016/8/3 2:06:32

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_LOADOFFLINECHATNTF_TYPE 26622

class PtcC2M_LoadOfflineChatNtf : public CProtocol
{
public:
    explicit PtcC2M_LoadOfflineChatNtf() : CProtocol(PTCC2M_LOADOFFLINECHATNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_LoadOfflineChatNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LoadOffLineChatNtf m_Data;
};

#endif