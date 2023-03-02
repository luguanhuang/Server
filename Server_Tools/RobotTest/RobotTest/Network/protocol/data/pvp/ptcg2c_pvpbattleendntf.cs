using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/23 11:41:30

namespace XMainClient
{
    using DataType = KKSG.PvpBattleEndData;

	class PtcG2C_PvpBattleEndNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_PvpBattleEndNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 46438;
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
			Process_PtcG2C_PvpBattleEndNtf.Process(this);
		}

	}
}
