#ifndef _AITREEMGR_H_ 
#define _AITREEMGR_H_

#include <unordered_map>
#include <string>
#include <vector>
#include <tinyxml2.h>
#include "unit/unit.h"

class AINodeBase;
class RootNode;

bool GetFilesBySuffix( const std::string &path , const std::string &suffix , std::vector<std::string>& files);
class AITreeMgr
{
	class TreeInfo
	{
	private:
		std::vector<RootNode*> m_TreeVersions;
		std::unordered_map<RootNode*, int> m_TreeRefCount;
	public:
		TreeInfo();
		virtual ~TreeInfo();
		void AddTreeNode(RootNode* node);
		void RemoveTreeNode(RootNode* node);

		void AddTreeRefCount(RootNode* node);
		void RemoveTreeRefCount(RootNode* node);

		RootNode* GetLatestTree();

		UINT32 GetTreeCount();
	};

	AITreeMgr();
	~AITreeMgr();
	DECLARE_SINGLETON(AITreeMgr)
public:
	bool Init();
	void Uninit();

	void Update();

	bool LoadFile();
	bool LoadTreeInDir();

	RootNode* GetAITreeByName(const std::string& name);
	void IncTreeRef(const std::string& name ,RootNode* node);
	void DecTreeRef(const std::string& name, RootNode* node);

	bool LoadOneAITree(const std::string& fileName);

private:
	AINodeBase* LoadOneNode(tinyxml2::XMLElement* elem, AINodeBase* root);

	std::unordered_map<std::string,TreeInfo*> m_AITreeMap;

};

#endif