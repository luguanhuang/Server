﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/18 16:01:13

namespace XMainClient
{
    using DataType = KKSG.GuildCheckinBoxNtf;

	class PtcM2C_NotifyGuildCheckinProcess : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_NotifyGuildCheckinProcess()
		{

		}

		public override uint GetProtoType()
		{
			return 34704;
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
			Process_PtcM2C_NotifyGuildCheckinProcess.Process(this);
		}

	}
}
