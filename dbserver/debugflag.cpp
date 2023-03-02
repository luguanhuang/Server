#include "pch.h"
#include "debugflag.h"
#include "cmdline.h"

DebugFlag gDebugFlag;

DebugFlag::DebugFlag()
{
	ShowSetField = false;
	ShowRunSQL = false;
	ShowFlow = false;
}

static bool TestFlag(const std::string &v)
{
	return v == "on" || v == "1" || v == "ON" || v == "true" || v == "TRUE";
}


bool CmdLineSetDebugFlag(const CmdLineArgs &args, std::string &outputMessage)
{
	const char *cmdName = args[0].c_str();
	UINT32 Size = args.size();
	int i = 0;

	if (Size == 1)
	{
		LogInfo("\n%s list\n%s set flagName on/off", cmdName, cmdName);
		return true;
	}

#define SET_DEBUG_FLAG(name) if (args[2] == #name || ++i == ID) { gDebugFlag.name = TestFlag(args[3]); LogInfo("Set debugFlag %s = [%s]", #name, (gDebugFlag.name ? "ON " : "OFF")); }
#define LIST_DEBUG_FLAG(name)    ss << ++i << ". [" << (gDebugFlag.name ? "ON " : "OFF") << "]   " << #name << "\n" 

	if (args[1] == "list")
	{
		std::stringstream ss;
		LIST_DEBUG_FLAG(ShowSetField);
		LIST_DEBUG_FLAG(ShowRunSQL);
		LIST_DEBUG_FLAG(ShowFlow);

		LogInfo("\n%s", ss.str().c_str());
		return true;
	}

	if (args[1] == "set")
	{
		if (Size != 4)
		{
			LogInfo("\n%s list\n%s show flagName\n%s set flagName on/off", cmdName, cmdName, cmdName);
			return true;
		}

		int ID = -1;
		if (isdigit(args[2][0]))
		{
			ID = atoi(args[2].c_str());
		}

		SET_DEBUG_FLAG(ShowSetField);
		SET_DEBUG_FLAG(ShowRunSQL);
		SET_DEBUG_FLAG(ShowFlow);

		return true;
	}

	return true;
}