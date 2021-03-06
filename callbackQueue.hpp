#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

class CallbackQueue
{
private:
    using u8 = std::uint8_t;
    using u64 = std::uint64_t;
    std::array<u64, 2> mFlags = {{0u, 0u}};

public:
    bool Empty() const { return mFlags[0] == 0u && mFlags[1] == 0u; } // TODO: empty flag? #performance
    void Clear() { mFlags[0] = mFlags[1] = 0u; }
    
    u8 Size() const { return __builtin_popcountll(mFlags[0]) + __builtin_popcountll(mFlags[1]); }
    
    void Push(u8 x)
    {
        assert(x < 81u);
        u8 i = 0u;
        
        if (x >= 64u)
        {
            ++i;
            x -= 64u;
        }

        u64 bit = 1ull << x;
        mFlags[i] |= bit;
    }

    void PushFlags(std::array<u64, 2> flags) { mFlags[0] |= flags[0]; mFlags[1] |= flags[1]; }
    void PushFlags(u64 flags) { mFlags[0] |= flags; } // TODO: remove
    
    u8 Pop()
    {
        if (mFlags[0] != 0ull)
        {
            u8 ret = __builtin_ctzll(mFlags[0]);
            mFlags[0] &= ~(1ull << ret);
            return u8(ret);
        }
        else if (mFlags[1] != 0ull)
        {
            u8 ret = __builtin_ctzll(mFlags[1]);
            mFlags[1] &= ~(1ull << ret);
            return u8(64u + ret);
        }

        return u8(255);
    }

    void PopElement(u8 element)
    {
        if (element < 64u)
        {
            mFlags[0] &= ~(1u << element);
        }
        else
        {
            element -= 64u;
            mFlags[1] &= ~(1u << element);
        }
    }
};
