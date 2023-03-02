using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/27 11:40:39

namespace XMainClient
{
    using DataType = KKSG.GardenBanquetNtf;

	class PtcG2C_GardenBanquetNotice : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_GardenBanquetNotice()
		{

		}

		public override uint GetProtoType()
		{
			return 36929;
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
			Process_PtcG2C_GardenBanquetNotice.Process(this);
		}

	}
}
