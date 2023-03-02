using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/11 19:57:20

namespace XMainClient
{
    using DataType = KKSG.PayAllInfo;

	class PtcG2C_PayAllInfoNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_PayAllInfoNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 4976;
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
			Process_PtcG2C_PayAllInfoNtf.Process(this);
		}

	}
}
