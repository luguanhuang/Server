using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/12/23 11:03:05

namespace XMainClient
{
    using DataType = KKSG.AddLevelScriptExtString;

	class PtcC2G_AddLevelScriptExtString : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_AddLevelScriptExtString()
		{

		}

		public override uint GetProtoType()
		{
			return 34579;
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
