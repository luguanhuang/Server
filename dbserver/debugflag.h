#ifndef __DEBUGFLAG_H__
#define __DEBUGFLAG_H__

#include "cmdline.h"

struct DebugFlag
{
	bool ShowSetField;
	bool ShowRunSQL;
	bool ShowFlow;

	DebugFlag();
};

extern DebugFlag gDebugFlag;

bool CmdLineSetDebugFlag(const CmdLineArgs &args, std::string &outputMessage);

#endif // __DEBUGFLAG_H__