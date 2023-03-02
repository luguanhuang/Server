using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/8 14:55:23

namespace XMainClient
{
    using DataType = KKSG.ResWarFinalAll;

	class PtcM2C_ResWarFinalAllNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_ResWarFinalAllNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 35696;
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
			Process_PtcM2C_ResWarFinalAllNtf.Process(this);
		}

	}
}
