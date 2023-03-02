using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/7 15:47:16

namespace XMainClient
{
    using DataType = KKSG.PayScoreData;

	class PtcG2C_PayScoreNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_PayScoreNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 61859;
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
			Process_PtcG2C_PayScoreNtf.Process(this);
		}

	}
}
