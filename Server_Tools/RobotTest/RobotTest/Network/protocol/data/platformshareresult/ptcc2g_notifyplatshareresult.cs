﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/5/31 21:33:22

namespace XMainClient
{
    using DataType = KKSG.NotifyPlatShareResultArg;

	class PtcC2G_NotifyPlatShareResult : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2G_NotifyPlatShareResult()
		{

		}

		public override uint GetProtoType()
		{
			return 8480;
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
			throw new Exception("Send only protocol can not call process");
		}

	}
}
