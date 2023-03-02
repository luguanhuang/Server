using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/26 16:49:13

namespace XMainClient
{
    using DataType = KKSG.QAIDNameList;

	class PtcG2C_QAIDNameNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_QAIDNameNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 55856;
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
			Process_PtcG2C_QAIDNameNtf.Process(this);
		}

	}
}
