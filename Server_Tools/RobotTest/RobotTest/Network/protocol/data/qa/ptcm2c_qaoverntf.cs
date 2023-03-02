using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2017/2/7 3:14:18

namespace XMainClient
{
    using DataType = KKSG.QAOverNtf;

	class PtcM2C_QAOverNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_QAOverNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 29361;
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
			Process_PtcM2C_QAOverNtf.Process(this);
		}

	}
}
