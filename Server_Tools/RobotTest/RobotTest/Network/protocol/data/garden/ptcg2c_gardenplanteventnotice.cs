using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/22 9:51:11

namespace XMainClient
{
    using DataType = KKSG.GardenPlantEventNoticeArg;

	class PtcG2C_GardenPlantEventNotice : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_GardenPlantEventNotice()
		{

		}

		public override uint GetProtoType()
		{
			return 60686;
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
			Process_PtcG2C_GardenPlantEventNotice.Process(this);
		}

	}
}
