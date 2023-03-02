using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/13 20:25:31

namespace XMainClient
{
    using DataType = KKSG.NotifyStartUpTypeToClient;

	class PtcG2C_NotifyStartUpTypeToClient : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_NotifyStartUpTypeToClient()
		{

		}

		public override uint GetProtoType()
		{
			return 64412;
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
			Process_PtcG2C_NotifyStartUpTypeToClient.Process(this);
		}

	}
}
