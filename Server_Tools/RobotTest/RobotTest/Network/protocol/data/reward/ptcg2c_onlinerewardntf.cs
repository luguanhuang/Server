using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/10/20 17:54:11

namespace XMainClient
{
    using DataType = KKSG.OnlineRewardNtf;

	class PtcG2C_OnlineRewardNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_OnlineRewardNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 1895;
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
			Process_PtcG2C_OnlineRewardNtf.Process(this);
		}

	}
}
