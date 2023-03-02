using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/10/10 15:04:06

namespace XMainClient
{
    using DataType = KKSG.BigMeleePoint;

	class PtcG2C_BigMeleePointNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_BigMeleePointNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 15624;
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
			Process_PtcG2C_BigMeleePointNtf.Process(this);
		}

	}
}
