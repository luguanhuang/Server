using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/19 10:25:32

namespace XMainClient
{
    using DataType = KKSG.IdipMessage;

	class PtcG2C_NotifyIdipMessageGs : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_NotifyIdipMessageGs()
		{

		}

		public override uint GetProtoType()
		{
			return 59353;
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
			Process_PtcG2C_NotifyIdipMessageGs.Process(this);
		}

	}
}
