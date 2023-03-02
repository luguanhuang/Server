using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/25 15:42:23

namespace XMainClient
{
    using DataType = KKSG.ChatNotify;

	class PtcT2C_ChatNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcT2C_ChatNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 4256;
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
			Process_PtcT2C_ChatNotify.Process(this);
		}

	}
}
