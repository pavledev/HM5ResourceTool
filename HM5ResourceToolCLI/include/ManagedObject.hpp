#pragma once

using namespace System;

namespace HM5ResourceToolCLI
{
    template <typename T>
    public ref class ManagedObject
    {
    protected:
        T* instance;

    public:
        ManagedObject()
        {
            instance = nullptr;
        }

        ManagedObject(T* instance) : instance(instance)
        {
        }

        virtual ~ManagedObject()
        {
            if (instance)
            {
                delete instance;
            }
        }

        !ManagedObject()
        {
            if (instance)
            {
                delete instance;
            }
        }

        T* GetInstance()
        {
            return instance;
        }
    };
}
