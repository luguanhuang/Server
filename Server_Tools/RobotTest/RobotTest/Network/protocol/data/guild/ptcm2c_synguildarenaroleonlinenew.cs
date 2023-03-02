using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/8 15:17:24

namespace XMainClient
{
    using DataType = KKSG.SynGuildArenaRoleOnline;

	class PtcM2C_SynGuildArenaRoleOnlineNew : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_SynGuildArenaRoleOnlineNew()
		{

		}

		public override uint GetProtoType()
		{
			return 26598;
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
			Process_PtcM2C_SynGuildArenaRoleOnlineNew.Process(this);
		}

	}
}
