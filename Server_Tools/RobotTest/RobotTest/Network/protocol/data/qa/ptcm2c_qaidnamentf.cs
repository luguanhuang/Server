using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/10/9 11:11:33

namespace XMainClient
{
    using DataType = KKSG.QAIDNameList;

	class PtcM2C_QAIDNameNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_QAIDNameNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 987;
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
			Process_PtcM2C_QAIDNameNtf.Process(this);
		}

	}
}
