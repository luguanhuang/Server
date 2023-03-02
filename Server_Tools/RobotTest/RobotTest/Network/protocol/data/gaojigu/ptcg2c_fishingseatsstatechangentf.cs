using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/3/16 21:08:38

namespace XMainClient
{
    //using DataType = KKSG.;

	class PtcG2C_FishingSeatsStateChangeNtf : Protocol
	{
		//public DataType Data = new DataType();

		public PtcG2C_FishingSeatsStateChangeNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 64695;
		}

		public override void Serialize(MemoryStream stream)
		{
			//Serializer.Serialize(stream, Data);
		}

		public override void DeSerialize(MemoryStream stream)
		{
			//Data = Serializer.Deserialize<DataType>(stream);
		}

		public override void Process()
		{
			Process_PtcG2C_FishingSeatsStateChangeNtf.Process(this);
		}

	}
}
