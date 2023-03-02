﻿using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/18 14:23:53

namespace XMainClient
{
    using DataType = KKSG.TeamInviteAck;

	class PtcC2M_TeamInviteAckC2M : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_TeamInviteAckC2M()
		{

		}

		public override uint GetProtoType()
		{
			return 15365;
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
			throw new Exception("Send only protocol can not call process");
		}

	}
}
