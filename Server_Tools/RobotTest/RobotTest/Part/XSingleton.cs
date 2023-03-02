using System;

namespace XUtliPoolLib
{
    public abstract class XBaseSingleton
    {
        public virtual bool Init() { return true; }
        public virtual void Uninit() { }
    }

    public class XSingleton<T> : XBaseSingleton where T : new()
    {
        protected XSingleton()
        {
            /* Here we added restriction in case a totally new Object of T be created outside.
             * It is not very delicate but works.
             * Also we can use reflection to get ctor. of T in a usual way, But
             * it's may cost more performance.
             */

            if (null != _instance)
            {
                //barely goes here...
                //Type type = _instance.GetType();
                //throw new XDoubleNewException(_instance.ToString() + @" can not be created again.");
            }
        }

        private static readonly T _instance = new T();

        public static T singleton
        {
            get
            {
                return _instance;
            }
        }
    }
}
