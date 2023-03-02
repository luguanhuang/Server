#include "pch.h"
#include "forbidwordmgr.h"

INSTANCE_SINGLETON(ForbidWordMgr);

ForbidWordMgr::ForbidWordMgr()
{
	m_root = NULL;	
}

ForbidWordMgr::~ForbidWordMgr()
{

}

bool ForbidWordMgr::Init()
{
	if (!m_table.LoadFile("table/ForbidWordInName.txt"))
	{
		LogWarn("Load ForbidWordInName.txt failed!");
		return false;
	}

	BuildTree();

	return true;
}

void ForbidWordMgr::Uninit()
{
	DelTree(m_root);
}

void ForbidWordMgr::BuildTree()
{
	if(m_root)
	{
		return ;
	}
	m_root = new FWnode();
	for(size_t i = 0; i < m_table.Table.size(); ++i)
	{
		AddWord(m_table.Table[i]->forbidword);
	}
	AddWord("\n");
}

void ForbidWordMgr::AddWord( const std::string& word )
{
	if(NULL == m_root)
	{
		SSError << "forbid word build tree error root NULL " << word << END;
	}

	FWnode* curnode = m_root;
	FWchildNode* curchildnode = NULL;
	for(size_t i = 0; i < word.size(); ++i)
	{
		if(NULL == curnode)
		{
			if(NULL == curchildnode)
			{
				SSError << "forbid word build tree error ptrs NULL " << word << END;
				return;
			}else
			{
				if(NULL == curchildnode->m_child)
				{
					curchildnode->m_child = new FWnode();
				}
				curnode = curchildnode->m_child;
			}
		}
		if(NULL == curnode)
		{
			SSError << "forbid word build tree error ptr NULL " << word << END;
			return;
		}
		char pc = word[i];
		curchildnode = NULL;
		for(auto it = curnode->m_childs.begin(); it != curnode->m_childs.end(); ++it)
		{
			if((*it).m_c == pc)
			{
				curchildnode = &(*it);
				break;
			}
		}
		if(NULL == curchildnode)
		{
			FWchildNode pchnode;
			pchnode.m_c = pc;
			curnode->m_childs.push_back(pchnode);
			curchildnode = &(curnode->m_childs.back());
		}
		curnode = NULL;
	}
	if(curchildnode)
	{
		curchildnode->m_onewend = true;
	}
}

void ForbidWordMgr::DelTree(FWnode* cur)
{
	if(cur)
	{
		for(auto it = cur->m_childs.begin(); it != cur->m_childs.end(); ++it)
		{
			DelTree((*it).m_child);
		}
		cur->m_childs.clear();
		delete cur;
		cur = NULL;
	}
}

bool ForbidWordMgr::HaveForbidWord( const std::string& word )
{
	const char* s = word.c_str();
	int size = (int)word.size();
	for(int i = 0; i < size; ++i)
	{
		if(MatchLenght(s+i, size-i))
		{
			return true;
		}
	}
	return false;
}

void ForbidWordMgr::FilterForbidWord( std::string& word )
{
	const char* s = word.c_str();
	int size = (int)word.size();
	for(int i = 0; i < size; ++i)
	{
		int len = MatchLenght(s+i, size-i);
		if(len)
		{
			for(int j = 0; j < len; ++j)
			{
				if(i + j < size)
				{
					word[i + j] = '*';
				}
				else
				{
					SSError << "filter forbid word error = " << word << END;
				}
			}
			i += (len-1);
		}
	}
}

int ForbidWordMgr::MatchLenght( const char* s, int size)
{
	int len = 0;
	FWnode* cur = m_root;
	for(int i = 0; i < size; ++i)
	{
		char pc = *(s + i);	
		//SSError << (UINT8)(pc) << " " <<END;
		if(NULL == cur)
		{
			break;
		}
		FWchildNode* childcur = NULL;
		for(auto it = cur->m_childs.begin(); it != cur->m_childs.end(); ++it)
		{
			if((*it).m_c == pc)
			{
				childcur = &(*it);
			}
		}
		cur = NULL;
		if(childcur)
		{
			if(childcur->m_onewend)
			{
				len = i + 1;
				break;
			}
			cur = childcur->m_child;
		}
	}
	//SSError << "end" << END;
	return len;
}

bool ForbidWordMgr::HaveIllegalCode( const std::string& word )
{
	auto it = word.begin();
	while(it < word.end())
	{
		char c = (*it);
		int len = EncodeLength(c);
		if(0 == len)//ASCII
		{
			if(!IsLegalChar(c))
			{
				return true;
			}
			else
			{
				++it;
			}
		}
		else
		{
			if(it + len > word.end())
			{
				return true;
			}
			if(!IsLegalChinese(std::string(it, it+len)))
			{
				return true;
			}
			else
			{
				it += len;
			}
		}
	}
	return false;
}

void ForbidWordMgr::FilterIllegalCode( std::string& word)
{
	auto it = word.begin();
	while(it < word.end())
	{
		char c = (*it);
		int len = EncodeLength(c);
		if(0 == len)//ASCII
		{
			if(!IsLegalChar(c))
			{
				it = word.erase(it);
			}
			else
			{
				++it;
			}
		}
		else
		{
			if(it + len > word.end())
			{
				SSError << " Name Encoding Error = " << word << END;
				return ;
			}
			if(!IsLegalChinese(std::string(it, it+len)))
			{
				it = word.erase(it, it+len);
			}
			else
			{
				it += len;
			}
		}
	}
}

int ForbidWordMgr::EncodeLength( char c )
{
	int len = 0;
	for(int i = 7; i > 0; --i)
	{
		if(!((0x01<<i)&c))
		{
			break;
		}
		++len;
	}
	return len;
}

bool ForbidWordMgr::IsLegalChar( char c )
{
	if(('0' <= c && c <= '9')||
		('A' <= c && c <= 'Z')||
		('a' <=c && c <= 'z'))
	{
		return true;
	}
	return false;
}

bool ForbidWordMgr::IsLegalChinese( const string& word)
{
	//常用汉字 
	//unicode 范围 U+4E00 --- U+9FBB
	//utf-8 范围   0xE4B880 --- 0xE9BEBB
   
	static std::string cwmin;
	static std::string cwmax;
	if(cwmin.empty())
	{
		cwmin.push_back((char)(0xE4));
		cwmin.push_back((char)(0xB8));
		cwmin.push_back((char)(0x80));
	}
	if(cwmax.empty())
	{
		cwmax.push_back((char)(0xE9));
		cwmax.push_back((char)(0xBE));
		cwmax.push_back((char)(0xBB));
	}

	if(3 != (int)word.size())//常用汉字长度为3
	{
		return false;
	}
	if(cwmin <= word && word<= cwmax)
	{
		return true;
	}
	return false;
}
