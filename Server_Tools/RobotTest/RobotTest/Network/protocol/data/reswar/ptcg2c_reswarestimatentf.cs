using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/1 19:41:23

namespace XMainClient
{
    using DataType = KKSG.ResWarEstimateInfo;

	class PtcG2C_ResWarEstimateNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_ResWarEstimateNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 53855;
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
			Process_PtcG2C_ResWarEstimateNtf.Process(this);
		}

	}
}
