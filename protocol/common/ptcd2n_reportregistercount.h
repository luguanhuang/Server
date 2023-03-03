#ifndef __PTCD2N_REPORTREGISTERCOUNT_H__
#define __PTCD2N_REPORTREGISTERCOUNT_H__

// generate by ProtoGen at date: 2017/3/7 17:25:53

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCD2N_REPORTREGISTERCOUNT_TYPE 49223

class PtcD2N_ReportRegisterCount : public CProtocol
{
public:
    explicit PtcD2N_ReportRegisterCount() : CProtocol(PTCD2N_REPORTREGISTERCOUNT_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcD2N_ReportRegisterCount()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::UpdateRegisterCount2Ns m_Data;
};

#endif