#include "pch.h"
#include <format>
#include "Glacier2/Resource/ZLocalResourceFileMediator.hpp"

namespace IOI
{
    std::string ZLocalResourceFileMediator::CreateMD5(std::string resourceID)
    {
        unsigned char digest[16];
        MD5Context md5c;

        std::transform(resourceID.begin(), resourceID.end(), resourceID.begin(), [](unsigned char c) { return std::tolower(c); });

        unsigned char* buffer = new unsigned char[resourceID.length()];

        std::copy(resourceID.data(), resourceID.data() + resourceID.length() + 1, buffer);

        MD5Init(&md5c);
        MD5Update(&md5c, buffer, static_cast<unsigned int>(resourceID.length()));
        MD5Final(digest, &md5c);

        std::stringstream ss;

        for (unsigned long long i = 0; i < 16; i++)
        {
            ss << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << static_cast<int>(digest[i]);
        }

        return ss.str();
    }

    std::string ZLocalResourceFileMediator::CalcResourceFileName(std::string resourceID)
    {
        std::transform(resourceID.begin(), resourceID.end(), resourceID.begin(), [](unsigned char c) { return std::tolower(c); });

        std::string extension = resourceID.substr(resourceID.find_last_of('.') + 1);
        int index = static_cast<int>(resourceID.find(':'));
        int index2 = static_cast<int>(resourceID.find('?'));
        int index3 = static_cast<int>(resourceID.find(']'));
        std::string folderPath = "../runtime";

        if (index2 != -1 && index < index2)
        {
            folderPath += resourceID.substr(index + 1, index2 - index - 1);
        }
        else
        {
            folderPath += resourceID.substr(index + 1, index3 - index - 1);
        }

        resourceID = resourceID.substr(0, resourceID.find_last_of('.') + 1);

        std::string fileName = CreateMD5(resourceID);

        return std::format("{}/{}.{}", folderPath, fileName, extension);
    }

    std::string ZLocalResourceFileMediator::GetResourceFileName(const ZRuntimeResourceID& resourceID)
    {
        std::string idOriginalResource;
        std::string result;

        if (resourceID.IsLibraryResource())
        {
            ZRuntimeResourceID ridLibrary = resourceID.GetLibraryRuntimeResourceID();

            idOriginalResource = ZRuntimeResourceID::QueryResourceID(ridLibrary);
        }
        else
        {
            idOriginalResource = ZRuntimeResourceID::QueryResourceID(resourceID);
        }

        result = ZLocalResourceFileMediator::CalcResourceFileName(idOriginalResource);

        return result;
    }
}
