using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/12/11 14:37:59

namespace XMainClient
{
    using DataType = KKSG.PkTimeoutNtf;

	class PtcG2C_PkTimeoutNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_PkTimeoutNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 58692;
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
			Process_PtcG2C_PkTimeoutNtf.Process(this);
		}

	}
}
