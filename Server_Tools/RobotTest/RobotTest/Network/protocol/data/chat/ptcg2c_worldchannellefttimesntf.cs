using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/25 12:08:58

namespace XMainClient
{
    using DataType = KKSG.WorldChannelLeftTimesNtf;

	class PtcG2C_WorldChannelLeftTimesNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_WorldChannelLeftTimesNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 37503;
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
			Process_PtcG2C_WorldChannelLeftTimesNtf.Process(this);
		}

	}
}
