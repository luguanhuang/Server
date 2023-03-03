#ifndef __PTCC2M_QUERYRESWARREQUET_H__
#define __PTCC2M_QUERYRESWARREQUET_H__

// generate by ProtoGen at date: 2016/11/11 21:12:55

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2M_QUERYRESWARREQUET_TYPE 53580

class PtcC2M_QueryResWarRequet : public CProtocol
{
public:
    explicit PtcC2M_QueryResWarRequet() : CProtocol(PTCC2M_QUERYRESWARREQUET_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2M_QueryResWarRequet()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::QueryResWarArg m_Data;
};

#endif