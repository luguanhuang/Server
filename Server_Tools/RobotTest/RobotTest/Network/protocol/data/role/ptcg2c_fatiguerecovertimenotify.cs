﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/7/30 15:18:22

namespace XMainClient
{
    using DataType = KKSG.FatigueRecoverTimeInfo;

	class PtcG2C_FatigueRecoverTimeNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_FatigueRecoverTimeNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 14296;
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
			Process_PtcG2C_FatigueRecoverTimeNotify.Process(this);
		}

	}
}
