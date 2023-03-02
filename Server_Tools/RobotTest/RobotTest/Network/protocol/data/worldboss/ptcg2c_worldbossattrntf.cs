using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/9 0:42:05

namespace XMainClient
{
    using DataType = KKSG.WorldBossAttrNtf;

	class PtcG2C_WorldBossAttrNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_WorldBossAttrNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 31578;
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
			Process_PtcG2C_WorldBossAttrNtf.Process(this);
		}

	}
}
