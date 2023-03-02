using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/30 21:05:23

namespace XMainClient
{
    using DataType = KKSG.SkyCityAllTeamBaseInfo;

	class PtcG2C_SkyCityTeamRes : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_SkyCityTeamRes()
		{

		}

		public override uint GetProtoType()
		{
			return 49519;
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
			Process_PtcG2C_SkyCityTeamRes.Process(this);
		}

	}
}
