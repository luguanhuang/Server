#ifndef __PTCC2G_SYNDOINGGUILDINHERIT_H__
#define __PTCC2G_SYNDOINGGUILDINHERIT_H__

// generate by ProtoGen at date: 2016/11/18 21:07:38

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_SYNDOINGGUILDINHERIT_TYPE 51759

class PtcC2G_SynDoingGuildInherit : public CProtocol
{
public:
    explicit PtcC2G_SynDoingGuildInherit() : CProtocol(PTCC2G_SYNDOINGGUILDINHERIT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_SynDoingGuildInherit()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::SynDoingGuildInherit m_Data;
};

#endif