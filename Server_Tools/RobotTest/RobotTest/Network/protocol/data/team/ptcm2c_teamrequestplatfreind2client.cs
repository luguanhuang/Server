﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/4 20:50:55

namespace XMainClient
{
    using DataType = KKSG.TeamRequestPlatFreind2ClientData;

	class PtcM2C_TeamRequestPlatFreind2Client : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_TeamRequestPlatFreind2Client()
		{

		}

		public override uint GetProtoType()
		{
			return 37841;
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
			Process_PtcM2C_TeamRequestPlatFreind2Client.Process(this);
		}

	}
}
