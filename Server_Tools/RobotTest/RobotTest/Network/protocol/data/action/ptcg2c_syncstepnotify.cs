﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/12 22:48:06

namespace XMainClient
{
    using DataType = KKSG.StepSyncInfo;

	class PtcG2C_SyncStepNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_SyncStepNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 37999;
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
			Process_PtcG2C_SyncStepNotify.Process(this);
		}

	}
}
