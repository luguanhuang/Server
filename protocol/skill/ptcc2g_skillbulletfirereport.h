#ifndef __PTCC2G_SKILLBULLETFIREREPORT_H__
#define __PTCC2G_SKILLBULLETFIREREPORT_H__

// generate by ProtoGen at date: 2015/12/16 14:54:31

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_SKILLBULLETFIREREPORT_TYPE 54744

class PtcC2G_SkillBulletFireReport : public CProtocol
{
public:
    explicit PtcC2G_SkillBulletFireReport() : CProtocol(PTCC2G_SKILLBULLETFIREREPORT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_SkillBulletFireReport()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::BulletUnitData m_Data;
};

#endif