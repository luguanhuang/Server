using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/3 11:52:28

namespace XMainClient
{
    using DataType = KKSG.RoleStateNtf;

	class PtcM2C_RoleStateNtfNew : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_RoleStateNtfNew()
		{

		}

		public override uint GetProtoType()
		{
			return 62463;
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
			Process_PtcM2C_RoleStateNtfNew.Process(this);
		}

	}
}
