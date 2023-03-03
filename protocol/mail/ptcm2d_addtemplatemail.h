#ifndef __PTCM2D_ADDTEMPLATEMAIL_H__
#define __PTCM2D_ADDTEMPLATEMAIL_H__

// generate by ProtoGen at date: 2016/8/22 12:09:35

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2D_ADDTEMPLATEMAIL_TYPE 36445

class PtcM2D_AddTemplateMail : public CProtocol
{
public:
    explicit PtcM2D_AddTemplateMail() : CProtocol(PTCM2D_ADDTEMPLATEMAIL_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2D_AddTemplateMail()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AddTemplateMailData m_Data;
};

#endif