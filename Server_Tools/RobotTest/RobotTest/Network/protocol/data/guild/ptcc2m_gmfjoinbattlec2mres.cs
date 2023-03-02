using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/20 17:04:52

namespace XMainClient
{
    using DataType = KKSG.GmfJoinBattleRes;

	class PtcC2M_GmfJoinBattleC2MRes : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_GmfJoinBattleC2MRes()
		{

		}

		public override uint GetProtoType()
		{
			return 25047;
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
