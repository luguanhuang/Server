using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/6 20:52:41

namespace XMainClient
{
    using DataType = KKSG.HorseWaitTime;

	class PtcG2C_HorseWaitTimeNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_HorseWaitTimeNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 34138;
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
			Process_PtcG2C_HorseWaitTimeNtf.Process(this);
		}

	}
}
