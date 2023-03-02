using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/11/26 21:23:46

namespace XMainClient
{
    using DataType = KKSG.randomtask;

	class PtcG2C_TakeRandomTask : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_TakeRandomTask()
		{

		}

		public override uint GetProtoType()
		{
			return 8442;
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
			Process_PtcG2C_TakeRandomTask.Process(this);
		}

	}
}
