#ifndef __GMHANDLER_H__
#define __GMHANDLER_H__

namespace KKSG
{
	class GMCmdArg;
	class GMCmdRes;
}

typedef void (*GMHandlerFunction)(const KKSG::GMCmdArg &roArg, KKSG::GMCmdRes &roRes);

class GMHandler
{
	GMHandler();
	~GMHandler();
	DECLARE_SINGLETON(GMHandler)

public:

	bool Init();
	void Uninit();

	void ExecuteCmd(const KKSG::GMCmdArg &roArg, KKSG::GMCmdRes &roRes);
	std::vector<std::string> Cmds();

private:
	std::map<std::string, GMHandlerFunction> m_Handlers;
};

#endif // __GMHANDLER_H__