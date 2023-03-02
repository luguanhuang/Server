using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/26 16:27:47

namespace XMainClient
{
    using DataType = KKSG.WeddingLoadInfoNtf;

	class PtcG2C_WeddingLoadInfoNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_WeddingLoadInfoNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 61694;
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
			Process_PtcG2C_WeddingLoadInfoNtf.Process(this);
		}

	}
}
