using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/2/25 14:13:02

namespace XMainClient
{
    using DataType = KKSG.QTEOperation;

	class PtcC2G_QTEOperation : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_QTEOperation()
		{

		}

		public override uint GetProtoType()
		{
			return 11413;
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
