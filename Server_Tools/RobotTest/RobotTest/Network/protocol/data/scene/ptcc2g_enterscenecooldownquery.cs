using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/7/30 18:15:16

namespace XMainClient
{
    using DataType = KKSG.EnterSceneCoolDownQuery;

	class PtcC2G_EnterSceneCoolDownQuery : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_EnterSceneCoolDownQuery()
		{

		}

		public override uint GetProtoType()
		{
			return 40442;
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
