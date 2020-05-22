#ifndef R_RCPPSIMDJSON_UTILS_H
#define R_RCPPSIMDJSON_UTILS_H

#include <Rcpp.h>

namespace rcppsimdjson::utils {

template <typename int_T>
inline constexpr bool is_castable_int(int_T);

template <>
inline constexpr bool is_castable_int<uint64_t>(uint64_t x) {
  return x <= std::numeric_limits<int>::max();
}

template <>
inline constexpr bool is_castable_int<int64_t>(int64_t x) {
  return x <= std::numeric_limits<int>::max() && x > NA_INTEGER;
}

template <typename int_T, bool bit64_integer64, bool int_64_strings>
inline constexpr SEXP resolve_int(int_T x) {
  return is_castable_int<int_T>(x)                                        //
             ? Rcpp::wrap<int>(x)                                         //
             : bit64_integer64 ?                                          //
                   Rcpp::wrap<int64_t>(x)                                 //
                               : int_64_strings ?                         //                 
                                     Rcpp::wrap(std::to_string(x))        //
    // TODO am I supposed to be able to do this ^^^^^^^^^^^^^^ inside a constexpr function?
                                                : Rcpp::wrap<double>(x);  //
}

}  // namespace rcppsimdjson::utils
#endif