using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/28 9:59:30

namespace XMainClient
{
    using DataType = KKSG.QARoomRankNtf;

	class PtcG2C_QARoomRankNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_QARoomRankNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 51016;
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
			Process_PtcG2C_QARoomRankNtf.Process(this);
		}

	}
}
