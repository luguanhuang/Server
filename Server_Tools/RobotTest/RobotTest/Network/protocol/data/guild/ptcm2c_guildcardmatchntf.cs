using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/2 17:39:33

namespace XMainClient
{
    using DataType = KKSG.GuildCardMatchNtf;

	class PtcM2C_GuildCardMatchNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_GuildCardMatchNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 64513;
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
			Process_PtcM2C_GuildCardMatchNtf.Process(this);
		}

	}
}
