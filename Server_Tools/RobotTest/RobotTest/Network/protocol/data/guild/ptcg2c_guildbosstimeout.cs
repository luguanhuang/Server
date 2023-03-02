using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/18 21:02:01

namespace XMainClient
{
    using DataType = KKSG.GuildBossTimeOut;

	class PtcG2C_GuildBossTimeOut : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_GuildBossTimeOut()
		{

		}

		public override uint GetProtoType()
		{
			return 56816;
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
			Process_PtcG2C_GuildBossTimeOut.Process(this);
		}

	}
}
