#ifndef RCPPSIMDJSON_PARSE_JSON_HPP
#define RCPPSIMDJSON_PARSE_JSON_HPP

#include <Rcpp.h>

#include "simdjson.h"

#include "utils.hpp"

namespace rcppsimdjson {
namespace parse_json {

// dispatcher forward declaration
template <rcppsimdjson::utils::Int64_R_Type int64_opt>
inline constexpr SEXP parse_element(simdjson::dom::element);


template <rcppsimdjson::utils::Int64_R_Type int64_opt>
inline constexpr SEXP parse_array(simdjson::dom::array array) {
#if __cplusplus >= 201703L
  Rcpp::List out(std::size(array));
#else
  Rcpp::List out(array.size());
#endif

  R_xlen_t i = 0;
  for (auto child : array) {
    out[i++] = parse_element<int64_opt>(child);
  }

  return out;
}


template <rcppsimdjson::utils::Int64_R_Type int64_opt>
inline constexpr SEXP parse_object(simdjson::dom::object object) {
#if __cplusplus >= 201703L
  const R_xlen_t n = std::size(object);
#else
  const R_xlen_t n = object.size();
#endif

  Rcpp::List out(n);
  Rcpp::CharacterVector out_names(n);
  R_xlen_t i = 0;

#if __cplusplus >= 201703L
  for (auto [key, value] : object) {
    out[i] = parse_element<int64_opt>(value);
    out_names[i++] = std::string(key);
  }
#else
  for (auto key_value : object) {
    out[i] = parse_element<int64_opt>(key_value.value);
    out_names[i++] = std::string(key_value.key);
  }
#endif

  out.attr("names") = out_names;
  return out;
}


template <rcppsimdjson::utils::Int64_R_Type int64_opt>
inline constexpr SEXP parse_element(simdjson::dom::element element) {
  switch (element.type()) {
    case simdjson::dom::element_type::ARRAY:
      return parse_array<int64_opt>(element);

    case simdjson::dom::element_type::OBJECT:
      return parse_object<int64_opt>(element);

    case simdjson::dom::element_type::STRING:
#if __cplusplus >= 201703L
      return Rcpp::wrap(std::string(element));
#else
      return Rcpp::wrap(std::string(std::string_view(element)));
#endif

    case simdjson::dom::element_type::INT64:
      return rcppsimdjson::utils::resolve_int64<int64_t, int64_opt>(element);

    case simdjson::dom::element_type::DOUBLE:
      return Rcpp::wrap<double>(element);

    case simdjson::dom::element_type::BOOL:
      return Rcpp::wrap<bool>(element);

    case simdjson::dom::element_type::NULL_VALUE:
      return R_NilValue;

    // least likely, so go last
    case simdjson::dom::element_type::UINT64:
      return rcppsimdjson::utils::resolve_int64<uint64_t, int64_opt>(element);
  }

  return R_NilValue;
}


}  // namespace parse_json
}  // namespace rcppsimdjson

#endif