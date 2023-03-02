/*
 * =====================================================================================
 *    Description:  ��������� ������
 *    Author:  LiYang 
 *
 * =====================================================================================
 */

#ifndef _Packet_Coder_H__
#define _Packet_Coder_H__


class IPacketCoder
{
public:
	virtual ~IPacketCoder() {}


	// ����ֵ:	  ��ʾ�Ƿ�Ҫ�����ݱ��� ����
	//			  ����true�Ż���� Encode Decode �ӿ�
	virtual bool IsCoded() const = 0;


	// pcIn:      ������Ļ�����
	// dwInLen:   ����������ݳ���
	// pcOut:	  ������������������
	// dwOutLen:  ����ʱΪ���뻺�������ȣ����ʱΪ���������ݳ���
	// ����ֵ:	  true��ʾ����ɹ���false��ʾ����ʧ��(����������ռ䲻��)
	virtual bool Encode(const char* pcIn, UINT32 dwInLen, char* pcOut, UINT32& dwOutLen) = 0;


	// pcIn:      ������Ļ�����
	// dwInLen:   ����������ݳ��ȣ�
	// pcOut:	  ������������������
	// dwOutLen:  ����ʱΪ���뻺�������ȣ����ʱΪ���������ݳ���
	// dwUsedLen: ��ʾ���뻺�������ѱ�����ʹ�õ����ݳ���
	// ����ֵ:	  true��ʾ����ɹ���false��ʾ����ʧ��(����������ռ䲻��)
	virtual bool Decode(const char* pcIn, UINT32 dwInLen, char* pcOut, UINT32& dwOutLen, UINT32& dwUsedLen) = 0;


	// pcData:	  �����������
	// nLen:	  ��������ݳ���
	// nPacketLen:����ɹ�ʱΪ�����ݳ���
	// ����ֵ:	  true��ʾ�ɹ����һ�����ݰ���false��ʾû��� (������true��nPacketLen == 0 �򲻴���)
	virtual bool BreakPacket(const char* pcData, UINT32 dwLen, UINT32& dwPacketLen) = 0;
};


#endif
