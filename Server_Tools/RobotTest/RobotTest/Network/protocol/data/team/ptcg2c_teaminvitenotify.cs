using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/11/30 19:32:41

namespace XMainClient
{
    using DataType = KKSG.TeamInvite;

	class PtcG2C_TeamInviteNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_TeamInviteNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 4060;
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
			Process_PtcG2C_TeamInviteNotify.Process(this);
		}

	}
}
