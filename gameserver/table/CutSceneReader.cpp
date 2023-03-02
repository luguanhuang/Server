#include "pch.h"
#include "CutSceneReader.h"
#include <tinyxml2.h>
#include "baseconfig.h"
#include "util.h"

using namespace tinyxml2;

static std::map<std::string, int> Cache;

int CutSceneReader::LoadCutSceneTime(const char *fileName)
{
	auto i = Cache.find(fileName);
	if (i != Cache.end())
	{
		return i->second;
	}

	tinyxml2::XMLDocument doc;
	XMLError error = doc.LoadFile(Config::GetConfig()->GetFilePath(fileName).c_str());
	if (error != XML_SUCCESS)
	{
		LogError("parser cutscene file %s get XMLError code: %d", fileName, error);
		return 0;
	}

	XMLElement *cutSceneDataNode = doc.FirstChildElement("XCutSceneData");
	XMLElement *elem = NULL;

	elem = cutSceneDataNode->FirstChildElement("TotalFrame");
	int cutSceneTime = 0;
	if (elem)
	{
		cutSceneTime = (int)(convert<float>(elem->GetText()) * 1000 / 30);
	}

	Cache[std::string(fileName)] = cutSceneTime;

	return cutSceneTime;
}
