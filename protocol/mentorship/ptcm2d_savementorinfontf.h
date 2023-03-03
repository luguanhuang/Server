#ifndef __PTCM2D_SAVEMENTORINFONTF_H__
#define __PTCM2D_SAVEMENTORINFONTF_H__

// generate by ProtoGen at date: 2016/12/17 21:40:04

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_SAVEMENTORINFONTF_TYPE 58217

class PtcM2D_SaveMentorInfoNtf : public CProtocol
{
public:
    explicit PtcM2D_SaveMentorInfoNtf() : CProtocol(PTCM2D_SAVEMENTORINFONTF_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_SaveMentorInfoNtf()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SaveMentor2DbData m_Data;
};

#endif