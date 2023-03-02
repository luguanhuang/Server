using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/19 19:53:09

namespace XMainClient
{
    using DataType = KKSG.DailyTaskEventNtf;

	class PtcM2C_DailyTaskEventNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_DailyTaskEventNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 26376;
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
			Process_PtcM2C_DailyTaskEventNtf.Process(this);
		}

	}
}
