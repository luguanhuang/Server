using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/30 13:18:54

namespace XMainClient
{
    using DataType = KKSG.NotifyTransferWall;

	class PtcG2C_OnTransferWall : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_OnTransferWall()
		{

		}

		public override uint GetProtoType()
		{
			return 37585;
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
			Process_PtcG2C_OnTransferWall.Process(this);
		}

	}
}
