using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/23 21:13:13

namespace XMainClient
{
    using DataType = KKSG.FriendOpNotify;

	class PtcG2C_FriendOpNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_FriendOpNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 8170;
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
			Process_PtcG2C_FriendOpNotify.Process(this);
		}

	}
}
