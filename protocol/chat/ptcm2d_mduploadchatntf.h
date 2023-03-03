#ifndef __PTCM2D_MDUPLOADCHATNTF_H__
#define __PTCM2D_MDUPLOADCHATNTF_H__

// generate by ProtoGen at date: 2016/9/28 22:27:51

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_MDUPLOADCHATNTF_TYPE 16216

class PtcM2D_MDUpLoadChatNtf : public CProtocol
{
public:
    explicit PtcM2D_MDUpLoadChatNtf() : CProtocol(PTCM2D_MDUPLOADCHATNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_MDUpLoadChatNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UploadChatInfoNtf m_Data;
};

#endif