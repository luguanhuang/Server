﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/18 17:28:57

namespace XMainClient
{
    using DataType = KKSG.ReceiveFlowerData;

	class PtcM2C_MSFlowerRainNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_MSFlowerRainNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 11986;
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
			Process_PtcM2C_MSFlowerRainNtf.Process(this);
		}

	}
}