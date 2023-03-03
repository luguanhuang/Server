#ifndef __PTCS2M_TEAMREQUESTPLATFREINDCONFIRM_H__
#define __PTCS2M_TEAMREQUESTPLATFREINDCONFIRM_H__

// generate by ProtoGen at date: 2017/5/3 15:42:44

#include "protocol.h"
#include "pb/project.pb.h"

#define PTCS2M_TEAMREQUESTPLATFREINDCONFIRM_TYPE 45804

class PtcS2M_TeamRequestPlatFreindConfirm : public CProtocol
{
public:
    explicit PtcS2M_TeamRequestPlatFreindConfirm() : CProtocol(PTCS2M_TEAMREQUESTPLATFREINDCONFIRM_TYPE)
    {
		m_message = &m_Data;
    }

    virtual ~PtcS2M_TeamRequestPlatFreindConfirm()
    {
    }

    virtual void Process(UINT32 dwConnID);

public:
	KKSG::TeamRequestPlatFreindConfirmData m_Data;
};

#endif