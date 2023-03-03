#ifndef __PTCC2G_ADDLEVELSCRIPTEXTSTRING_H__
#define __PTCC2G_ADDLEVELSCRIPTEXTSTRING_H__

// generate by ProtoGen at date: 2015/12/23 11:03:05

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_ADDLEVELSCRIPTEXTSTRING_TYPE 34579

class PtcC2G_AddLevelScriptExtString : public CProtocol
{
public:
    explicit PtcC2G_AddLevelScriptExtString() : CProtocol(PTCC2G_ADDLEVELSCRIPTEXTSTRING_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_AddLevelScriptExtString()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::AddLevelScriptExtString m_Data;
};

#endif