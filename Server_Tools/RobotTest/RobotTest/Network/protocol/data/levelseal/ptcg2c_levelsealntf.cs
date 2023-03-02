using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/16 14:25:26

namespace XMainClient
{
    using DataType = KKSG.LevelSealInfo;

	class PtcG2C_LevelSealNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_LevelSealNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 40338;
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
			Process_PtcG2C_LevelSealNtf.Process(this);
		}

	}
}
