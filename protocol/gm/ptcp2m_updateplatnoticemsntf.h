#ifndef __PTCP2M_UPDATEPLATNOTICEMSNTF_H__
#define __PTCP2M_UPDATEPLATNOTICEMSNTF_H__

// generate by ProtoGen at date: 2016/11/19 21:02:06

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCP2M_UPDATEPLATNOTICEMSNTF_TYPE 53792

class PtcP2M_UpdatePlatNoticeMsNtf : public CProtocol
{
public:
    explicit PtcP2M_UpdatePlatNoticeMsNtf() : CProtocol(PTCP2M_UPDATEPLATNOTICEMSNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcP2M_UpdatePlatNoticeMsNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PlatNotice m_Data;
};

#endif