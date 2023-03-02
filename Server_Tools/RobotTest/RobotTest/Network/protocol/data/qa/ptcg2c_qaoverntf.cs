using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/28 9:58:06

namespace XMainClient
{
    using DataType = KKSG.QAOverNtf;

	class PtcG2C_QAOverNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_QAOverNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 37952;
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
			Process_PtcG2C_QAOverNtf.Process(this);
		}

	}
}
