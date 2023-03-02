using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/25 17:03:17

namespace XMainClient
{
    //using DataType = KKSG.;

	class PtcG2C_FlowerRankRewardNtf : Protocol
	{
		//public DataType Data = new DataType();

		public PtcG2C_FlowerRankRewardNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 14326;
		}

		public override void Serialize(MemoryStream stream)
		{
			//Serializer.Serialize(stream, Data);
		}

		public override void DeSerialize(MemoryStream stream)
		{
			//Data = Serializer.Deserialize<DataType>(stream);
		}

		public override void Process()
		{
			Process_PtcG2C_FlowerRankRewardNtf.Process(this);
		}

	}
}
