using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/18 21:07:38

namespace XMainClient
{
    using DataType = KKSG.SynDoingGuildInherit;

	class PtcC2G_SynDoingGuildInherit : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_SynDoingGuildInherit()
		{

		}

		public override uint GetProtoType()
		{
			return 51759;
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
