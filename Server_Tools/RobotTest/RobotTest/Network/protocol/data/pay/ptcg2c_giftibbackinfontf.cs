﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/3/14 14:09:51

namespace XMainClient
{
    using DataType = KKSG.GiftIBBackInfo;

	class PtcG2C_GiftIBBackInfoNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_GiftIBBackInfoNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 6953;
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
			Process_PtcG2C_GiftIBBackInfoNtf.Process(this);
		}

	}
}
