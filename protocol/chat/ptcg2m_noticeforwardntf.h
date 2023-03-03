#ifndef __PTCG2M_NOTICEFORWARDNTF_H__
#define __PTCG2M_NOTICEFORWARDNTF_H__

// generate by ProtoGen at date: 2016/7/26 22:46:49

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCG2M_NOTICEFORWARDNTF_TYPE 25246

class PtcG2M_NoticeForwardNtf : public CProtocol
{
public:
    explicit PtcG2M_NoticeForwardNtf() : CProtocol(PTCG2M_NOTICEFORWARDNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcG2M_NoticeForwardNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NoticeForwardNtf m_Data;
};

#endif