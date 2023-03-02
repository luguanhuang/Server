using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/27 14:55:37

namespace XMainClient
{
    using DataType = KKSG.WarningRandomSet;

	class PtcG2C_WarningRandomNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_WarningRandomNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 8594;
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
			Process_PtcG2C_WarningRandomNtf.Process(this);
		}

	}
}
