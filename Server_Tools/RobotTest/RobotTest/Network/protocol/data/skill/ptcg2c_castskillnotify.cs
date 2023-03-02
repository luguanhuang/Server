using System;
using System.Collections.Generic;

using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2014/12/15 10:08:30

namespace XMainClient
{
    using DataType = KKSG.SkillDataUnit;

	class PtcG2C_CastSkillNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_CastSkillNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 25922;
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
			Process_PtcG2C_CastSkillNotify.Process(this);
		}

	}
}
