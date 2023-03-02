using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/5 17:11:03

namespace XMainClient
{
    using DataType = KKSG.PayMemberPrivilege;

	class PtcG2C_PayMemberPrivilegeNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_PayMemberPrivilegeNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 33306;
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
			Process_PtcG2C_PayMemberPrivilegeNtf.Process(this);
		}

	}
}
