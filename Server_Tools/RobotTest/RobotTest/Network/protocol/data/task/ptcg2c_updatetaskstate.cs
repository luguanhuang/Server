using System;
using System.Collections.Generic;

using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/4/17 15:10:09

namespace XMainClient
{
    using DataType = KKSG.TaskState;

	class PtcG2C_UpdateTaskState : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_UpdateTaskState()
		{

		}

		public override uint GetProtoType()
		{
			return 24801;
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
			Process_PtcG2C_UpdateTaskState.Process(this);
		}

	}
}
