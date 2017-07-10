#pragma once

#include  "QsSdkConfig.h"

#include <memory>

namespace QingStor
{
    /**
     * ::new, ::delete, ::malloc, ::free, std::make_shared, and std::make_unique should not be used in SDK code
     *  use these functions instead or Aws::MakeShared
     */
    template<typename T, typename ...ArgTypes>
    T* New(ArgTypes&&... args)
    {
        //void *rawMemory = Malloc(allocationTag, sizeof(T));
        void *rawMemory = malloc(sizeof(T));

        T *constructedMemory = new (rawMemory) T(std::forward<ArgTypes>(args)...);
        return constructedMemory;
    }

    /**
     * ::new, ::delete, ::malloc, ::free, std::make_shared, and std::make_unique should not be used in SDK code
     *  use these functions instead or Aws::MakeShared
     */
    template<typename T>
    void Delete(T* pointerToT)
    {
        if (pointerToT == nullptr)
        {
            return;
        }

        pointerToT->~T();
        free(pointerToT);
    }
}