using System;
using System.Collections.Generic;
using UnityEngine;

namespace XUtliPoolLib
{
    public sealed class XTimerMgr : XSingleton<XTimerMgr>
    {
        public delegate void ElapsedEventHandler(object param);
        public delegate void AccurateElapsedEventHandler(object param, float delay);
        public delegate void ElapsedIDEventHandler(object param, int id);
        private sealed class XTimer : IComparable<XTimer>, IHere
        {
            private double _triggerTime;
            private object _param;
            private object _handler;

            private bool _global = false;
            private uint _token = 0;

            public XTimer(double trigger, object handler, object parma, uint token, bool global, int id)
            {
                Refine(trigger, handler, parma, token, global, id);
            }

            public void Refine(double trigger, object handler, object parma, uint token, bool global, int id)
            {
                _triggerTime = trigger;

                _handler = handler;
                _param = parma;
                _global = global;

                _token = token;

                Here = -1;
                IsInPool = false;
                Id = id;
            }
            public void Refine(double trigger)
            {
                _triggerTime = trigger;
            }

            public double TriggerTime { get { return _triggerTime; } }

            public bool IsGlobaled { get { return _global; } }
            public bool IsInPool { get; set; }

            public uint Token { get { return _token; } }
            public int Here { get; set; }

            public int Id { get; set; }

            public void Fire(float delta)
            {
                if (_handler is AccurateElapsedEventHandler)
                {
                    (_handler as AccurateElapsedEventHandler)(_param, delta);
                }
                else if (_handler is ElapsedIDEventHandler)
                {
                    (_handler as ElapsedIDEventHandler)(_param, Id);
                }
                else
                {
                    (_handler as ElapsedEventHandler)(_param);
                }
            }

            public int CompareTo(XTimer other)
            {
                //BUG #32194
                return (_triggerTime == other._triggerTime) ?
                    ((int)(_token - other.Token)) :
                    (_triggerTime < other._triggerTime ? -1 : 1);
            }

            public double TimeLeft()
            {
                return _triggerTime - XTimerMgr.singleton.Elapsed;   // DateTime.Now.Ticks;
                //return XCommon.singleton.TicksToSeconds(left);
            }
        }

        private uint _token = 0;
        private double _elapsed = 0;

        //event pool
        private Queue<XTimer> _pool = new Queue<XTimer>();
        private XHeap<XTimer> _timers = new XHeap<XTimer>();

        private Dictionary<uint, XTimer> _dict = new Dictionary<uint, XTimer>(20);

        private float _intervalTime = 0.0f;
        private float _updateTime = 0.1f;
        private bool _fixedUpdate = false;

        public double Elapsed { get { return _elapsed; } }
        public bool NeedFixedUpdate { get { return _fixedUpdate; } }

        //Interval time in seconds
        public uint SetTimer(float interval, ElapsedEventHandler handler, object param)
        {
            ++_token;

            if (interval <= 0)
            {
                handler(param);
                /*
                 * after handler(param) another timer maybe set
                 * so _token must be updated again
                 */
                ++_token;
            }
            else
            {
                double trigger = _elapsed + (double)interval;

                XTimer aTimer = GetTimer(trigger, handler, param, _token, false);

                _timers.PushHeap(aTimer);
                _dict.Add(_token, aTimer);
            }

            return _token;
        }

        public uint SetGlobalTimer(float interval, ElapsedEventHandler handler, object param)
        {
            ++_token;

            if (interval <= 0)
            {
                handler(param);
                /*
                 * after handler(param) another timer maybe set
                 * so _token must be updated again
                 */
                ++_token;
            }
            else
            {
                double trigger = _elapsed + (double)interval;

                XTimer aTimer = GetTimer(trigger, handler, param, _token, true);

                _timers.PushHeap(aTimer);
                _dict.Add(_token, aTimer);
            }

            return _token;
        }

        public uint SetTimerAccurate(float interval, AccurateElapsedEventHandler handler, object param)
        {
            ++_token;

            if (interval <= 0)
            {
                handler(param, 0);
                /*
                 * after handler(param) another timer maybe set
                 * so _token must be updated again
                 */
                ++_token;
            }
            else
            {
                double trigger = _elapsed + (double)interval;

                XTimer aTimer = GetTimer(trigger, handler, param, _token, false);

                _timers.PushHeap(aTimer);
                _dict.Add(_token, aTimer);
            }

            return _token;
        }

        public void AdjustTimer(float interval, uint token, bool closed = false)
        {
            XTimer timer = null;

            if (_dict.TryGetValue(token, out timer) && !timer.IsInPool)
            {
                double trigger = closed ? (_elapsed - (Time.deltaTime * 0.5f) + interval) : (_elapsed + interval);
                double old_trigger_at = timer.TriggerTime;

                timer.Refine(trigger);

                _timers.Adjust(timer, old_trigger_at < timer.TriggerTime);
            }
        }

        public void KillTimerAll()
        {
            List<XTimer> itemsToRemove = new List<XTimer>();

            foreach (XTimer timer in _dict.Values)
            {
                if (timer.IsGlobaled) continue;
                itemsToRemove.Add(timer);
            }

            for (int i = 0; i < itemsToRemove.Count; i++)
            {
                KillTimer(itemsToRemove[i]);
            }

            itemsToRemove.Clear();
        }

        private void KillTimer(XTimer timer)
        {
            if (timer == null) return;

            _timers.PopHeapAt(timer.Here);

            Discard(timer);
        }

        public void KillTimer(uint token)
        {
            if (token == 0) return;

            XTimer timer = null;

            if (_dict.TryGetValue(token, out timer))
            {
                KillTimer(timer);
            }
        }

        public double TimeLeft(uint token)
        {
            XTimer timer = null;

            if (_dict.TryGetValue(token, out timer))
            {
                return timer.TimeLeft();
            }
            else
            {
                return 0;
            }
        }

        public void Update(float fDeltaT)
        {
            _elapsed += fDeltaT;
            _intervalTime += fDeltaT;

            if (_intervalTime > _updateTime)
            {
                _intervalTime = 0.0f;
                _fixedUpdate = true;
            }

            TriggerTimers();
        }

        public void PostUpdate()
        {
            _fixedUpdate = false;
        }

        private void TriggerTimers()
        {
            while (_timers.HeapSize > 0)
            {
                XTimer timer = _timers.Peek();
                float delta = (float)(_elapsed - timer.TriggerTime);

                if (delta >= 0)
                {
                    ExecuteTimer(_timers.PopHeap(), delta);
                }
                else
                {
                    break;
                }
            }
        }

        private void ExecuteTimer(XTimer timer, float delta)
        {
            //discard first
            Discard(timer);
            //then fired
            timer.Fire(delta);

            //uint token = timer.Token;
            /*
             * After Fired
             * timer could already be discard because caller kill-timer in callback function
             * timer could already be reused because caller set another timer after killing last one in callback function
             */
            //timer.Fire(delta);
            /*
             * So...
             * Discard not executed every time after Fire being called.
             */
            //if (token == timer.Token) Discard(timer);
        }

        private void Discard(XTimer timer)
        {
            //already in pool
            if (timer.IsInPool)
            {
                return;
            }

            //remove
            if (_dict.Remove(timer.Token))
            {
                timer.IsInPool = true;
                //back to event pool
                _pool.Enqueue(timer);
            }
        }

        private XTimer GetTimer(double trigger, object handler, object parma, uint token, bool global, int id = -1)
        {
            if (_pool.Count > 0)
            {
                XTimer t = _pool.Dequeue();

                t.Refine(trigger, handler, parma, token, global, id);

                return t;
            }
            else
            {
                return new XTimer(trigger, handler, parma, token, global, id);
            }
        }

        public override bool Init()
        {
            return true;
        }

        public override void Uninit()
        {

        }

        public float GetCurTime()
        {
            return 0;
        }
    }

    public class XElapseTimer
    {
        private float m_OriLeftTime = 0.0f;
        private float m_LeftTime = 0.0f;
        private float m_LastTime = -1.0f;
        private float m_PassTime = 0.0f;
        public float LeftTime
        {
            get
            {
                return m_LeftTime;
            }
            set
            {
                m_LeftTime = value;
                m_OriLeftTime = m_LeftTime;
                m_LastTime = -1.0f;
                m_PassTime = 0.0f;
            }
        }

        public float PassTime
        {
            get
            {
                return m_PassTime;
            }
        }

        public void Update()
        {
            m_PassTime = 0.0f;
            if (m_LastTime < 0.0f)
            {
                m_LastTime = Time.time;
            }
            else
            {
                m_PassTime = Time.time - m_LastTime;
            }
            m_LeftTime = m_OriLeftTime - m_PassTime;
            if (m_LeftTime < 0.0f)
                m_LeftTime = 0.0f;
        }
    }
}