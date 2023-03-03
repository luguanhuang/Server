#ifndef __PTCM2C_NOTICEGUILDINHERITEXP_H__
#define __PTCM2C_NOTICEGUILDINHERITEXP_H__

// generate by ProtoGen at date: 2016/11/17 19:42:25

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_NOTICEGUILDINHERITEXP_TYPE 52440

class PtcM2C_NoticeGuildInheritExp : public CProtocol
{
public:
    explicit PtcM2C_NoticeGuildInheritExp() : CProtocol(PTCM2C_NOTICEGUILDINHERITEXP_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_NoticeGuildInheritExp()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NoticeGuildInheritExp m_Data;
};

#endif