using System;
using System.Collections.Generic;

namespace XUtliPoolLib
{
    internal interface IHere
    {
        int Here { get; set; }
    }

    internal class XHeap<T> where T : IComparable<T>, IHere
    {
        private List<T> _heap = null;

        private int _heapSize = 0;

        public XHeap()
        {
            _heap = new List<T>();
            _heapSize = 0;
        }

        public int HeapSize
        {
            get { return _heapSize; }
        }

        //O(log(n))
        public void PushHeap(T item)
        {
            int count = _heap.Count;
            if (_heapSize < count)
            {
                _heap[_heapSize] = item;
            }
            else
            {
                _heap.Add(item);
            }

            item.Here = _heapSize;
            HeapifyUp(_heap, _heapSize);

            _heapSize++;
        }

        //O(log(n))
        public T PopHeap()
        {
            T t = default(T);

            if (_heapSize > 0)
            {
                _heapSize--;

                Swap(_heap, 0, _heapSize);
                HeapifyDown(_heap, 0, _heapSize);

                t = _heap[_heapSize];
                t.Here = -1;
                _heap[_heapSize] = default(T);
            }
            
            return t;
        }

        //O(log(n))
        public void PopHeapAt(int Idx)
        {
            if (_heapSize > 0 && Idx >= 0 && Idx < _heapSize)
            {
                _heapSize--;

                Swap(_heap, Idx, _heapSize);

                if (_heap[_heapSize].CompareTo(_heap[Idx]) < 0)
                    HeapifyDown(_heap, Idx, _heapSize);
                else if (_heap[_heapSize].CompareTo(_heap[Idx]) > 0)
                    HeapifyUp(_heap, Idx);

                _heap[_heapSize].Here = -1;
                _heap[_heapSize] = default(T);
            }
        }

        public void Adjust(T item, bool downwords)
        {
            if (_heapSize > 1)
            {
                if (downwords)
                {
                    HeapifyDown(_heap, item.Here, _heapSize);
                }
                else
                {
                    HeapifyUp(_heap, item.Here);
                }
            }
        }

        public static void MakeHeap(List<T> list)
        {
            int i = list.Count / 2 - 1;

            while (i >= 0)
            {
                HeapifyDown(list, i, list.Count);
                --i;
            }
        }

        /* O(nlog(n))
         * Notice:
         * The list will be sorted in descending order.
         */
        public static void HeapSort(List<T> list)
        {
            if (list.Count < 2) return;

            MakeHeap(list);

            for (int i = list.Count - 1; i > 0; i--)
            {
                Swap(list, 0, i);
                HeapifyDown(list, 0, i);
            }
        }

        /* O(1)
         * this function may cause corruption of the heap structure
         * when using the reference T to change the data value.
         */
        public T Peek()
        {
            if (_heapSize > 0)
            {
                return _heap[0];
            }
            else
                return default(T);
        }

        public void Clear()
        {
            _heap.Clear();
            _heapSize = 0;
        }

        //O(log(n))
        private static void HeapifyDown(List<T> heap, int curIdx, int heapSize)
        {
            while (curIdx < heapSize)
            {
                int left = 2 * curIdx + 1;
                int right = 2 * curIdx + 2;

                T smaller = heap[curIdx];
                int smallerIdx = curIdx;

                if (left < heapSize && heap[left].CompareTo(smaller) < 0)
                {
                    smaller = heap[left];
                    smallerIdx = left;
                }
                if (right < heapSize && heap[right].CompareTo(smaller) < 0)
                {
                    smaller = heap[right];
                    smallerIdx = right;
                }

                if (smallerIdx != curIdx)
                {
                    Swap(heap, curIdx, smallerIdx);

                    curIdx = smallerIdx;
                }
                else
                    break;
            }
        }

        //O(log(n))
        private static void HeapifyUp(List<T> heap, int curIdx)
        {
            while (curIdx > 0)
            {
                int parent = (curIdx - 1) / 2;

                T smaller = heap[parent];
                int smallerIdx = parent;

                if (parent >= 0 && heap[curIdx].CompareTo(smaller) < 0)
                {
                    smallerIdx = curIdx;
                }

                if (smallerIdx != parent)
                {
                    Swap(heap, parent, smallerIdx);
                    curIdx = parent;
                }
                else
                    break;
            }
        }

        private static void Swap(List<T> heap, int x, int y)
        {
            T t = heap[x];
            heap[x] = heap[y]; heap[x].Here = x;
            heap[y] = t; heap[y].Here = y;
        }
    }
}
