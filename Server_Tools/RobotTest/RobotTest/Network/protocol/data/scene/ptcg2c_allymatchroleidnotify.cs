using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/3/24 16:49:56

namespace XMainClient
{
    using DataType = KKSG.AllyMatchRoleID;

	class PtcG2C_AllyMatchRoleIDNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_AllyMatchRoleIDNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 41598;
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
			Process_PtcG2C_AllyMatchRoleIDNotify.Process(this);
		}

	}
}
