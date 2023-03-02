#ifndef __TABLECMD_H__
#define __TABLECMD_H__

#include "cmdline.h"

class TableCmd
{
	TableCmd();
	~TableCmd();
	DECLARE_SINGLETON(TableCmd)

public:

	bool Init();
	void Uninit();

	static bool CmdLineTable(const CmdLineArgs &args, std::string &outputMessage);

private:


};

#endif // __TABLECMD_H__