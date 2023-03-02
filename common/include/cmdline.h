#ifndef __CMDLINE_H__
#define __CMDLINE_H__

typedef std::vector<std::string> CmdLineArgs;

typedef std::function<bool (const CmdLineArgs &args, std::string &outputMessage)> CMDLineFun;

class CmdLine
{
public:
	static void RegistCmd(const char *cmd, CMDLineFun f);
	static void Setup();
	static void Run();
	static bool RunCmd(const std::string &cmd, std::string &outputMessage);
	static void Stop();
};

#endif