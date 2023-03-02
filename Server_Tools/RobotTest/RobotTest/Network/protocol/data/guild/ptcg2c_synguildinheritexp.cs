using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/17 19:53:51

namespace XMainClient
{
    using DataType = KKSG.synGuildInheritExp;

	class PtcG2C_synGuildInheritExp : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_synGuildInheritExp()
		{

		}

		public override uint GetProtoType()
		{
			return 15872;
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
			Process_PtcG2C_synGuildInheritExp.Process(this);
		}

	}
}
