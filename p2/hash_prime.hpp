// adopted from
// /usr/include/c++/10.2.0/ext/pb_ds/detail/resize_policy/hash_prime_size_policy_imp.hpp

#include <utility>

namespace HashPrime {
enum {
  num_distinct_sizes_32_bit = 30,
  num_distinct_sizes_64_bit = 62,
  num_distinct_sizes = sizeof(std::size_t) != 8 ? num_distinct_sizes_32_bit
                                                : num_distinct_sizes_64_bit,
};

// Originally taken from the SGI implementation; acknowledged in the docs.
// Further modified (for 64 bits) from tr1's hashtable.
static constexpr std::size_t g_a_sizes[num_distinct_sizes_64_bit] = {
    /* 0     */ 5ul,
    /* 1     */ 11ul,
    /* 2     */ 23ul,
    /* 3     */ 47ul,
    /* 4     */ 97ul,
    /* 5     */ 199ul,
    /* 6     */ 409ul,
    /* 7     */ 823ul,
    /* 8     */ 1741ul,
    /* 9     */ 3469ul,
    /* 10    */ 6949ul,
    /* 11    */ 14033ul,
    /* 12    */ 28411ul,
    /* 13    */ 57557ul,
    /* 14    */ 116731ul,
    /* 15    */ 236897ul,
    /* 16    */ 480881ul,
    /* 17    */ 976369ul,
    /* 18    */ 1982627ul,
    /* 19    */ 4026031ul,
    /* 20    */ 8175383ul,
    /* 21    */ 16601593ul,
    /* 22    */ 33712729ul,
    /* 23    */ 68460391ul,
    /* 24    */ 139022417ul,
    /* 25    */ 282312799ul,
    /* 26    */ 573292817ul,
    /* 27    */ 1164186217ul,
    /* 28    */ 2364114217ul,
    /* 29    */ 4294967291ul,
    /* 30    */ (std::size_t)8589934583ull,
    /* 31    */ (std::size_t)17179869143ull,
    /* 32    */ (std::size_t)34359738337ull,
    /* 33    */ (std::size_t)68719476731ull,
    /* 34    */ (std::size_t)137438953447ull,
    /* 35    */ (std::size_t)274877906899ull,
    /* 36    */ (std::size_t)549755813881ull,
    /* 37    */ (std::size_t)1099511627689ull,
    /* 38    */ (std::size_t)2199023255531ull,
    /* 39    */ (std::size_t)4398046511093ull,
    /* 40    */ (std::size_t)8796093022151ull,
    /* 41    */ (std::size_t)17592186044399ull,
    /* 42    */ (std::size_t)35184372088777ull,
    /* 43    */ (std::size_t)70368744177643ull,
    /* 44    */ (std::size_t)140737488355213ull,
    /* 45    */ (std::size_t)281474976710597ull,
    /* 46    */ (std::size_t)562949953421231ull,
    /* 47    */ (std::size_t)1125899906842597ull,
    /* 48    */ (std::size_t)2251799813685119ull,
    /* 49    */ (std::size_t)4503599627370449ull,
    /* 50    */ (std::size_t)9007199254740881ull,
    /* 51    */ (std::size_t)18014398509481951ull,
    /* 52    */ (std::size_t)36028797018963913ull,
    /* 53    */ (std::size_t)72057594037927931ull,
    /* 54    */ (std::size_t)144115188075855859ull,
    /* 55    */ (std::size_t)288230376151711717ull,
    /* 56    */ (std::size_t)576460752303423433ull,
    /* 57    */ (std::size_t)1152921504606846883ull,
    /* 58    */ (std::size_t)2305843009213693951ull,
    /* 59    */ (std::size_t)4611686018427387847ull,
    /* 60    */ (std::size_t)9223372036854775783ull,
    /* 61    */ (std::size_t)18446744073709551557ull,
};

} // namespace HashPrime
