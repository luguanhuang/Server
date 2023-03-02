﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/3/10 20:48:47

namespace XMainClient
{
    using DataType = KKSG.Position;

	class PtcG2C_CorrectPosition : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_CorrectPosition()
		{

		}

		public override uint GetProtoType()
		{
			return 53665;
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
			Process_PtcG2C_CorrectPosition.Process(this);
		}

	}
}
