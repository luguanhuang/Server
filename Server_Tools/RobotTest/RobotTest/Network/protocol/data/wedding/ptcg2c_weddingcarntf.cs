using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/8/3 17:01:06

namespace XMainClient
{
    using DataType = KKSG.WeddingCarNotify;

	class PtcG2C_WeddingCarNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_WeddingCarNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 48301;
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
			Process_PtcG2C_WeddingCarNtf.Process(this);
		}

	}
}
