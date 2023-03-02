using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/29 20:17:16

namespace XMainClient
{
    using DataType = KKSG.IdipClearChatInfo;

	class PtcM2C_IdipClearChatNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_IdipClearChatNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 47934;
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
			Process_PtcM2C_IdipClearChatNtf.Process(this);
		}

	}
}
