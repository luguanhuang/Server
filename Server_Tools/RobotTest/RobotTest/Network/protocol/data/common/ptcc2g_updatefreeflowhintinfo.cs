using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/21 19:43:57

namespace XMainClient
{
    using DataType = KKSG.UpdateFreeflowHintInfo;

	class PtcC2G_UpdateFreeflowHintInfo : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_UpdateFreeflowHintInfo()
		{

		}

		public override uint GetProtoType()
		{
			return 27628;
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
