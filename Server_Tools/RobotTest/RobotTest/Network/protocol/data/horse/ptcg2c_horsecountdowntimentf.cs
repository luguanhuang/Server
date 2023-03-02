using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/14 11:05:49

namespace XMainClient
{
    using DataType = KKSG.HorseCountDownTime;

	class PtcG2C_HorseCountDownTimeNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_HorseCountDownTimeNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 65307;
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
			Process_PtcG2C_HorseCountDownTimeNtf.Process(this);
		}

	}
}
