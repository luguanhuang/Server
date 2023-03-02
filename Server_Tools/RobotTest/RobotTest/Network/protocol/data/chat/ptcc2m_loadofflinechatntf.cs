using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/3 2:06:32

namespace XMainClient
{
    using DataType = KKSG.LoadOffLineChatNtf;

	class PtcC2M_LoadOfflineChatNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_LoadOfflineChatNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 26622;
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
