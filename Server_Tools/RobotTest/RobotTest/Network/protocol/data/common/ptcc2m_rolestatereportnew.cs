using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/3/15 10:03:31

namespace XMainClient
{
    using DataType = KKSG.RoleStateReport;

	class PtcC2M_RoleStateReportNew : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_RoleStateReportNew()
		{

		}

		public override uint GetProtoType()
		{
			return 10217;
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
