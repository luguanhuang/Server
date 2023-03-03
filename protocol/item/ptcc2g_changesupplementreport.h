#ifndef __PTCC2G_CHANGESUPPLEMENTREPORT_H__
#define __PTCC2G_CHANGESUPPLEMENTREPORT_H__

// generate by ProtoGen at date: 2016/1/18 17:24:47

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCC2G_CHANGESUPPLEMENTREPORT_TYPE 42193

class PtcC2G_ChangeSupplementReport : public CProtocol
{
public:
    explicit PtcC2G_ChangeSupplementReport() : CProtocol(PTCC2G_CHANGESUPPLEMENTREPORT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcC2G_ChangeSupplementReport()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::ChangeSupplementReport m_Data;
};

#endif