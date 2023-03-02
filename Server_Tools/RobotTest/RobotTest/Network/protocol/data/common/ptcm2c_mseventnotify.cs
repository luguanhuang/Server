using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/29 22:13:30

namespace XMainClient
{
    using DataType = KKSG.EventNotify;

	class PtcM2C_MSEventNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_MSEventNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 1415;
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
			Process_PtcM2C_MSEventNotify.Process(this);
		}

	}
}
