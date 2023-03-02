using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/8 20:33:56

namespace XMainClient
{
    using DataType = KKSG.ReceiveFlowerData;

	class PtcG2C_ReceiveFlowerNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_ReceiveFlowerNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 43606;
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
			Process_PtcG2C_ReceiveFlowerNtf.Process(this);
		}

	}
}
