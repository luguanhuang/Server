using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XMainClient
{
    public struct Position
    {
        public float x;
        public float y;
        public float z;
    }

    class Motion
    {
        private ClientState state;
        private Position position;

        public Motion(ClientState state_)
        {
            state = state_;
        }

        public void SetPosition(KKSG.Vec3 v)
        {
            position.x = v.x;
            position.y = v.y;
            position.z = v.z;
        }

        public void MoveTo(double x, double z)
        {
            double dx = x - position.x;
            double dz = z - position.z;
            double dir = Math.Atan2(dz, dx);
            if (dir < 0) dir += 2 * Math.PI;
            double angle = 450 - dir * 360 / 2 / Math.PI;
            if (angle > 360) angle -= 360;

            PtcC2G_MoveOperationReq movePtc = new PtcC2G_MoveOperationReq();
            movePtc.Data.Direction = (int)(10 * angle);
            movePtc.Data.Velocity = 50;
            movePtc.Data.PosX = (int)(position.x * 100);
            movePtc.Data.PosY = (int)(position.y * 100);
            movePtc.Data.PosZ = (int)(position.z * 100);
            movePtc.Data.DesX = (int)(x * 100);
            movePtc.Data.DesY = (int)(position.y * 100);
            movePtc.Data.DesZ = (int)(z * 100);
            movePtc.Data.Inertia = true;
            //movePtc.Data.Time = state.GetServerTime();
            state.Send(movePtc);

            //Console.WriteLine("Move From {0:F2}, {1:F2} to {2:F2}, {3:F2}", position.x, position.z, x, z);

            double ds = Math.Sqrt(dx * dx + dz * dz);
            position.x = (float)(x + 2.0 * dx / ds);
            position.z = (float)(z + 2.0 * dz / ds);
        }
    }
}
