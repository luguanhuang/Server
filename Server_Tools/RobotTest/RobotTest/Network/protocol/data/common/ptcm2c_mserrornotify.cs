using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/18 14:17:41

namespace XMainClient
{
    using DataType = KKSG.ErrorInfo;

	class PtcM2C_MSErrorNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcM2C_MSErrorNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 48740;
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
			Process_PtcM2C_MSErrorNotify.Process(this);
		}

	}
}
