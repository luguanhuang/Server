#ifndef __PTCP2I_UPDATEPLATNOTICELOGINNTF_H__
#define __PTCP2I_UPDATEPLATNOTICELOGINNTF_H__

// generate by ProtoGen at date: 2016/11/19 21:02:09

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCP2I_UPDATEPLATNOTICELOGINNTF_TYPE 49664

class PtcP2I_UpdatePlatNoticeLoginNtf : public CProtocol
{
public:
    explicit PtcP2I_UpdatePlatNoticeLoginNtf() : CProtocol(PTCP2I_UPDATEPLATNOTICELOGINNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcP2I_UpdatePlatNoticeLoginNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PlatNotice m_Data;
};

#endif