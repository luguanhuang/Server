using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/9/14 15:23:58

namespace XMainClient
{
    using DataType = KKSG.FashionChangedData;

	class PtcG2C_FashoinChangedNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_FashoinChangedNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 12350;
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
			Process_PtcG2C_FashoinChangedNtf.Process(this);
		}

	}
}
