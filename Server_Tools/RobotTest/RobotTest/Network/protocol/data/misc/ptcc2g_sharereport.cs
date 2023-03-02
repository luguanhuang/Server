using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/7/31 20:31:33

namespace XMainClient
{
    using DataType = KKSG.ShareReportData;

	class PtcC2G_ShareReport : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_ShareReport()
		{

		}

		public override uint GetProtoType()
		{
			return 31884;
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
