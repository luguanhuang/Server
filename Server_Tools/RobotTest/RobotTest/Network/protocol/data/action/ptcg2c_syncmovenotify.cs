using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/28 16:17:18

namespace XMainClient
{
    using DataType = KKSG.StepMoveData;

	class PtcG2C_SyncMoveNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_SyncMoveNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 32838;
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
			Process_PtcG2C_SyncMoveNotify.Process(this);
		}

	}
}
