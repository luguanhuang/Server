using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/20 17:04:26

namespace XMainClient
{
    using DataType = KKSG.GmfJoinBattleArg;

	class PtcM2C_GmfJoinBattleM2CReq : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_GmfJoinBattleM2CReq()
		{

		}

		public override uint GetProtoType()
		{
			return 63969;
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
			Process_PtcM2C_GmfJoinBattleM2CReq.Process(this);
		}

	}
}
