using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/22 21:17:19

namespace XMainClient
{
    using DataType = KKSG.JoinRoom;

	class PtcC2G_JoinRoom : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_JoinRoom()
		{

		}

		public override uint GetProtoType()
		{
			return 8517;
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
