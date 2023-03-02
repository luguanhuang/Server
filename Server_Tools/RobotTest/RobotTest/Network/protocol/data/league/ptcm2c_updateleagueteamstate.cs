using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/14 22:15:04

namespace XMainClient
{
    using DataType = KKSG.UpdateLeagueTeamState;

	class PtcM2C_UpdateLeagueTeamState : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_UpdateLeagueTeamState()
		{

		}

		public override uint GetProtoType()
		{
			return 7643;
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
			Process_PtcM2C_UpdateLeagueTeamState.Process(this);
		}

	}
}
