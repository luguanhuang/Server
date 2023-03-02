using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/11 17:59:51

namespace XMainClient
{
    using DataType = KKSG.CancelRedDot;

	class PtcC2G_CancelRedDotReq : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_CancelRedDotReq()
		{

		}

		public override uint GetProtoType()
		{
			return 40873;
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
			throw new Exception("Send only protocol can not call process");
		}

	}
}
