using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/4/13 15:53:44

namespace XMainClient
{
    using DataType = KKSG.RoleStateMatch;

	class PtcG2C_RoleMatchStateNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_RoleMatchStateNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 47999;
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
			Process_PtcG2C_RoleMatchStateNotify.Process(this);
		}

	}
}
