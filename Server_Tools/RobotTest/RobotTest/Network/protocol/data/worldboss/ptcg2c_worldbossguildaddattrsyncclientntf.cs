using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/4/10 14:56:15

namespace XMainClient
{
    using DataType = KKSG.WorldBossGuildAddAttrSyncClient;

	class PtcG2C_WorldBossGuildAddAttrSyncClientNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_WorldBossGuildAddAttrSyncClientNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 65314;
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
			Process_PtcG2C_WorldBossGuildAddAttrSyncClientNtf.Process(this);
		}

	}
}
