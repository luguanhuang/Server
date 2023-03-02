using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/3/7 16:06:06

namespace XMainClient
{
    using DataType = KKSG.CountDownNtf;

	class PtcG2C_CountDownNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_CountDownNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 3259;
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
			Process_PtcG2C_CountDownNtf.Process(this);
		}

	}
}
