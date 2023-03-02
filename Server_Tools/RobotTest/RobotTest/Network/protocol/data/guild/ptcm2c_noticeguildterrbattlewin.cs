using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/2/9 11:07:49

namespace XMainClient
{
    using DataType = KKSG.NoticeGuildTerrBattleWin;

	class PtcM2C_NoticeGuildTerrBattleWin : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_NoticeGuildTerrBattleWin()
		{

		}

		public override uint GetProtoType()
		{
			return 61655;
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
			Process_PtcM2C_NoticeGuildTerrBattleWin.Process(this);
		}

	}
}
