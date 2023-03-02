using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/2 0:34:01

namespace XMainClient
{
    using DataType = KKSG.GmfWaitFightArg;

	class PtcG2C_GmfWaitFightBegin : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_GmfWaitFightBegin()
		{

		}

		public override uint GetProtoType()
		{
			return 59721;
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
			Process_PtcG2C_GmfWaitFightBegin.Process(this);
		}

	}
}
