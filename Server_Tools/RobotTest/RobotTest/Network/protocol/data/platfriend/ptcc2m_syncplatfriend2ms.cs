using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/27 10:54:00

namespace XMainClient
{
    using DataType = KKSG.SyncPlatFriend2MSData;

	class PtcC2M_SyncPlatFriend2MS : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_SyncPlatFriend2MS()
		{

		}

		public override uint GetProtoType()
		{
			return 38885;
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
