#ifndef __FORBIDWORD_H__
#define __FORBIDWORD_H__

#include "table/ForbidWord.h"
#include "string.h"

struct FWnode;
struct FWchildNode
{
	FWchildNode()
	{
		m_c = 0;
		m_onewend = false;
		m_child = NULL;
	}
	bool m_onewend; 
	char m_c;
	FWnode* m_child;
};

struct FWnode
{
	std::list<FWchildNode> m_childs;
};

class ForbidWordMgr
{
	ForbidWordMgr();
	~ForbidWordMgr();
	DECLARE_SINGLETON(ForbidWordMgr);
public:
	bool Init();
	void Uninit();

	//Ãô¸Ð´Ê
	bool HaveForbidWord(const std::string& word);
	void FilterForbidWord(std::string& word);
	//·Ç·¨×Ö·û
	bool HaveIllegalCode(const std::string& word);
	void FilterIllegalCode(std::string& word);

private:
	void BuildTree();
	void AddWord(const std::string& word);
	void DelTree(FWnode* cur);

	int MatchLenght(const char* s, int size);

	bool IsLegalChinese(const string& word);
	int EncodeLength(char c);
	bool IsLegalChar(char c);

	FWnode* m_root;
	ForbidWord m_table;
};
#endif