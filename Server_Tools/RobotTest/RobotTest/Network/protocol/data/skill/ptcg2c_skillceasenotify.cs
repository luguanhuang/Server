using System;
using System.Collections.Generic;

using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2014/12/10 17:00:36

namespace XMainClient
{
    using DataType = KKSG.SkillCeaseInfo;

	class PtcG2C_SkillCeaseNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_SkillCeaseNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 21566;
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
			Process_PtcG2C_SkillCeaseNotify.Process(this);
		}

	}
}
