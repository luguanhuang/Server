﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/5 22:58:11

namespace XMainClient
{
    using DataType = KKSG.NotifyLeagueTeamCreate;

	class PtcM2C_NotifyLeagueTeamCreate : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_NotifyLeagueTeamCreate()
		{

		}

		public override uint GetProtoType()
		{
			return 22343;
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
			Process_PtcM2C_NotifyLeagueTeamCreate.Process(this);
		}

	}
}
