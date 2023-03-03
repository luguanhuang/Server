#ifndef __PTCM2D_DBGROUPCHATSAVEGROUPNTF_H__
#define __PTCM2D_DBGROUPCHATSAVEGROUPNTF_H__

// generate by ProtoGen at date: 2017/7/28 15:41:26

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_DBGROUPCHATSAVEGROUPNTF_TYPE 59908

class PtcM2D_DBGroupChatSaveGroupNtf : public CProtocol
{
public:
    explicit PtcM2D_DBGroupChatSaveGroupNtf() : CProtocol(PTCM2D_DBGROUPCHATSAVEGROUPNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_DBGroupChatSaveGroupNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::DBGroupChatSaveGroup m_Data;
};

#endif