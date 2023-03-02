using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/26 19:49:00

namespace XMainClient
{
    using DataType = KKSG.GardenFishStopArg;

	class PtcC2M_GardenFishStop : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_GardenFishStop()
		{

		}

		public override uint GetProtoType()
		{
			return 56656;
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
			throw new Exception("Send only protocol can not call process");
		}

	}
}
