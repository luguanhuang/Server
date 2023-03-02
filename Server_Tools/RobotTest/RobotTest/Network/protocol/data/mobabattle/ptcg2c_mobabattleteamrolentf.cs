using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/6/9 16:25:34

namespace XMainClient
{
    using DataType = KKSG.MobaBattleTeamRoleData;

	class PtcG2C_MobaBattleTeamRoleNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_MobaBattleTeamRoleNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 44930;
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
			Process_PtcG2C_MobaBattleTeamRoleNtf.Process(this);
		}

	}
}
