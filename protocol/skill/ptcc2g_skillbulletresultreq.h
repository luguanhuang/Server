#ifndef __PTCC2G_SKILLBULLETRESULTREQ_H__
#define __PTCC2G_SKILLBULLETRESULTREQ_H__

// generate by ProtoGen at date: 2015/12/16 15:27:28

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_SKILLBULLETRESULTREQ_TYPE 15929

class PtcC2G_SkillBulletResultReq : public CProtocol
{
public:
    explicit PtcC2G_SkillBulletResultReq() : CProtocol(PTCC2G_SKILLBULLETRESULTREQ_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_SkillBulletResultReq()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SkillBulletResultReqUnit m_Data;
};

#endif