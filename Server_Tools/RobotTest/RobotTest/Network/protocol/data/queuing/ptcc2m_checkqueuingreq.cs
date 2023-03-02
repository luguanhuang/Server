using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/23 14:28:52

namespace XMainClient
{
    using DataType = KKSG.CheckQueuingReq;

	class PtcC2M_CheckQueuingReq : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_CheckQueuingReq()
		{

		}

		public override uint GetProtoType()
		{
			return 28232;
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
