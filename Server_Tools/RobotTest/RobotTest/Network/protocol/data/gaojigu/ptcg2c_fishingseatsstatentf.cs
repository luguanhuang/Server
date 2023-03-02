using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/3/16 21:11:22

namespace XMainClient
{
    using DataType = KKSG.SeatState;

	class PtcG2C_FishingSeatsStateNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_FishingSeatsStateNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 17352;
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
			Process_PtcG2C_FishingSeatsStateNtf.Process(this);
		}

	}
}
