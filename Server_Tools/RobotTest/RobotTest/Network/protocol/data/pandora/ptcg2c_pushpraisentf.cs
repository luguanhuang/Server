using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/9 22:40:20

namespace XMainClient
{
    using DataType = KKSG.PushPraise;

	class PtcG2C_PushPraiseNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_PushPraiseNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 5686;
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
			Process_PtcG2C_PushPraiseNtf.Process(this);
		}

	}
}
