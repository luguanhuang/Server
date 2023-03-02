#ifndef __XEXPRESS_H__
#define __XEXPRESS_H__

class XExpress
{
	struct NodeTable
	{
		NodeTable ()
		{
			nParam		= 0;			// 参数1
			nOper		= 0;			// 操作符号 > = <
			nResult		= 0;			// 逻辑计算后的结果
			nLogic		= 0;			// 逻辑运算符号 | &
			nParamIndex = 0;			// 逻辑参数 位置
			pLeft		= NULL;			// 左树指针
			pRight		= NULL;			// 右树指针
			pParnent	= NULL;			// 父亲节点
		}
		int nParam;				// 参数1
		int nOper;				// 操作符号 > = <
		int nResult;			// 逻辑计算后的结果
		int nLogic;				// 逻辑运算符号 | &
		int nParamIndex;		// 逻辑参数位置
		NodeTable *pLeft;		// 左树指针
		NodeTable *pRight;		// 右树指针
		NodeTable *pParnent;	// 父亲节点
	};
	enum eCompare
	{
		eSmaller = 1,
		eEqual	 = 2,
		eBiger   = 3,	
	};
	enum eParam
	{
		eParam1=1,
		eParam2=2,
		eParam3=3,
	};
	enum eLogic
	{
		eAnd = 1,
		eOr  = 2,
	};
public:
	static void BuildTree(NodeTable **pRoot,const std::string str);
	static void Execute(XExpress::NodeTable *pNode,int nParam1,int nParam2,int Param3);
	static void Destory(XExpress::NodeTable *pTree);
};
#endif
