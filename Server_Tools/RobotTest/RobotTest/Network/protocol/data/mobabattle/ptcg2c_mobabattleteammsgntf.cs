using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/6/10 17:13:05

namespace XMainClient
{
    using DataType = KKSG.MobaBattleTeamMsg;

	class PtcG2C_MobaBattleTeamMsgNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_MobaBattleTeamMsgNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 14987;
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
			Process_PtcG2C_MobaBattleTeamMsgNtf.Process(this);
		}

	}
}
