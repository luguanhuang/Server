using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/12/16 16:38:20

namespace XMainClient
{
    using DataType = KKSG.GuildGoblinSceneInfo;

	class PtcG2C_GuildGoblinKillNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_GuildGoblinKillNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 9436;
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
			Process_PtcG2C_GuildGoblinKillNtf.Process(this);
		}

	}
}
