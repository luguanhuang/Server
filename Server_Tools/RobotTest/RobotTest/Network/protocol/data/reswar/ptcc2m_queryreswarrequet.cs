using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/11 21:12:55

namespace XMainClient
{
    using DataType = KKSG.QueryResWarArg;

	class PtcC2M_QueryResWarRequet : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_QueryResWarRequet()
		{

		}

		public override uint GetProtoType()
		{
			return 53580;
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
