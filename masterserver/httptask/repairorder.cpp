#include "pch.h"
#include "repairorder.h"
#include "config.h"

RepairOrderTask::RepairOrderTask()
{
	m_strUrl = MSConfig::Instance()->GetMsdkUrl();
}

RepairOrderTask::~RepairOrderTask()
{

}

const std::string RepairOrderTask::GetUrl()
{
	std::ostringstream ss;
	ss << m_strUrl << "/cron.php";
	return ss.str();
}

void RepairOrderTask::OnResult(const std::string &response)
{

}

void RepairOrderTask::OnFailed()
{

}
