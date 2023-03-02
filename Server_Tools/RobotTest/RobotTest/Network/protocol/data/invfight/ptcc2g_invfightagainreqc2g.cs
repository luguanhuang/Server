using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/8/30 15:39:46

namespace XMainClient
{
    using DataType = KKSG.InvFightAgainPara;

	class PtcC2G_InvfightAgainReqC2G : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_InvfightAgainReqC2G()
		{

		}

		public override uint GetProtoType()
		{
			return 2055;
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
