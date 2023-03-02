using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/6/6 18:14:48

namespace XMainClient
{
    using DataType = KKSG.OpenPrivateChat;

	class PtcC2M_OpenPrivateChatNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_OpenPrivateChatNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 23206;
		}

		public override void Serialize(MemoryStream stream)
		{
			Serializer.Serialize(stream, Data);
		}

		public override void DeSerialize(MemoryStream stream)
		{
			Data = Serializer.Deserialize<DataType>(stream);
		}

		public override void Process()
		{
			throw new Exception("Send only protocol can not call process");
		}

	}
}
