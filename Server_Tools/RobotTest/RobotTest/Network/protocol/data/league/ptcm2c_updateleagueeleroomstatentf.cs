using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/1/14 15:52:11

namespace XMainClient
{
    using DataType = KKSG.UpdateLeagueEleRoomStateNtf;

	class PtcM2C_UpdateLeagueEleRoomStateNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_UpdateLeagueEleRoomStateNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 15800;
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
			Process_PtcM2C_UpdateLeagueEleRoomStateNtf.Process(this);
		}

	}
}
