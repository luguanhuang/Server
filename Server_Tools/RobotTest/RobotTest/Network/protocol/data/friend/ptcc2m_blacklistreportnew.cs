using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/29 14:26:00

namespace XMainClient
{
    using DataType = KKSG.BlackListReport;

	class PtcC2M_BlackListReportNew : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_BlackListReportNew()
		{

		}

		public override uint GetProtoType()
		{
			return 57057;
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
