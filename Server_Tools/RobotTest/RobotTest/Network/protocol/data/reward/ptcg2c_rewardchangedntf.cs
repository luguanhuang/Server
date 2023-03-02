using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/7/9 20:54:38

namespace XMainClient
{
    using DataType = KKSG.RewardChanged;

	class PtcG2C_RewardChangedNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_RewardChangedNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 57873;
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
			Process_PtcG2C_RewardChangedNtf.Process(this);
		}

	}
}
