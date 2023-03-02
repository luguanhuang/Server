#include "pch.h"
#include "express.h"

void XExpress::BuildTree(NodeTable **pRoot,const std::string str)
{
	NodeTable *pCur = NULL;
	int nPos = 0;
	while(nPos < str.length())
	{
		if(str[nPos] == '(')
		{
			NodeTable *pNode =  new NodeTable;
			if (pCur)
			{
				pCur->pLeft		 =  pNode;
				pNode->pParnent  =  pCur;
			}else
			{
				*pRoot = pNode;
			}
			pCur   = pNode;
			nPos++;
		}else if(str[nPos] == ')')
		{
			nPos++;
			pCur = pCur->pParnent;
		}else if (str[nPos] == '&'||str[nPos] == '|')
		{
			if (str[nPos]=='&')
			{
				pCur->nLogic = 1;
			}else if(str[nPos]=='|')
			{
				pCur->nLogic = 2;
			}		
			NodeTable *pNode	=  new NodeTable;
			pCur->pRight		=  pNode;
			pNode->pParnent		=  pCur;
			pCur   = pNode;
			nPos++;
			nPos++;
		}else if(str[nPos] == 'a'||str[nPos] == 'b'||str[nPos] == 'c')
		{
			pCur->nParamIndex = str[nPos] - 97 + 1;
			nPos++;
			if (str[nPos] == '<'||str[nPos] == '='||str[nPos] == '>')
			{
				pCur->nOper = str[nPos] - 60 + 1;
				nPos++;
				std::string strNext	= str.substr(nPos, str.length()-1);
				int nCurPos		= strNext.find(")");			
				std::string strNum	= strNext.substr(0, nCurPos);
				pCur->nParam    = atoi(strNum.c_str());			//抽取数值
				nPos			+= nCurPos;
			}
		}else
		{
			nPos++;
		}
	}
}
//后根遍历算法
void XExpress::Execute(XExpress::NodeTable *pNode,int nParam1,int nParam2,int Param3)
{
	if(pNode!=NULL)
	{
		Execute(pNode->pLeft,nParam1,nParam2,Param3);	
		Execute(pNode->pRight,nParam1,nParam2,Param3);
		switch(pNode->nOper)
		{
		case XExpress::eSmaller:
			{
				switch (pNode->nParamIndex)
				{
				case XExpress::eParam1:
					{
						if (nParam1<pNode->nParam){pNode->nResult = 1;}else{pNode->nResult = 0;} break;
					}
				case XExpress::eParam2:
					{
						if (nParam2<pNode->nParam){pNode->nResult = 1;}else{pNode->nResult = 0;}  break;
					}
				case XExpress::eParam3:
					{
						if (Param3<pNode->nParam){pNode->nResult = 1;}else{pNode->nResult = 0;}  break;
					}
				default:{}
				}
				break;
			}
		case XExpress::eBiger:
			{
				switch (pNode->nParamIndex)
				{
				case XExpress::eParam1:
					{
						if (nParam1>pNode->nParam){pNode->nResult = 1;}else{pNode->nResult = 0;}	break;
					}
				case XExpress::eParam2:
					{
						if (nParam2>pNode->nParam){pNode->nResult = 1;}else{pNode->nResult = 0;}  break;
					}
				case XExpress::eParam3:
					{
						if (Param3>pNode->nParam){pNode->nResult = 1;}else{pNode->nResult = 0;}   break;
					}
				default:{}
				}
				break;
			}
			break;
		case XExpress::eEqual:
			{
				switch (pNode->nParamIndex)
				{
				case XExpress::eParam1:
					{
						if (pNode->nParam == nParam1){pNode->nResult = 1;}else{pNode->nResult = 0;} break;
					}
				case XExpress::eParam2:
					{
						if (pNode->nParam == nParam2){pNode->nResult = 1;}else{pNode->nResult = 0;} break;
					}
				case XExpress::eParam3:
					{
						if (pNode->nParam == Param3){pNode->nResult = 1;}else{pNode->nResult = 0;} break;
					}
				default:{}
				}
				break;
			}
		default:{}
		}
		switch(pNode->nLogic)
		{
		case XExpress::eAnd:
			{
				if (pNode->pLeft->nResult&&pNode->pRight->nResult){pNode->nResult = 1;}break;
			}
		case XExpress::eOr:
			{
				if (pNode->pLeft->nResult||pNode->pRight->nResult){pNode->nResult = 1;}break;
			}
		default:{}
		}
	}
}


void XExpress::Destory(XExpress::NodeTable *pTree)
{
	if(pTree==NULL){return;}
	Destory(pTree->pLeft);
	Destory(pTree->pRight);
	delete pTree;
}

//  NodeTable *pRoot = NULL;;
//	BuildTree(&pRoot,"((a<3)&(b=5))");
//	Execute(pRoot,4,5,0);
//	BuildTree(&pRoot,"(a>3)");
//	BuildTree(&pRoot,"((a>3)|((b=5)&(c<4)))");
//  BuildTree(&pRoot, "(((a>3)&(b=5))&(c>4))");
//  Execute(pRoot,4,5,0);
//  Destory(pRoot);
