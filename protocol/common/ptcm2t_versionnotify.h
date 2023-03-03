#ifndef __PTCM2T_VERSIONNOTIFY_H__
#define __PTCM2T_VERSIONNOTIFY_H__

// generate by ProtoGen at date: 2016/7/27 21:24:57

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCM2T_VERSIONNOTIFY_TYPE 8566

class PtcM2T_VersionNotify : public CProtocol
{
public:
    explicit PtcM2T_VersionNotify() : CProtocol(PTCM2T_VERSIONNOTIFY_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcM2T_VersionNotify()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SVersion m_Data;
};

#endif