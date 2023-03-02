using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/2/4 9:45:16

namespace XMainClient
{
    using DataType = KKSG.PokerTournamentEndReFundMsg;

	class PtcM2C_PokerTournamentEndReFund : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_PokerTournamentEndReFund()
		{

		}

		public override uint GetProtoType()
		{
			return 50590;
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
			Process_PtcM2C_PokerTournamentEndReFund.Process(this);
		}

	}
}
