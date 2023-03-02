#ifndef _H_Parse_Line_H__
#define _H_Parse_Line_H__

#include "tinyxml2.h"


inline std::string ParseLineID(const char* pszFile)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError error = doc.LoadFile(pszFile);
	if (error != tinyxml2::XML_SUCCESS)
	{
		printf("Parser file %s failed, error code: %d", pszFile, error);
		return "";
	}

	tinyxml2::XMLElement* root = doc.RootElement();
	const char* pszLineID = root->Attribute("line");
	if(pszLineID == NULL)
	{
		printf("Parser file %s failed, no 'line' attribute", pszFile);
		return "";
	}

	return std::string(pszLineID);
}

inline bool ParseCross(const char* pszFile)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError error = doc.LoadFile(pszFile);
	if (error != tinyxml2::XML_SUCCESS)
	{
		printf("Parser file %s failed, error code: %d", pszFile, error);
		return false;
	}

	tinyxml2::XMLElement* root = doc.RootElement();
	const char* pszCross = root->Attribute("cross");
	if(pszCross == NULL)
	{
		return false;
	}
	return  (0 == strcmp("1", pszCross));
}


#endif
