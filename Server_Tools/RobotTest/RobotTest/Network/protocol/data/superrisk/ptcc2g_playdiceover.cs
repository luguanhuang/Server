using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/5 16:14:04

namespace XMainClient
{
    using DataType = KKSG.PlayDiceOverData;

	class PtcC2G_PlayDiceOver : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_PlayDiceOver()
		{

		}

		public override uint GetProtoType()
		{
			return 2064;
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
