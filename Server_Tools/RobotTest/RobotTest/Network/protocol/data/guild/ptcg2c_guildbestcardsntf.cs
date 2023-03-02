using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/10/11 17:33:06

namespace XMainClient
{
    using DataType = KKSG.GuildBestCardsNtf;

	class PtcG2C_GuildBestCardsNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_GuildBestCardsNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 44473;
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
			Process_PtcG2C_GuildBestCardsNtf.Process(this);
		}

	}
}
