﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/2 0:33:11

namespace XMainClient
{
    using DataType = KKSG.GmfWaitOtherArg;

	class PtcG2C_GmfWaitOtherLoad : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_GmfWaitOtherLoad()
		{

		}

		public override uint GetProtoType()
		{
			return 1133;
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
			Process_PtcG2C_GmfWaitOtherLoad.Process(this);
		}

	}
}
