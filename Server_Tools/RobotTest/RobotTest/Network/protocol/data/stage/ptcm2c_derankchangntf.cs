using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/9/19 12:42:33

namespace XMainClient
{
    using DataType = KKSG.DERankChangePara;

	class PtcM2C_DERankChangNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_DERankChangNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 11404;
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
			Process_PtcM2C_DERankChangNtf.Process(this);
		}

	}
}
