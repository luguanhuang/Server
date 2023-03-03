#ifndef __PTCK2M_UPDATEWORLDMATCHNUM_H__
#define __PTCK2M_UPDATEWORLDMATCHNUM_H__

// generate by ProtoGen at date: 2016/11/15 19:09:21

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2M_UPDATEWORLDMATCHNUM_TYPE 64286

class PtcK2M_UpdateWorldMatchNum : public CProtocol
{
public:
    explicit PtcK2M_UpdateWorldMatchNum() : CProtocol(PTCK2M_UPDATEWORLDMATCHNUM_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2M_UpdateWorldMatchNum()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateWorldMatchNum m_Data;
};

#endif