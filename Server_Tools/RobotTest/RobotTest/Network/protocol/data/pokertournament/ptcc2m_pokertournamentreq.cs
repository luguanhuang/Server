using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/10 21:11:26

namespace XMainClient
{
    using DataType = KKSG.GuildCardMatchReq;

	class PtcC2M_PokerTournamentReq : Protocol
	{
		public DataType Data = new DataType();

		public PtcC2M_PokerTournamentReq()
		{

		}

		public override uint GetProtoType()
		{
			return 3685;
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
