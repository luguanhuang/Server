using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/8 20:35:51

namespace XMainClient
{
    using DataType = KKSG.ReceiveFlowerData;

	class PtcG2C_FlowerRainNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_FlowerRainNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 30604;
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
			Process_PtcG2C_FlowerRainNtf.Process(this);
		}

	}
}
