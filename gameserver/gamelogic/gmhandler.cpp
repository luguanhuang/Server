#include "pch.h"
#include "gmhandler.h"
#include "pb/project.pb.h"
#include "unit/rolemanager.h"


INSTANCE_SINGLETON(GMHandler)

GMHandler::GMHandler()
{

}

GMHandler::~GMHandler()
{

}

void GMHandler::Uninit()
{

}

void GMHandler::ExecuteCmd(const KKSG::GMCmdArg &roArg, KKSG::GMCmdRes &roRes)
{
	auto it = m_Handlers.find(roArg.cmd());
	if (it != m_Handlers.end())
	{
		LogInfo("run gm command: %s", roArg.cmd().c_str());
		roRes.set_result(true);
		it->second(roArg, roRes);
	}
	else
	{
		LogWarn("can't process gm command : %s", roArg.cmd().c_str());
		roRes.set_result(false);
		roRes.set_outputmessage("unknow command : " + roArg.cmd());
	}
}

static void CmdGetRoleCount(const KKSG::GMCmdArg &roArg, KKSG::GMCmdRes &roRes)
{
	std::stringstream ss;
	ss << "role count = " << RoleManager::Instance()->GetRoleCount();
	roRes.set_outputmessage(ss.str());
}

static void CmdList(const KKSG::GMCmdArg &roArg, KKSG::GMCmdRes &roRes)
{
	std::stringstream ss;
	std::vector<std::string> Cmds = GMHandler::Instance()->Cmds();
	for (auto it = Cmds.begin(); it != Cmds.end(); ++it)
	{
		ss << *it << "<br/>";
	}
	roRes.set_outputmessage(ss.str());
}

bool GMHandler::Init()
{
	m_Handlers["rolecount"] = CmdGetRoleCount;
	m_Handlers["cmdlist"] = CmdList;
	return true;
}

std::vector<std::string> GMHandler::Cmds()
{
	std::vector<std::string> cmds;
	for (auto it = m_Handlers.begin(); it != m_Handlers.end(); ++it)
	{
		cmds.push_back(it->first);
	}
	return cmds;
}
