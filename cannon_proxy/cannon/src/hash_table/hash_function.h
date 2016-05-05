//
// Created by LuoHuaqing on 2/14/16.
//

#ifndef CANNON_HASH_FUNCTION_H
#define CANNON_HASH_FUNCTION_H

namespace paradigm4 {
    /// Primary class template hash.
    template<typename _Tp>
    struct hash;
    // Explicit specializations for integer types.
#define _Cxx_hashtable_define_trivial_hash(_Tp) \
    template<>                                   \
    struct hash<_Tp>                              \
    {                                              \
      size_t                                        \
      operator()(_Tp __val) const noexcept           \
      { return static_cast<size_t>(__val); }          \
    };

    /// Explicit specialization for bool.
    _Cxx_hashtable_define_trivial_hash(bool)

    /// Explicit specialization for char.
    _Cxx_hashtable_define_trivial_hash(char)

    /// Explicit specialization for signed char.
    _Cxx_hashtable_define_trivial_hash(signed char)

    /// Explicit specialization for unsigned char.
    _Cxx_hashtable_define_trivial_hash(unsigned char)

    /// Explicit specialization for wchar_t.
    _Cxx_hashtable_define_trivial_hash(wchar_t)

    /// Explicit specialization for short.
    _Cxx_hashtable_define_trivial_hash(short)

    /// Explicit specialization for int.
    _Cxx_hashtable_define_trivial_hash(int)

    /// Explicit specialization for long.
    _Cxx_hashtable_define_trivial_hash(long)

    /// Explicit specialization for long long.
    _Cxx_hashtable_define_trivial_hash(long long)

    /// Explicit specialization for unsigned short.
    _Cxx_hashtable_define_trivial_hash(unsigned short)

    /// Explicit specialization for unsigned int.
    _Cxx_hashtable_define_trivial_hash(unsigned int)

    /// Explicit specialization for unsigned long.
    _Cxx_hashtable_define_trivial_hash(unsigned long)

    /// Explicit specialization for unsigned long long.
    _Cxx_hashtable_define_trivial_hash(unsigned long long)

#undef _Cxx_hashtable_define_trivial_hash
} //namespace paradigm4

#endif //CANNON_HASH_FUNCTION_H
