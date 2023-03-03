#ifndef __PTCK2M_LEAGUECROSSELINTFMS_H__
#define __PTCK2M_LEAGUECROSSELINTFMS_H__

// generate by ProtoGen at date: 2017/2/6 10:30:26

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCK2M_LEAGUECROSSELINTFMS_TYPE 10773

class PtcK2M_LeagueCrossEliNtfMs : public CProtocol
{
public:
    explicit PtcK2M_LeagueCrossEliNtfMs() : CProtocol(PTCK2M_LEAGUECROSSELINTFMS_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcK2M_LeagueCrossEliNtfMs()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::LeagueCrossEliNtfMs m_Data;
};

#endif