using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/12/14 23:25:15

namespace XMainClient
{
    using DataType = KKSG.GCFZhanLingPara;

	class PtcG2C_GCFZhanLingNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_GCFZhanLingNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 14402;
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
			Process_PtcG2C_GCFZhanLingNotify.Process(this);
		}

	}
}
