﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/10/27 12:07:55

namespace XMainClient
{
    using DataType = KKSG.WorldLevel;

	class PtcG2C_WorldLevelNtf2Client : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_WorldLevelNtf2Client()
		{

		}

		public override uint GetProtoType()
		{
			return 63449;
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
			Process_PtcG2C_WorldLevelNtf2Client.Process(this);
		}

	}
}
