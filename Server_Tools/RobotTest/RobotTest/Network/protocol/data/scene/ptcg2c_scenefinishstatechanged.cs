using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ProtoBuf;

// generate by ProtoGen at date: 2015/12/16 16:44:55

namespace XMainClient
{
    using DataType = KKSG.FinishStateInfo;

	class PtcG2C_SceneFinishStateChanged : Protocol
	{
		public DataType Data = new DataType();

		public PtcG2C_SceneFinishStateChanged()
		{

		}

		public override uint GetProtoType()
		{
			return 60400;
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
			Process_PtcG2C_SceneFinishStateChanged.Process(this);
		}

	}
}
