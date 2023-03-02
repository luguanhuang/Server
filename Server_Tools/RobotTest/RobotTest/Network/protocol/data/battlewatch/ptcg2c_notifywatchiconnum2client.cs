using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2016/8/2 10:34:27

namespace XMainClient
{
    using DataType = KKSG.IconWatchListNum;

	class PtcG2C_NotifyWatchIconNum2Client : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_NotifyWatchIconNum2Client()
		{

		}

		public override uint GetProtoType()
		{
			return 48952;
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
			Process_PtcG2C_NotifyWatchIconNum2Client.Process(this);
		}

	}
}
