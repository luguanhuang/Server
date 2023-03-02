#ifndef _H_Test_Case_H__
#define _H_Test_Case_H__


typedef std::vector<std::string> CArgList;


class CTestCase
{
public:
	CTestCase() {}
	virtual ~CTestCase() {}

	virtual bool Init() { return true; }
	virtual void Uninit() {}

	virtual CTestCase* New() = 0;
	virtual void Run(const CArgList& roArgs) = 0;
};


#endif