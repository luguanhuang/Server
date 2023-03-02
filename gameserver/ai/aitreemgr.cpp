#include "pch.h"
#include "aitreemgr.h"
#include "ainodefactory.h"
#include "ainode.h"
#include "ainodelogic.h"

#ifdef WIN32
#include <io.h>
#else
#include <dirent.h>
#endif
#include "table/SkillCombo.h"
#include "component/XAIComponent.h"


INSTANCE_SINGLETON(AITreeMgr)

bool CompareFileName(const std::string& a, const std::string& b)
{
	return strcmp(a.c_str(), b.c_str()) <= 0;
}

AITreeMgr::TreeInfo::TreeInfo()
{

}

AITreeMgr::TreeInfo::~TreeInfo()
{
	auto it = m_TreeVersions.begin();

	while (it != m_TreeVersions.end())
	{
		delete (*it);
		it ++;
	}

	m_TreeVersions.clear();
	m_TreeRefCount.clear();
}

UINT32 AITreeMgr::TreeInfo::GetTreeCount()
{
	return m_TreeVersions.size();
}

void AITreeMgr::TreeInfo::AddTreeNode(RootNode* node)
{
	auto it = m_TreeVersions.begin();

	while (it != m_TreeVersions.end())
	{
		if ((*it) == node)
			return;

		it ++;
	}

	// remove old tree if ref is 0

	auto treeit = m_TreeVersions.begin();

	while (treeit != m_TreeVersions.end())
	{
		RootNode* node = (*treeit);

		if (m_TreeRefCount.find(node) != m_TreeRefCount.end())
		{
			if (m_TreeRefCount[node] == 0)
			{
				treeit = m_TreeVersions.erase(treeit);
				m_TreeRefCount.erase(m_TreeRefCount.find(node));
				delete node;
			}
			else
				treeit ++;
		}
		else
			treeit = m_TreeVersions.erase(treeit);
	}

	// add the new node
	m_TreeVersions.push_back(node);
	m_TreeRefCount[node] = 0;
}

void AITreeMgr::TreeInfo::RemoveTreeNode(RootNode* node)
{
	auto it = m_TreeVersions.begin();

	if (m_TreeRefCount.find(node) != m_TreeRefCount.end())
	{
		auto refit = m_TreeRefCount.find(node);
		m_TreeRefCount.erase(refit);
	}

	while (it != m_TreeVersions.end())
	{
		if (node == (*it))
		{
			m_TreeVersions.erase(it);
			delete node;
			return;
		}
		it ++;
	}
}

void AITreeMgr::TreeInfo::AddTreeRefCount(RootNode* node)
{
	if (m_TreeRefCount.find(node) != m_TreeRefCount.end())
		m_TreeRefCount[node] += 1;
}

void AITreeMgr::TreeInfo::RemoveTreeRefCount(RootNode* node)
{
	if (m_TreeRefCount.find(node) != m_TreeRefCount.end() && m_TreeVersions.size() != 0)
	{
		m_TreeRefCount[node] -= 1;

		if (m_TreeRefCount[node] < 0)
			m_TreeRefCount[node] = 0;

		if (node != m_TreeVersions[m_TreeVersions.size()-1] && m_TreeRefCount[node] == 0)
		{
			RemoveTreeNode(node);
		}
	}
}

RootNode* AITreeMgr::TreeInfo::GetLatestTree()
{
	if (m_TreeVersions.size() == 0)
		return NULL;

	return m_TreeVersions[m_TreeVersions.size()-1];
}

bool AITreeMgr::Init()
{
	XAIComponent::InitIndex();

	if(!LoadTreeInDir())
	{
		return false;
	}

	return true;
}

void AITreeMgr::Uninit()
{
	for(auto it = m_AITreeMap.begin(); it != m_AITreeMap.end(); ++it)
	{
		delete it->second;
	}
	m_AITreeMap.clear();
}

bool AITreeMgr::LoadFile()
{
	return LoadTreeInDir();
}

void AITreeMgr::Update()
{

}

bool AITreeMgr::LoadTreeInDir()
{
	std::string path = "gsconf/table/AITree/";//配置文件路径

	std::vector<std::string> files;

	if(!GetFilesBySuffix(path , "xml" , files))
	{
		return false;
	}

	std::sort(files.begin(), files.end(), CompareFileName);

	bool ret = true;
	for(auto it = files.begin(); it != files.end(); ++it)
	{
		//if("Monster.xml" == (*it) && !LoadOneAITree(path + (*it)))
		if(!LoadOneAITree(path + (*it)))
		{
			ret = false;
		}
	}
	return ret;
}

RootNode* AITreeMgr::GetAITreeByName(const std::string& name)
{
	auto it = m_AITreeMap.find(name);
	if(it != m_AITreeMap.end())
	{
		return it->second->GetLatestTree();
	}
	return NULL;
}

void AITreeMgr::IncTreeRef(const std::string& name ,RootNode* node)
{
	auto it = m_AITreeMap.find(name);

	if (it != m_AITreeMap.end())
	{
		it->second->AddTreeRefCount(node);
	}
}

void AITreeMgr::DecTreeRef(const std::string& name, RootNode* node)
{
	auto it = m_AITreeMap.find(name);

	if (it != m_AITreeMap.end())
	{
		it->second->RemoveTreeRefCount(node);
	}
}

AITreeMgr::AITreeMgr()
{

}

AITreeMgr::~AITreeMgr()
{

}

bool AITreeMgr::LoadOneAITree(const std::string& fileName)
{
	tinyxml2::XMLDocument doc;

	tinyxml2::XMLError error = doc.LoadFile(fileName.c_str());
	if (error != tinyxml2::XML_SUCCESS)
	{
		LogError("parser AI Tree file %s get XMLError code: %d", fileName.c_str(), error);
		return false;
	}

	AINodeBase* aiTree = NULL;
	tinyxml2::XMLElement* root = doc.RootElement();

	if(root)
	{
		std::string name(root->Name());
		AINodeFactory::SetActiveTree(name);

		aiTree = LoadOneNode(root->FirstChildElement() , NULL);
		if(aiTree && aiTree->GetType() == NODE_TYPE_ROOT)
		{
			auto it = m_AITreeMap.find(name);

			if (it == m_AITreeMap.end())
			{
				TreeInfo* info = new TreeInfo();
				info->AddTreeNode((RootNode*)aiTree);

				m_AITreeMap.insert(std::make_pair(name, info));
			}
			else
			{
				m_AITreeMap[name]->AddTreeNode((RootNode*)aiTree);
			}

		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

AINodeBase* AITreeMgr::LoadOneNode(tinyxml2::XMLElement* elem , AINodeBase* root)
{
	if(NULL == elem)
	{
		return NULL;
	}

	std::string name(elem->Name());
	//SSInfo<< "elem name: " << name.c_str() <<END;
	AINodeBase* node = AINodeFactory::CreateAINodeByName(name, elem , root);

	if(NULL == root && node->GetType() == NODE_TYPE_ROOT)
	{
		root = node;
	}


	if(node)
	{
		node->PostCreate(elem);

		if("ConditionalEvaluator" == name)
		{
			ConditionalEvaluator* condNode = dynamic_cast<ConditionalEvaluator*>(node);
			if(condNode)
			{
				std::string conditionName = condNode->GetConditionNodeName();
				AINodeBase* conditionNode = AINodeFactory::CreateAINodeByName(conditionName , elem , root);
				if(conditionNode)
				{
					if(!condNode->AddConditionNode(conditionNode))
					{
						delete conditionNode;
						conditionNode = NULL;
					}
					else
					{
						conditionNode->PostCreate(elem);
					}
				}
			}
		}

		tinyxml2::XMLElement* subElem = elem->FirstChildElement();
		while(subElem)
		{
			AINodeBase* child = LoadOneNode(subElem, root);
			if(child)
			{
				node->AddChild(child);
			}
			subElem = subElem->NextSiblingElement();
		}
	}

	return node;
}

//读取文件夹下 所有特定文件
bool GetFilesBySuffix(const std::string &path , const std::string &suffix , std::vector<std::string>& files)
{

#ifdef WIN32
	struct _finddata_t fileinfo;
	long hFile = _findfirst((path + "*." + suffix).c_str() , &fileinfo);
	if(-1 != hFile)
	{
		do 
		{
			if(fileinfo.attrib & _A_SUBDIR) //文件夹
			{
				;
			}
			else
			{
				files.push_back(fileinfo.name);
			}

		} while (0 == _findnext(hFile , &fileinfo));

	}
	else
	{
		SSError << "get files error path = " << path << " errorcode = " + hFile <<END;
		return false;
	}
    
#else
    
    DIR* dp = NULL;
    dp = opendir(path.c_str());
    if(dp)
    {
        struct dirent* dirp = NULL;
        while((dirp = readdir(dp)) != NULL)
        {
            if(DT_REG == dirp->d_type)
            {
                const std::string name(dirp->d_name);
                if(name.size() >= 5)
                {
                    if(name.substr(name.size()-3,3) == suffix)
                    {
                        files.push_back(name);
                    }
                }
            } 
        }
    }
    else
    {
		SSError << "get files error path = " << path << END;
        return false;
    }

    closedir(dp);
#endif

	return true;

}
