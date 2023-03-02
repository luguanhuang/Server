using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/1 20:10:40

namespace XMainClient
{
    using DataType = KKSG.GuildCardRankNtf;

	class PtcM2C_GuildCardRankNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_GuildCardRankNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 63693;
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
			Process_PtcM2C_GuildCardRankNtf.Process(this);
		}

	}
}
