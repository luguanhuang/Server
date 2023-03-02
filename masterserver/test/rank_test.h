#ifndef __H_Rank_Test_H__
#define __H_Rank_Test_H__

#include "test/testcase.h"


class CRankTestCase : public CTestCase
{
public:
	CRankTestCase() {}
	virtual ~CRankTestCase() {}

	virtual CTestCase* New() { return new CRankTestCase(); }
	virtual void Run(const CArgList& roArgs);
};


class garden: public CTestCase
{
	public:
	garden() {}
	virtual ~garden() {}

	virtual garden* New() { return new garden(); }
	virtual void Run(const CArgList& roArgs);
};

#endif