#include "pch.h"
#include <Windows.h>
#include <format>
#include "Glacier2/Resource/ZRuntimeResourceID.hpp"
#include "Glacier2/ZBinarySerializer.hpp"
#include "Hash/MD5.hpp"

namespace IOI
{
    unsigned long long ZRuntimeResourceID::GetID() const
    {
        return (static_cast<unsigned long long>(m_IDHigh) << 32) | m_IDLow;
    }

    unsigned long long ZRuntimeResourceID::GetID()
    {
        return (static_cast<unsigned long long>(m_IDHigh) << 32) | m_IDLow;
    }

    ZRuntimeResourceID ZRuntimeResourceID::QueryRuntimeResourceID(const std::string& idResource)
    {
        unsigned long long hash = CreateMD5(idResource);

        return Create(hash);
    }

    std::string ZRuntimeResourceID::QueryResourceID(const ZRuntimeResourceID& ridResource)
    {
        std::string result = "";
        static std::unordered_map<unsigned long long, std::string> resourceIDs;

        if (resourceIDs.empty())
        {
            std::string line;
            char buffer[MAX_PATH];

            GetModuleFileNameA(NULL, buffer, MAX_PATH);

            std::string::size_type pos = std::string(buffer).find_last_of("\\/");
            std::ifstream ifstream = std::ifstream(std::format("{}/HashMap_PC.txt", std::string(buffer).substr(0, pos)));

            if (ifstream.is_open())
            {
                ifstream.seekg(0, ifstream.end);

                size_t fileSize = static_cast<size_t>(ifstream.tellg());

                ifstream.seekg(0, ifstream.beg);

                std::vector<char> hashListData = std::vector<char>(fileSize, 0);
                unsigned int position = 0, lastPosition = 0;

                ifstream.read(hashListData.data(), fileSize);

                while (true)
                {
                    if (hashListData.data()[position] == 0x0A)
                    {
                        hashListData.data()[position] = 0;

                        std::string line = std::string(&hashListData.data()[lastPosition]);

                        unsigned long long hash = std::stoull(line.substr(0, line.find(' ')), nullptr, 16);
                        std::string resourceID = line.substr(line.find(' ') + 1);

                        resourceIDs.insert(std::make_pair(hash, resourceID));

                        lastPosition = position + 1;
                    }

                    position++;

                    if (position > fileSize)
                    {
                        break;
                    }
                }

                ifstream.close();
            }
        }

        auto it = resourceIDs.find(ridResource.GetID());

        if (it != resourceIDs.end())
        {
            return it->second;
        }

        return result;
    }

    ZRuntimeResourceID ZRuntimeResourceID::Create(unsigned long long nResource)
    {
        ZRuntimeResourceID result;

        result.m_IDHigh = nResource >> 32;
        result.m_IDLow = static_cast<unsigned int>(nResource);

        return result;
    }

    ZRuntimeResourceID ZRuntimeResourceID::CreateLibraryResourceID(ZRuntimeResourceID ridLibrary, int indexInLibrary)
    {
        ZRuntimeResourceID result;

        result.m_IDHigh = indexInLibrary & 0xFFFFFF | 0x80000000;
        result.m_IDLow = ridLibrary.m_IDLow;

        return result;
    }

    bool ZRuntimeResourceID::IsLibraryResource() const
    {
        return m_IDHigh >> 31;
    }

    bool ZRuntimeResourceID::IsLibrary() const
    {
        return (m_IDHigh >> 30) & 1;
    }

    ZRuntimeResourceID ZRuntimeResourceID::GetLibraryRuntimeResourceID() const
    {
        ZRuntimeResourceID result;

        result.m_IDHigh = 0x40000000;
        result.m_IDLow = m_IDLow;

        return result;
    }

    int ZRuntimeResourceID::GetIndexInLibrary() const
    {
        return m_IDHigh & 0xFFFFFF;
    }

    int ZRuntimeResourceID::GetIndexInLibrary()
    {
        return m_IDHigh & 0xFFFFFF;
    }

    void ZRuntimeResourceID::SerializeToJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.String("m_IDHigh");
        writer.Uint(m_IDHigh);
        writer.String("m_IDLow");
        writer.Uint(m_IDLow);

        writer.EndObject();
    }

    void ZRuntimeResourceID::SerializeToMemory(ZBinarySerializer& binarySerializer, const unsigned int offset)
    {
        unsigned int idHighOffset = offset + offsetof(ZRuntimeResourceID, m_IDHigh);
        unsigned int idLowOffset = offset + offsetof(ZRuntimeResourceID, m_IDLow);

        binarySerializer.WriteToMemory(&m_IDHigh, sizeof(unsigned int), idHighOffset);
        binarySerializer.WriteToMemory(&m_IDLow, sizeof(unsigned int), idLowOffset);

        binarySerializer.RecordRuntimeResourceIDOffset(offset);
    }

    ZRuntimeResourceID* ZRuntimeResourceID::DeserializeFromJSON(const rapidjson::Value& object)
    {
        ZRuntimeResourceID* runtimeResourceID = new ZRuntimeResourceID();

        runtimeResourceID->m_IDHigh = object["m_IDHigh"].GetUint();
        runtimeResourceID->m_IDLow = object["m_IDLow"].GetUint();

        return runtimeResourceID;
    }

    unsigned long long ZRuntimeResourceID::CreateMD5(const std::string& resourceID)
    {
        unsigned char digest[16];
        MD5Context md5c;
        std::string resourceID2 = resourceID;

        std::transform(resourceID2.begin(), resourceID2.end(), resourceID2.begin(), [](unsigned char c) { return std::tolower(c); });

        unsigned char* buffer = new unsigned char[resourceID2.length()];

        std::copy(resourceID2.data(), resourceID2.data() + resourceID2.length() + 1, buffer);

        MD5Init(&md5c);
        MD5Update(&md5c, buffer, static_cast<unsigned int>(resourceID2.length()));
        MD5Final(digest, &md5c);

        std::stringstream ss;

        for (unsigned long long i = 0; i < 8; i++)
        {
            ss << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << static_cast<int>(digest[i]);
        }

        return std::stoull(ss.str().substr(2), nullptr, 16);
    }

    unsigned int ZRuntimeResourceID::GetHigh()
    {
        return m_IDHigh;
    }

    unsigned int ZRuntimeResourceID::GetLow()
    {
        return m_IDLow;
    }

    const unsigned int ZRuntimeResourceID::GetLow() const
    {
        return m_IDLow;
    }

    void ZRuntimeResourceID::SetHigh(unsigned int idHigh)
    {
        m_IDHigh = idHigh;
    }

    void ZRuntimeResourceID::SetLow(unsigned int idLow)
    {
        m_IDLow = idLow;
    }

    bool ZRuntimeResourceID::operator==(const ZRuntimeResourceID& other)
    {
        return m_IDHigh == other.m_IDHigh && m_IDLow == other.m_IDLow;
    }
}
