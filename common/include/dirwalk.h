#ifndef __DIRWALK_H__
#define __DIRWALK_H__

#include <vector>
#include <string>

class IFileFilter
{
public:
	virtual ~IFileFilter(){}
	virtual bool operator()(const std::string & fileName) = 0;
};

class FileExtFilter : public IFileFilter
{
public:
	FileExtFilter(const char *ext);
	FileExtFilter(const std::string &ext);

	virtual bool operator()(const std::string & fileName);

private:
	std::string m_ext;
};

class DirWalk
{
public:
	DirWalk(const char *dir, bool Recursive);
	DirWalk(const std::string &dir, bool Recursive);

	void SetFilter(IFileFilter *pFilter);
	void Walk(std::vector<std::string> &output);

private:
	void _RealWalk(std::vector<std::string> &output, const std::string &path);

	std::string m_Dir;
	unsigned m_DirLength;
	bool m_Recursive;
	IFileFilter *m_pFilter;
};

#endif