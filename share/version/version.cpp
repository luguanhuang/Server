#include "pch.h"
#include "version.h"


void Version::SetEmpty()
{
	//if (ios.empty())
	//{
	//	ios = "null";
	//}
	//if (android.empty())
	//{
	//	android = "null";
	//}
	//if (pc.empty())
	//{
	//	pc = "0.0.0";
	//}
}

void Version::Clear()
{
	type = Version_None;
	ios.clear();
	android.clear();
	pc.clear();
}

std::string Version::GetClientVersion(const std::string& _ios, const std::string& _android, const std::string& _pc)
{
	switch (type)
	{
	case Version_None:
		{
			return "";
		}
	case Version_iOS:
		{
			return _ios;
		}
	case Version_Android:
		{
			return _android;
		}
	case Version_PC:
		{
			return _pc;
		}
	case Version_All:
		{
			//if (!ios.empty()) return *ios.begin();
			//if (!android.empty()) return *android.begin();
			//if (!pc.empty()) return *pc.begin();
			if (!_ios.empty()) return _ios;
			if (!_android.empty()) return _android;
			if (!_pc.empty()) return _pc;
		}
	}
	return "";
}

std::string Version::Check(const std::string& user, const std::unordered_set<std::string>& version)
{
	auto find = version.find(user);
	if (find != version.end())
	{
		return "";
	}
	if (version.empty())
	{
		return "null";
	}
	return *version.begin();
	//return user == version ? "" : version; 
}

const Version& Version::operator=(const Version& ver)
{
	type = ver.type;
	ios = ver.ios;
	android = ver.android;
	pc = ver.pc;

	return *this;
}

std::string Version::CheckVersion(const std::string& _ios, const std::string& _android, const std::string& _pc)
{
	SSInfo<<"type:"<<type<<" ios:"<<_ios<<" android:"<<_android<<" _pc:"<<_pc<<END;
	switch (type)
	{
	case Version_None:
		{
			return "";
		}
	case Version_iOS:
		{
			return Check(_ios, ios);
		}
	case Version_Android:
		{
			return Check(_android, android);
		}
	case Version_PC:
		{
			return Check(_pc, pc);
		}
	case Version_All:
		{
			if (!_ios.empty()) return Check(_ios, ios);
			if (!_android.empty()) return Check(_android, android);
			if (!_pc.empty()) return Check(_pc, pc);
			return "null";	
		}
	}
	return "null";
}
