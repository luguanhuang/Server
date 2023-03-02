using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/10/8 14:13:16

namespace XMainClient
{
    using DataType = KKSG.GuildCheckinBoxNtf;

	class PtcG2C_GuildCheckinBoxNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_GuildCheckinBoxNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 5114;
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
			Process_PtcG2C_GuildCheckinBoxNtf.Process(this);
		}

	}
}
