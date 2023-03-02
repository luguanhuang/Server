using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/19 22:29:31

namespace XMainClient
{
    using DataType = KKSG.HeroBattleTeamRoleData;

	class PtcG2C_HeroBattleTeamRoleNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_HeroBattleTeamRoleNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 25720;
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
			Process_PtcG2C_HeroBattleTeamRoleNtf.Process(this);
		}

	}
}
