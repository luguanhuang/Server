/**
 * 生成签名类
 *
 * @version 3.0.1
 * @author open.qq.com
 * @copyright (c) 2013, Tencent Corporation. All rights reserved.
 * @ History:
 *				 3.0.1 | harryhlli  | 2013-09-06 09:50:19 | fixed compilation errors
 *               3.0.0 | jixingguan | 2013-05-27 11:11:11 | initialization
 */
#ifndef TENCENT_OPEN_API_SNS_CHECK_H
#define TENCENT_OPEN_API_SNS_CHECK_H
#include <string>
#include <map>
#include <algorithm>
#include <stdio.h>
#include "singleton.h"
#if defined(WIN32) || defined(_WIN64)
#define snprintf _snprintf
#endif
/**
 * 生成签名类
 */
using namespace std;
class CSnsSigCheck
{
	CSnsSigCheck(){};
	~CSnsSigCheck(){};
	DECLARE_SINGLETON(CSnsSigCheck);

public:
	bool Init();
	void Uninit();
	/***********************************************************
	**函数名 GetErrMsg
	**输入:   
	**			无
	**
	**输出:  
	**				
	**返回:无
	**描述:
	**			返回出错信息
	**************************************************************/
	const char* GetErrMsg() { return m_szErrMsg;}
	
	//by huangds, 2018/5/16
	string makeSig2(map<string, string>& params);

	/***********************************************************
	**函数名 makeSig
	**输入:   
	**			method 请求方法 "get" or "post"
				url_path   openapi名称
				params    请求参数
				secret      密钥
	**输出:  
	**				
	**返回:  签名
	**描述: 生成签名
	**			  
	**************************************************************/	
    string makeSig(string& method, string& url_path, map<string, string>& params, string& secret);

	 /***********************************************************
	 **函数名 encodevalue
	 **输入:   
	 ** 		 method 请求方法 "get" or "post"
				 url_path	openapi名称
				 params    请求参数
				 secret 	 密钥
	 **输出:  
	 ** 			 
	 **返回:  签名
	 **描述: 回调发货URL专用的编码算法
	 **            编码规则为：除了 0~9 a~z A~Z !*()之外其他字符按其ASCII码的十六进制加%进行表示，例如"-"编码为"%2D"
	 **		  http://wiki.open.qq.com/wiki/%E5%9B%9E%E8%B0%83%E5%8F%91%E8%B4%A7URL%E7%9A%84%E5%8D%8F%E8%AE%AE%E8%AF%B4%E6%98%8E_V3
	 **************************************************************/ 
	
		/***********************************************************
	**函数名 verifySig
	**输入:   
	**			method    请求方法 "get" or "post"
	**			url_path
	**			params   腾讯调用发货回调URL携带的请求参数
	**			sig         腾讯调用发货回调URL时传递的签名
	**
	**输出:  
	**				
	**返回:无
	**描述:
	**			验证回调发货URL的签名 (注意和普通的OpenAPI签名算法不一样，详见
	**                http://wiki.open.qq.com/wiki/%E5%9B%9E%E8%B0%83%E5%8F%91%E8%B4%A7URL%E7%9A%84%E5%8D%8F%E8%AE%AE%E8%AF%B4%E6%98%8E_V3)
	**************************************************************/
	bool verifySig(string& method, string& url_path, map<string,string>& params,string& sig, string& secret);
	
	string encodevalue(string& value);

	 /***********************************************************
	 **函数名 join_params
	 **输入:   
	 ** 		 params  请求参数
	 **输出:  
	 ** 			 
	 **返回:  拼接后的参数
	 **描述: 将参数以&进行拼接
	 ** 		   
	 **************************************************************/ 
	string join_params(map<string,string> &params);
    
	string makeCookie(map<string, string>& params);
	string url_encode(const string& src);	

private:
	static string replace_str(string src,const string& old_value,const string& new_value);
	char m_szErrMsg[512];

};
#endif
