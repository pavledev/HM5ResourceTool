#pragma once

#include "ZBinarySerializer.hpp"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"
#include "Glacier2/ZString.hpp"

namespace IOI
{
    template <typename T>
    class TArray
    {
    public:
        TArray()
        {
            m_pStart = nullptr;
            m_pEnd = nullptr;
            m_pLast = nullptr;
        }

        TArray(const unsigned int ninitialCapacity)
        {
            m_pStart = new T[ninitialCapacity];
            m_pEnd = m_pStart;
            m_pLast = &m_pStart[ninitialCapacity];
        }

        TArray(const TArray& other)
        {
            if (other.Size() == 0)
            {
                return;
            }

            m_pStart = new T[other.Capacity()];
            m_pEnd = m_pStart;
            m_pLast = &m_pStart[other.Capacity()];

            for (unsigned int i = 0; i < other.Size(); ++i)
            {
                PushBack(other[i]);
            }
        }

        TArray& operator=(const TArray& other)
        {
            if (this != &other && other.Size() > 0)
            {
                if (m_pStart)
                {
                    delete[] m_pStart;
                }

                m_pStart = new T[other.Capacity()];
                m_pEnd = m_pStart;
                m_pLast = &m_pStart[other.Capacity()];

                for (unsigned int i = 0; i < other.Size(); ++i)
                {
                    PushBack(other[i]);
                }
            }

            return *this;
        }

        ~TArray()
        {
            if (m_pStart)
            {
                delete[] m_pStart;
            }
        }

        T* Start()
        {
            return m_pStart;
        }

        T* End()
        {
            return m_pEnd;
        }

        void Resize(unsigned int nSize)
        {
            const unsigned int capacity = Capacity();

            if (nSize < capacity)
            {
                return;
            }

            const unsigned int size = Size();
            unsigned int newSize = capacity * 2;

            if (newSize == 0)
            {
                newSize = 1;
            }

            while (newSize < nSize)
            {
                newSize *= 2;
            }

            T* newMemory = new T[newSize];

            if (m_pStart)
            {
                for (unsigned int i = 0; i < size; ++i)
                {
                    newMemory[i] = m_pStart[i];
                }

                delete[] m_pStart;
            }

            m_pStart = newMemory;
            m_pEnd = &m_pStart[size];
            m_pLast = &m_pStart[newSize];
        }

        unsigned int Size() const
        {
            return m_pEnd - m_pStart;
        }

        unsigned int Capacity() const
        {
            return m_pLast - m_pStart;
        }

        T& operator[](unsigned int nIndex)
        {
            return m_pStart[nIndex];
        }

        const T& operator[](unsigned int nIndex) const
        {
            return m_pStart[nIndex];
        }

        void Clear()
        {
            if (m_pStart)
            {
                delete[] m_pStart;

                m_pStart = nullptr;
                m_pEnd = nullptr;
                m_pLast = nullptr;
            }
        }

        void PushBack(const T& element)
        {
            const unsigned int size = Size();
            const unsigned int capacity = Capacity();

            if (size == capacity)
            {
                Resize(size + 1);
            }

            m_pStart[size] = element;
            m_pEnd = &m_pStart[size + 1];
        }

        void RemoveAt(const unsigned int index)
        {
            const unsigned int size = Size();
            const unsigned int sourceSize = (size - index - 1) * sizeof(T);

            memcpy(&m_pStart[index], &m_pStart[index + 1], sourceSize);

            m_pEnd--;
        }

        void SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
        {
            writer.StartArray();

            const unsigned int size = Size();

            for (unsigned int i = 0; i < size; i++)
            {
                T* element = &this->operator[](i);

                if constexpr (std::is_class_v<T>)
                {
                    element->SerializeToJSON(writer);
                }
                else
                {
                    if constexpr (std::is_same_v<T, char> || std::is_same_v<T, short> || std::is_same_v<T, int> || std::is_enum_v<T>)
                    {
                        writer.Int(*element);
                    }
                    else if constexpr (std::is_same_v<T, unsigned char> || std::is_same_v<T, unsigned short> || std::is_same_v<T, unsigned int>)
                    {
                        writer.Uint(*element);
                    }
                    else if constexpr (std::is_same_v<T, long long>)
                    {
                        writer.Int64(*element);
                    }
                    else if constexpr (std::is_same_v<T, unsigned long long>)
                    {
                        writer.Uint64(*element);
                    }
                    else if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>)
                    {
                        writer.Double(*element);
                    }
                    else if constexpr (std::is_same_v<T, bool>)
                    {
                        writer.Bool(*element);
                    }
                }
            }

            writer.EndArray();
        }

        void SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
        {
            const unsigned int elementsCount = Size();
            unsigned int elementsStartOffset;

            if (elementsCount > 0)
            {
                const unsigned int size = sizeof(T) * elementsCount;

                elementsStartOffset = binarySerializer.ReserveLayoutFor(elementsCount, sizeof(T), alignof(T), 4);

                const unsigned int elementsEndOffset = elementsStartOffset + size;

                binarySerializer.WriteToMemory(&elementsCount, sizeof(unsigned int), elementsStartOffset - 4);

                binarySerializer.WriteToMemory(&elementsStartOffset, sizeof(unsigned int), offset);
                binarySerializer.WriteToMemory(&elementsEndOffset, sizeof(unsigned int), offset + 4);
                binarySerializer.WriteToMemory(&elementsEndOffset, sizeof(unsigned int), offset + 8);
            }

            binarySerializer.RecordOffsetForRebasing(offset);
            binarySerializer.RecordOffsetForRebasing(offset + 4);
            binarySerializer.RecordOffsetForRebasing(offset + 8);

            for (unsigned int i = 0; i < elementsCount; ++i)
            {
                const unsigned int elementOffset = elementsStartOffset + sizeof(T) * i;
                T* type = &this->operator[](i);

                if (i == 0)
                {
                    binarySerializer.SetLayoutPointer(binarySerializer.GetAlignedLayoutPointer(binarySerializer.GetLayoutPointer(), alignof(TArray)));
                }

                if constexpr (std::is_class_v<T>)
                {
                    type->SerializeToMemory(binarySerializer, elementOffset);
                }
                else
                {
                    binarySerializer.WriteToMemory(type, sizeof(T), elementOffset);
                }
            }
        }

        static TArray* DeserializeFromJSON(const rapidjson::Value& array)
        {
            const unsigned int size = array.Size();
            TArray* array2 = new TArray(size);

            for (rapidjson::Value::ConstValueIterator it = array.Begin(); it != array.End(); ++it)
            {
                if constexpr (std::is_class_v<T>)
                {
                    T* element = nullptr;

                    if constexpr (std::is_same_v<T, ZString>)
                    {
                        element = T::DeserializeFromJSON(it->GetString());
                    }
                    else
                    {
                        if (it->IsObject())
                        {
                            const rapidjson::Value& object = it->GetObj();

                            element = T::DeserializeFromJSON(object);
                        }
                        else
                        {
                            const rapidjson::Value& array3 = it->GetArray();

                            element = T::DeserializeFromJSON(array3);
                        }
                    }

                    array2->PushBack(*element);

                    //delete element;
                }
                else
                {
                    if constexpr (std::is_same_v<T, char> || std::is_same_v<T, short> || std::is_same_v<T, int>)
                    {
                        array2->PushBack(it->GetInt());
                    }
                    else if constexpr (std::is_enum_v<T>)
                    {
                        array2->PushBack(static_cast<T>(it->GetUint()));
                    }
                    else if constexpr (std::is_same_v<T, unsigned char> || std::is_same_v<T, unsigned short> || std::is_same_v<T, unsigned int>)
                    {
                        array2->PushBack(it->GetUint());
                    }
                    else if constexpr (std::is_same_v<T, long long>)
                    {
                        array2->PushBack(it->GetInt64());
                    }
                    else if constexpr (std::is_same_v<T, unsigned long long>)
                    {
                        array2->PushBack(it->GetUint64());
                    }
                    else if constexpr (std::is_same_v<T, float>)
                    {
                        array2->PushBack(it->GetFloat());
                    }
                    else if constexpr (std::is_same_v<T, double>)
                    {
                        array2->PushBack(it->GetDouble());
                    }
                    else if constexpr (std::is_same_v<T, bool>)
                    {
                        array2->PushBack(it->GetBool());
                    }
                }
            }

            return array2;
        }

        T* GetStart()
        {
            return m_pStart;
        }

        T* GetEnd()
        {
            return m_pEnd;
        }

        bool operator==(const TArray& other)
        {
            for (unsigned int i = 0; i < Size(); ++i)
            {
                if (this->operator[](i) != other[i])
                {
                    return false;
                }
            }

            return true;
        }

    private:
        T* m_pStart;
        T* m_pEnd;
        T* m_pLast;
    };
}
