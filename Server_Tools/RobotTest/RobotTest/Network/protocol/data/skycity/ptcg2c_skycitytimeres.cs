using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/29 19:17:45

namespace XMainClient
{
    using DataType = KKSG.SkyCityTimeInfo;

	class PtcG2C_SkyCityTimeRes : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_SkyCityTimeRes()
		{

		}

		public override uint GetProtoType()
		{
			return 30724;
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
			Process_PtcG2C_SkyCityTimeRes.Process(this);
		}

	}
}
