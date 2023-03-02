using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/29 11:01:01

namespace XMainClient
{
    using DataType = KKSG.GmfGuildCombatPara;

	class PtcG2C_GmfGuildCombatNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_GmfGuildCombatNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 55102;
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
			Process_PtcG2C_GmfGuildCombatNtf.Process(this);
		}

	}
}
