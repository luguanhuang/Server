#ifndef __PTCM2D_DBGROUPCHATDELGROUPNTF_H__
#define __PTCM2D_DBGROUPCHATDELGROUPNTF_H__

// generate by ProtoGen at date: 2017/7/28 15:42:09

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_DBGROUPCHATDELGROUPNTF_TYPE 49002

class PtcM2D_DBGroupChatDelGroupNtf : public CProtocol
{
public:
    explicit PtcM2D_DBGroupChatDelGroupNtf() : CProtocol(PTCM2D_DBGROUPCHATDELGROUPNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_DBGroupChatDelGroupNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DBGroupChatDelGroup m_Data;
};

#endif