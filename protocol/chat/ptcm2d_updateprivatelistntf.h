#ifndef __PTCM2D_UPDATEPRIVATELISTNTF_H__
#define __PTCM2D_UPDATEPRIVATELISTNTF_H__

// generate by ProtoGen at date: 2016/10/1 16:51:28

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_UPDATEPRIVATELISTNTF_TYPE 13179

class PtcM2D_UpdatePrivateListNtf : public CProtocol
{
public:
    explicit PtcM2D_UpdatePrivateListNtf() : CProtocol(PTCM2D_UPDATEPRIVATELISTNTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_UpdatePrivateListNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::PrivateChatListToDB m_Data;
};

#endif