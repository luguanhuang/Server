using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/7 11:35:52

namespace XMainClient
{
    using DataType = KKSG.MyDragonGuild;

	class PtcM2C_LoginDragonGuildInfo : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_LoginDragonGuildInfo()
		{

		}

		public override uint GetProtoType()
		{
			return 21856;
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
			Process_PtcM2C_LoginDragonGuildInfo.Process(this);
		}

	}
}
