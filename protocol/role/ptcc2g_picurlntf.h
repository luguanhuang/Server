#ifndef __PTCC2G_PICURLNTF_H__
#define __PTCC2G_PICURLNTF_H__

// generate by ProtoGen at date: 2016/12/10 14:15:13

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_PICURLNTF_TYPE 30863

class PtcC2G_PicUrlNtf : public CProtocol
{
public:
    explicit PtcC2G_PicUrlNtf() : CProtocol(PTCC2G_PICURLNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_PicUrlNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PicUrlInfo m_Data;
};

#endif