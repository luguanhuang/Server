using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/12 20:06:34

namespace XMainClient
{
    using DataType = KKSG.UpdateDragonGuildToClient;

	class PtcM2C_UpdateDragonGuildToClient : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_UpdateDragonGuildToClient()
		{

		}

		public override uint GetProtoType()
		{
			return 52071;
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
			Process_PtcM2C_UpdateDragonGuildToClient.Process(this);
		}

	}
}
