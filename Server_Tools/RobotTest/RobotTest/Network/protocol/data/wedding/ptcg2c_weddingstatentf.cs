using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/26 16:29:25

namespace XMainClient
{
    using DataType = KKSG.WeddingStateNtf;

	class PtcG2C_WeddingStateNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_WeddingStateNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 30976;
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
			Process_PtcG2C_WeddingStateNtf.Process(this);
		}

	}
}
