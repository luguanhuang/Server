using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/27 20:32:17

namespace XMainClient
{
    using DataType = KKSG.SynGuildArenaRoleOnline;

	class PtcG2C_SynGuildArenaRoleOnline : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_SynGuildArenaRoleOnline()
		{

		}

		public override uint GetProtoType()
		{
			return 48528;
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
			Process_PtcG2C_SynGuildArenaRoleOnline.Process(this);
		}

	}
}
