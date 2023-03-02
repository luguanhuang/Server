﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/3/15 10:57:46

namespace XMainClient
{
    using DataType = KKSG.HintNotify;

	class PtcM2C_HintNotifyMS : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_HintNotifyMS()
		{

		}

		public override uint GetProtoType()
		{
			return 15542;
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
			Process_PtcM2C_HintNotifyMS.Process(this);
		}

	}
}
