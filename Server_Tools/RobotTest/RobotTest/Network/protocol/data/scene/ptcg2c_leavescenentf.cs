using System;
using System.Collections.Generic;

using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2014/10/16 16:05:41

namespace XMainClient
{
    //using DataType = KKSG.;

	class PtcG2C_LeaveSceneNtf : Protocol
	{
		//public DataType Data = new DataType();

		public PtcG2C_LeaveSceneNtf()
		{
		}

		public override uint GetProtoType()
		{
			return 33831;
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
			Process_PtcG2C_LeaveSceneNtf.Process(this);
		}

	}
}
