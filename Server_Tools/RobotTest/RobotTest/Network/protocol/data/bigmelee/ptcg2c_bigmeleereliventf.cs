﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/6/22 8:16:46

namespace XMainClient
{
    using DataType = KKSG.BigMeleeRelive;

	class PtcG2C_BigMeleeReliveNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_BigMeleeReliveNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 3358;
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
			Process_PtcG2C_BigMeleeReliveNtf.Process(this);
		}

	}
}
