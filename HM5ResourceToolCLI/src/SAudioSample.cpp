#include "pch.h"
#include "SAudioSample.hpp"

namespace HM5ResourceToolCLI
{
    SAudioSample::SAudioSample() : ManagedObject(new HM5ResourceTool::SAudioSample())
    {

    }

    SAudioSample::SAudioSample(HM5ResourceTool::SAudioSample* audioSample) : ManagedObject(audioSample)
    {

    }

    array<Byte>^ SAudioSample::GetData()
    {
        IntPtr dataPointer = IntPtr(instance->data);
        unsigned int size = instance->dataSize;
        array<Byte>^ bytes = gcnew array<Byte>(size);

        Marshal::Copy(dataPointer, bytes, 0, size);

        return bytes;
    }

    unsigned int SAudioSample::GetDataSize()
    {
        return instance->dataSize;
    }

    String^ SAudioSample::GetName()
    {
        return marshal_as<String^>(instance->name);
    }
}
