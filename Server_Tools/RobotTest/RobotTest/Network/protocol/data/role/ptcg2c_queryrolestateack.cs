﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/28 11:10:45

namespace XMainClient
{
    using DataType = KKSG.QueryRoleStateAck;

	class PtcG2C_QueryRoleStateAck : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_QueryRoleStateAck()
		{

		}

		public override uint GetProtoType()
		{
			return 53402;
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
			Process_PtcG2C_QueryRoleStateAck.Process(this);
		}

	}
}
