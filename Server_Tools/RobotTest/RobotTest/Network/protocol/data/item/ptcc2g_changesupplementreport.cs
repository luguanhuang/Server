using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/1/18 17:24:47

namespace XMainClient
{
    using DataType = KKSG.ChangeSupplementReport;

	class PtcC2G_ChangeSupplementReport : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_ChangeSupplementReport()
		{

		}

		public override uint GetProtoType()
		{
			return 42193;
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
