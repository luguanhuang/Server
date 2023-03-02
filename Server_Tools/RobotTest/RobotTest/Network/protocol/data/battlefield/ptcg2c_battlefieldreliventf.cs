using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/9/4 16:20:19

namespace XMainClient
{
    using DataType = KKSG.BattleFieldReliveInfo;

	class PtcG2C_BattleFieldReliveNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_BattleFieldReliveNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 813;
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
			Process_PtcG2C_BattleFieldReliveNtf.Process(this);
		}

	}
}
