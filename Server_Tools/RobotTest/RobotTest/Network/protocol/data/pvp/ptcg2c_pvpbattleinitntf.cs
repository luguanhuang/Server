using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/23 11:32:06

namespace XMainClient
{
    using DataType = KKSG.PvpBattleInitData;

	class PtcG2C_PvpBattleInitNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_PvpBattleInitNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 44776;
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
			Process_PtcG2C_PvpBattleInitNtf.Process(this);
		}

	}
}
