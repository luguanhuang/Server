#ifndef _AINODEFACTORY_H_ 
#define _AINODEFACTORY_H_

#include <tinyxml2.h>

class AINodeBase;

class AINodeFactory
{
public:
	static AINodeBase* CreateAINodeByName(const std::string& nodeName, tinyxml2::XMLElement* elem, AINodeBase* root );
	static int CreateIndexByName(const std::string& name);

	static void SetActiveTree(const std::string& tree);

	struct NameIndexInfo
	{
		std::map<std::string, int> mNameHash;
		int mNameIndex;

		void Reset()
		{
			mNameHash.clear();
			mNameIndex = 0;
		}
	};
private:
	//static std::map<std::string, NameIndexInfo> mNameIndexInfos;
	static NameIndexInfo mCommonNameIndexInfo;
	static NameIndexInfo mTempNameIndexInfo;
	static NameIndexInfo* mpCurActiveNameIndexInfo;
	static std::string mCurActiveTreeName;
};

#endif