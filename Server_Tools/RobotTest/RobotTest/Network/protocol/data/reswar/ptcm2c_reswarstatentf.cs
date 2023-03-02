using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/11/8 17:53:54

namespace XMainClient
{
    using DataType = KKSG.ResWarStateInfo;

	class PtcM2C_ResWarStateNtf : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_ResWarStateNtf()
		{

		}

		public override uint GetProtoType()
		{
			return 18481;
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
			Process_PtcM2C_ResWarStateNtf.Process(this);
		}

	}
}
