#ifndef __PTCC2G_UPDATETUTORIAL_H__
#define __PTCC2G_UPDATETUTORIAL_H__

// generate by ProtoGen at date: 2015/2/2 14:42:56

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_UPDATETUTORIAL_TYPE 31917

class PtcC2G_UpdateTutorial : public CProtocol
{
public:
    explicit PtcC2G_UpdateTutorial() : CProtocol(PTCC2G_UPDATETUTORIAL_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_UpdateTutorial()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TutorialInfo m_Data;
};

#endif