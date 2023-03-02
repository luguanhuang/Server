using System;
using System.Collections.Generic;

using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/4/14 14:15:49

namespace XMainClient
{
    using DataType = KKSG.SkillChangedData;

	class PtcG2C_SkillChangedNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_SkillChangedNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 38872;
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
			Process_PtcG2C_SkillChangedNtf.Process(this);
		}

	}
}
