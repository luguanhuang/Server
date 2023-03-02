using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/3/15 10:57:44

namespace XMainClient
{
    using DataType = KKSG.HintNotify;

	class PtcG2C_HintNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_HintNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 23114;
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
			Process_PtcG2C_HintNotify.Process(this);
		}

	}
}
