using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/6/24 15:51:35

namespace XMainClient
{
    using DataType = KKSG.CheckinInfoNotify;

	class PtcG2C_CheckinInfoNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_CheckinInfoNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 29332;
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
			Process_PtcG2C_CheckinInfoNotify.Process(this);
		}

	}
}
