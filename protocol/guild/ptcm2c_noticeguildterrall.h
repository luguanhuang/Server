#ifndef __PTCM2C_NOTICEGUILDTERRALL_H__
#define __PTCM2C_NOTICEGUILDTERRALL_H__

// generate by ProtoGen at date: 2016/12/21 15:07:46

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_NOTICEGUILDTERRALL_TYPE 7704

class PtcM2C_NoticeGuildTerrall : public CProtocol
{
public:
    explicit PtcM2C_NoticeGuildTerrall() : CProtocol(PTCM2C_NOTICEGUILDTERRALL_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_NoticeGuildTerrall()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NoticeGuildTerrall m_Data;
};

#endif