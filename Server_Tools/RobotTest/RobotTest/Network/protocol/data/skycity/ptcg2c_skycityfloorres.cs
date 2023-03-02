using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/29 16:38:29

namespace XMainClient
{
    using DataType = KKSG.SkyCityFloorInfo;

	class PtcG2C_SkyCityFloorRes : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_SkyCityFloorRes()
		{

		}

		public override uint GetProtoType()
		{
			return 35216;
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
			Process_PtcG2C_SkyCityFloorRes.Process(this);
		}

	}
}
