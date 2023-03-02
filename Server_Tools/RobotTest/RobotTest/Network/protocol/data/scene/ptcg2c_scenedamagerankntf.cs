using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/12/24 17:44:39

namespace XMainClient
{
    using DataType = KKSG.SceneDamageRankNtf;

	class PtcG2C_SceneDamageRankNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_SceneDamageRankNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 26864;
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
			Process_PtcG2C_SceneDamageRankNtf.Process(this);
		}

	}
}
