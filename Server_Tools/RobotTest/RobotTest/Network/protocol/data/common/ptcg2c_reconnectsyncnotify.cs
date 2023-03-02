using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/6/16 14:25:28

namespace XMainClient
{
    using DataType = KKSG.ReconectSync;

	class PtcG2C_ReconnectSyncNotify : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_ReconnectSyncNotify()
		{

		}

		public override uint GetProtoType()
		{
			return 42128;
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
			Process_PtcG2C_ReconnectSyncNotify.Process(this);
		}

	}
}
