using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/16 20:58:08

namespace XMainClient
{
    using DataType = KKSG.ScenePrepareInfoNtf;

	class PtcG2C_ScenePrepareInfoNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_ScenePrepareInfoNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 65478;
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
			Process_PtcG2C_ScenePrepareInfoNtf.Process(this);
		}

	}
}
