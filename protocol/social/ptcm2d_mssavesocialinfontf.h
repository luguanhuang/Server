#ifndef __PTCM2D_MSSAVESOCIALINFONTF_H__
#define __PTCM2D_MSSAVESOCIALINFONTF_H__

// generate by ProtoGen at date: 2016/8/17 17:51:57

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_MSSAVESOCIALINFONTF_TYPE 20325

class PtcM2D_MSSaveSocialInfoNtf : public CProtocol
{
public:
    explicit PtcM2D_MSSaveSocialInfoNtf() : CProtocol(PTCM2D_MSSAVESOCIALINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_MSSaveSocialInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SaveSocialInfoNtf m_Data;
};

#endif