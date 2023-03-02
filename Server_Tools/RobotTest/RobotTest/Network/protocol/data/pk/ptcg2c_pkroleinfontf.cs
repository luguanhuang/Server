using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/12/17 17:03:48

namespace XMainClient
{
    using DataType = KKSG.PkRoleInfoNtf;

	class PtcG2C_PkRoleInfoNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_PkRoleInfoNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 8937;
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
			Process_PtcG2C_PkRoleInfoNtf.Process(this);
		}

	}
}
