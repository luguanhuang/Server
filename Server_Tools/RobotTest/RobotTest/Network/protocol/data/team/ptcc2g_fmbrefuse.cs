using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/4 11:50:47

namespace XMainClient
{
    using DataType = KKSG.FMBRes;

	class PtcC2G_FMBRefuse : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_FMBRefuse()
		{

		}

		public override uint GetProtoType()
		{
			return 50821;
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
