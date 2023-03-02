using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/19 14:27:58

namespace XMainClient
{
    using DataType = KKSG.RoleStateMatch;

	class PtcM2C_RoleMatchStateM2CNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_RoleMatchStateM2CNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 11521;
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
			Process_PtcM2C_RoleMatchStateM2CNotify.Process(this);
		}

	}
}
