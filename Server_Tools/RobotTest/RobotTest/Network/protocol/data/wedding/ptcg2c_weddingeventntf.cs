using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/26 16:41:58

namespace XMainClient
{
    using DataType = KKSG.WeddingEventNtf;

	class PtcG2C_WeddingEventNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_WeddingEventNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 51472;
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
			Process_PtcG2C_WeddingEventNtf.Process(this);
		}

	}
}
