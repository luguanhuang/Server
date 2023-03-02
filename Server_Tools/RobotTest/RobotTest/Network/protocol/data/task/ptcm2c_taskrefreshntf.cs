using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/10/12 14:11:57

namespace XMainClient
{
    using DataType = KKSG.TaskRefreshNtf;

	class PtcM2C_TaskRefreshNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_TaskRefreshNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 40464;
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
			Process_PtcM2C_TaskRefreshNtf.Process(this);
		}

	}
}
