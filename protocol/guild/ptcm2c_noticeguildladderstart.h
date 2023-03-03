#ifndef __PTCM2C_NOTICEGUILDLADDERSTART_H__
#define __PTCM2C_NOTICEGUILDLADDERSTART_H__

// generate by ProtoGen at date: 2016/10/1 21:54:26

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2C_NOTICEGUILDLADDERSTART_TYPE 49782

class PtcM2C_NoticeGuildLadderStart : public CProtocol
{
public:
    explicit PtcM2C_NoticeGuildLadderStart() : CProtocol(PTCM2C_NOTICEGUILDLADDERSTART_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2C_NoticeGuildLadderStart()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::NoticeGuildLadderStart m_Data;
};

#endif