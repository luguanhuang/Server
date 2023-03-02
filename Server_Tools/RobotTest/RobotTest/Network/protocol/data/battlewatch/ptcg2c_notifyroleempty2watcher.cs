using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/7/21 15:51:34

namespace XMainClient
{
    //using DataType = KKSG.;

	class PtcG2C_NotifyRoleEmpty2Watcher : Protocol
	{
		//public DataType Data = new DataType();

		public PtcG2C_NotifyRoleEmpty2Watcher()
		{

		}

		public override uint GetProtoType()
		{
			return 1540;
		}

		public override void Serialize(MemoryStream stream)
		{
			//Serializer.Serialize(stream, Data);
		}

		public override void DeSerialize(MemoryStream stream)
		{
			//Data = Serializer.Deserialize<DataType>(stream);
		}

		public override void Process()
		{
			Process_PtcG2C_NotifyRoleEmpty2Watcher.Process(this);
		}

	}
}
