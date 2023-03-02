using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/20 16:51:32

namespace XMainClient
{
    using DataType = KKSG.BanquetNtfArg;

	class PtcG2C_GardenBanquetNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_GardenBanquetNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 56088;
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
			Process_PtcG2C_GardenBanquetNtf.Process(this);
		}

	}
}
