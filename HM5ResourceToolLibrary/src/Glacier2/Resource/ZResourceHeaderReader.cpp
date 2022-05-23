#include "pch.h"
#include "Glacier2/Resource/ZResourceHeaderReader.hpp"
#include "IO/ZBinaryReader.hpp"

namespace IOI
{
    ZResourceHeaderReader::ZResourceHeaderReader(SResourceHeaderHeader headerHeader, const unsigned char* pReferencesChunk)
    {
        m_HeaderHeader = headerHeader;
        m_pReferencesChunk = pReferencesChunk;
    }

    unsigned int ZResourceHeaderReader::GetResourceType() const
    {
        return m_HeaderHeader.m_type;
    }

    unsigned int ZResourceHeaderReader::GetNumResourceIdentifiers() const
    {
        if (m_pReferencesChunk)
        {
            return *reinterpret_cast<const unsigned int*>(m_pReferencesChunk) & 0x7FFFFFFF;
        }

        return 0;
    }

    ZRuntimeResourceID ZResourceHeaderReader::GetResourceIdentifier(unsigned int lResourceIdentifierIndex) const
    {
        ZRuntimeResourceID result;

        if (lResourceIdentifierIndex == UINT32_MAX)
        {
            result.SetHigh(-1);
            result.SetLow(-1);
        }
        else
        {
            HM5ResourceTool::IO::ZBinaryReader binaryReader = HM5ResourceTool::IO::ZBinaryReader(m_pReferencesChunk, m_HeaderHeader.m_nReferencesChunkSize);
            int numberOfReferences = binaryReader.ReadInt32();

            if (numberOfReferences < 0)
            {
                binaryReader.GetInputStream()->Seek(sizeof(long long) * lResourceIdentifierIndex, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Current);

                unsigned long long hash = binaryReader.ReadUInt64();

                result = ZRuntimeResourceID::Create(hash);
            }
            else
            {
                const unsigned int firstFlagOffset = binaryReader.ReadUInt32();
                const unsigned int firstResourceIDOffset = binaryReader.ReadUInt32();
                std::string resourceID;

                if (numberOfReferences == 1)
                {
                    const unsigned int flag = binaryReader.ReadUInt32();

                    resourceID = binaryReader.ReadString();
                }
                else
                {
                    binaryReader.GetInputStream()->Seek(firstFlagOffset + lResourceIdentifierIndex * sizeof(int), HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

                    unsigned int resourceIDOffset = binaryReader.ReadUInt32() & 0x3FFFFFFF;

                    binaryReader.GetInputStream()->Seek(firstResourceIDOffset + resourceIDOffset, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

                    resourceID = binaryReader.ReadString();
                }

                result = ZRuntimeResourceID::QueryRuntimeResourceID(resourceID);
            }
        }

        return result;
    }

    EResourceReferenceFlags ZResourceHeaderReader::GetResourceFlags(unsigned int lResourceIdentifierIndex) const
    {
        HM5ResourceTool::IO::ZBinaryReader binaryReader = HM5ResourceTool::IO::ZBinaryReader(m_pReferencesChunk, m_HeaderHeader.m_nReferencesChunkSize);
        int numberOfReferences = binaryReader.ReadInt32();
        EResourceReferenceFlags result;

        if (numberOfReferences < 0)
        {
            unsigned int numberOfReferences2 = GetNumResourceIdentifiers();

            binaryReader.GetInputStream()->Seek(numberOfReferences2 * sizeof(long long) + lResourceIdentifierIndex, HM5ResourceTool::IO::IInputStream::ESeekOrigin::Current);

            result = static_cast<EResourceReferenceFlags>(binaryReader.ReadByte() << 24);
        }
        else
        {
            const unsigned int firstFlagOffset = binaryReader.ReadUInt32();

            binaryReader.GetInputStream()->Seek(firstFlagOffset + lResourceIdentifierIndex * sizeof(int), HM5ResourceTool::IO::IInputStream::ESeekOrigin::Begin);

            result = static_cast<EResourceReferenceFlags>(binaryReader.ReadUInt32() & 0xFF000000);
        }

        return result;
    }

    bool ZResourceHeaderReader::HasIntegerFormat() const
    {
        if (m_pReferencesChunk)
        {
            return *reinterpret_cast<const unsigned int*>(m_pReferencesChunk) >> 31;
        }

        return 0;
    }
}
