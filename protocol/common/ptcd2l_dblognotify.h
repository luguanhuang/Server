#ifndef __PTCD2L_DBLOGNOTIFY_H__
#define __PTCD2L_DBLOGNOTIFY_H__

// generate by ProtoGen at date: 2015/10/20 15:22:14

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCD2L_DBLOGNOTIFY_TYPE 62195

class PtcD2L_DBLogNotify : public CProtocol
{
public:
    explicit PtcD2L_DBLogNotify() : CProtocol(PTCD2L_DBLOGNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcD2L_DBLogNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LogMsg m_Data;
};

#endif