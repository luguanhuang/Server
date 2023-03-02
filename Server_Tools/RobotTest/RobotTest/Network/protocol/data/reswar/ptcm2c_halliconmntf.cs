﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/16 23:07:52

namespace XMainClient
{
    using DataType = KKSG.HallIconPara;

	class PtcM2C_HallIconMNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_HallIconMNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 51500;
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
			Process_PtcM2C_HallIconMNtf.Process(this);
		}

	}
}
