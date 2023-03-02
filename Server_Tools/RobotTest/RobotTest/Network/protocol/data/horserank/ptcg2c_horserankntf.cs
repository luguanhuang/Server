using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/6 20:39:44

namespace XMainClient
{
    using DataType = KKSG.HorseRank;

	class PtcG2C_HorseRankNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_HorseRankNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 22250;
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
			Process_PtcG2C_HorseRankNtf.Process(this);
		}

	}
}
