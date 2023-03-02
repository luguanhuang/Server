using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/23 16:46:36

namespace XMainClient
{
    using DataType = KKSG.AddAttrCount;

	class PtcG2C_NotifyGuildBossAddAttr : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_NotifyGuildBossAddAttr()
		{

		}

		public override uint GetProtoType()
		{
			return 42027;
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
			Process_PtcG2C_NotifyGuildBossAddAttr.Process(this);
		}

	}
}
