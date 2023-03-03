/*
qqcrypt.h


QQ加密
hyj@tencent.com
1999/12/24
2002/03/27

  实现下列算法:
  Hash算法: MD5,已实现
  对称算法: DES,未实现
  非对称算法: RSA,未实现


2002/06/14
	修改v3解密函数，去掉v3加密函数

*/

#ifndef TRUE
#define TRUE 1
#endif /* TRUE */

#ifndef FALSE
#define FALSE 0
#endif /* FALSE */


#ifndef _INCLUDED_OICQCRYPT_H_
#define _INCLUDED_OICQCRYPT_H_


#define ENCRYPT_PADLEN		18
#define	CRYPT_KEY_SIZE		16

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/************************************************************************************************
	对称加密底层函数
************************************************************************************************/
//pOutBuffer、pInBuffer均为8byte, pKey为16byte
void TeaEncryptECB(const char *pInBuf, const char *pKey, char *pOutBuf);
void TeaDecryptECB(const char *pInBuf, const char *pKey, char *pOutBuf);
void TeaEncryptECB3(const char *pInBuf, const char *pKey, char *pOutBuf);
void TeaDecryptECB3(const char *pInBuf, const char *pKey, char *pOutBuf);



/************************************************************************************************
	QQ对称加密第一代函数
************************************************************************************************/

/*pKey为16byte*/
/*
	输入:pInBuf为需加密的明文部分(Body),nInBufLen为pInBuf长度;
	输出:pOutBuf为密文格式,pOutBufLen为pOutBuf的长度是8byte的倍数,至少应预留nInBufLen+17;
*/
/*TEA加密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
void oi_symmetry_encrypt(const char* pInBuf, int nInBufLen, const char* pKey, char* pOutBuf, int *pOutBufLen);

/*pKey为16byte*/
/*
	输入:pInBuf为密文格式,nInBufLen为pInBuf的长度是8byte的倍数; *pOutBufLen为接收缓冲区的长度
		特别注意*pOutBufLen应预置接收缓冲区的长度!
	输出:pOutBuf为明文(Body),pOutBufLen为pOutBuf的长度,至少应预留nInBufLen-10;
	返回值:如果格式正确返回TRUE;
*/
/*TEA解密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
char oi_symmetry_decrypt(const char* pInBuf, int nInBufLen, const char* pKey, char* pOutBuf, int *pOutBufLen);







/************************************************************************************************
	QQ对称加密第二代函数
************************************************************************************************/

/*pKey为16byte*/
/*
	输入:nInBufLen为需加密的明文部分(Body)长度;
	输出:返回为加密后的长度(是8byte的倍数);
*/
/*TEA加密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
int oi_symmetry_encrypt2_len(int nInBufLen);


/*pKey为16byte*/
/*
	输入:pInBuf为需加密的明文部分(Body),nInBufLen为pInBuf长度;
	输出:pOutBuf为密文格式,pOutBufLen为pOutBuf的长度是8byte的倍数,至少应预留nInBufLen+17;
*/
/*TEA加密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
void oi_symmetry_encrypt2(const char* pInBuf, int nInBufLen, const char* pKey, char* pOutBuf, int *pOutBufLen);



/*pKey为16byte*/
/*
	输入:pInBuf为密文格式,nInBufLen为pInBuf的长度是8byte的倍数; *pOutBufLen为接收缓冲区的长度
		特别注意*pOutBufLen应预置接收缓冲区的长度!
	输出:pOutBuf为明文(Body),pOutBufLen为pOutBuf的长度,至少应预留nInBufLen-10;
	返回值:如果格式正确返回TRUE;
*/
/*TEA解密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
char oi_symmetry_decrypt2(const char* pInBuf, int nInBufLen, const char* pKey, char* pOutBuf, int *pOutBufLen);




/************************************************************************************************
	QQ对称解密第三代函数
************************************************************************************************/

/*pKey为16byte*/
/*
	输入:nInBufLen为需加密的明文部分(Body)长度;
	输出:返回为加密后的长度(是8byte的倍数);
*/
/*TEA加密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
int oi_symmetry_encrypt3_len(int nInBufLen);


/*pKey为16byte*/
/*
	输入:pInBuf为密文格式,nInBufLen为pInBuf的长度是8byte的倍数; *pOutBufLen为接收缓冲区的长度
		特别注意*pOutBufLen应预置接收缓冲区的长度!
	输出:pOutBuf为明文(Body),pOutBufLen为pOutBuf的长度,至少应预留nInBufLen-10;
	返回值:如果格式正确返回TRUE;
*/
/*TEA解密算法,CBC模式*/
/*密文格式:PadLen(1byte)+Padding(var,0-7byte)+Salt(2byte)+Body(var byte)+Zero(7byte)*/
char qq_symmetry_decrypt3(const char* pInBuf, int nInBufLen, char chMainVer, char chSubVer, unsigned long dwUin, const char* pKey, char* pOutBuf, int *pOutBufLen);

#ifdef __cplusplus
}
#endif

#endif // #ifndef _INCLUDED_OICQCRYPT_H_

