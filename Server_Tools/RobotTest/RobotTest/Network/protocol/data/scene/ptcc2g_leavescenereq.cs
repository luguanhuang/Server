using System;
using System.Collections.Generic;

using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2014/10/16 11:11:07

namespace XMainClient
{
    //using DataType = KKSG.;

	class PtcC2G_LeaveSceneReq : Protocol
	{
		//public DataType Data = new DataType();

		public PtcC2G_LeaveSceneReq()
		{
		}

		public override uint GetProtoType()
		{
			return 27927;
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
			throw new Exception("Send only protocol can not call process");
		}

	}
}
