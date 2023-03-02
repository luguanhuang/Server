using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/1 21:27:08

namespace XMainClient
{
    using DataType = KKSG.SkyCityKillerInfo;

	class PtcG2C_SkyCityKillerRes : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_SkyCityKillerRes()
		{

		}

		public override uint GetProtoType()
		{
			return 1030;
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
			Process_PtcG2C_SkyCityKillerRes.Process(this);
		}

	}
}
