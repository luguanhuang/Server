using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/26 14:29:59

namespace XMainClient
{
    using DataType = KKSG.MyGuild;

	class PtcM2C_LoginGuildInfo : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_LoginGuildInfo()
		{

		}

		public override uint GetProtoType()
		{
			return 29049;
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
			Process_PtcM2C_LoginGuildInfo.Process(this);
		}

	}
}
