#ifndef __PTCM2C_NOTICEGUILDTERREND_H__
#define __PTCM2C_NOTICEGUILDTERREND_H__

// generate by ProtoGen at date: 2017/2/8 20:41:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_NOTICEGUILDTERREND_TYPE 2103

class PtcM2C_NoticeGuildTerrEnd : public CProtocol
{
public:
    explicit PtcM2C_NoticeGuildTerrEnd() : CProtocol(PTCM2C_NOTICEGUILDTERREND_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_NoticeGuildTerrEnd()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NoticeGuildTerrEnd m_Data;
};

#endif