#ifndef __PTCC2G_SKILLRESULTREQ_H__
#define __PTCC2G_SKILLRESULTREQ_H__

// generate by ProtoGen at date: 2015/12/16 11:08:35

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_SKILLRESULTREQ_TYPE 41958

class PtcC2G_SkillResultReq : public CProtocol
{
public:
    explicit PtcC2G_SkillResultReq() : CProtocol(PTCC2G_SKILLRESULTREQ_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_SkillResultReq()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SkillResultReqUnit m_Data;
};

#endif