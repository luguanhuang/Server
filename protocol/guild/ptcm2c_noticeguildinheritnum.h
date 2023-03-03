#ifndef __PTCM2C_NOTICEGUILDINHERITNUM_H__
#define __PTCM2C_NOTICEGUILDINHERITNUM_H__

// generate by ProtoGen at date: 2016/11/19 10:47:43

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_NOTICEGUILDINHERITNUM_TYPE 51650

class PtcM2C_NoticeGuildInheritNum : public CProtocol
{
public:
    explicit PtcM2C_NoticeGuildInheritNum() : CProtocol(PTCM2C_NOTICEGUILDINHERITNUM_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_NoticeGuildInheritNum()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NoticeGuildInheritNum m_Data;
};

#endif