#ifndef __XEXPRESS_H__
#define __XEXPRESS_H__

class XExpress
{
	struct NodeTable
	{
		NodeTable ()
		{
			nParam		= 0;			// ����1
			nOper		= 0;			// �������� > = <
			nResult		= 0;			// �߼������Ľ��
			nLogic		= 0;			// �߼�������� | &
			nParamIndex = 0;			// �߼����� λ��
			pLeft		= NULL;			// ����ָ��
			pRight		= NULL;			// ����ָ��
			pParnent	= NULL;			// ���׽ڵ�
		}
		int nParam;				// ����1
		int nOper;				// �������� > = <
		int nResult;			// �߼������Ľ��
		int nLogic;				// �߼�������� | &
		int nParamIndex;		// �߼�����λ��
		NodeTable *pLeft;		// ����ָ��
		NodeTable *pRight;		// ����ָ��
		NodeTable *pParnent;	// ���׽ڵ�
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
