using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/27 11:57:21

namespace XMainClient
{
    using DataType = KKSG.BanquetNtfArg;

	class PtcM2C_GardenBanquetNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_GardenBanquetNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 21287;
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
			Process_PtcM2C_GardenBanquetNtf.Process(this);
		}

	}
}
