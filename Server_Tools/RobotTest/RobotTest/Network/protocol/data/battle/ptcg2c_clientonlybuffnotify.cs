using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/27 21:26:02

namespace XMainClient
{
    using DataType = KKSG.BuffList;

	class PtcG2C_ClientOnlyBuffNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_ClientOnlyBuffNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 35149;
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
			Process_PtcG2C_ClientOnlyBuffNotify.Process(this);
		}

	}
}
