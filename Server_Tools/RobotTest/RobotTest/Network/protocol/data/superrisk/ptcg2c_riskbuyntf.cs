using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/9 12:13:03

namespace XMainClient
{
    using DataType = KKSG.RiskBuyData;

	class PtcG2C_RiskBuyNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_RiskBuyNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 61237;
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
			Process_PtcG2C_RiskBuyNtf.Process(this);
		}

	}
}
