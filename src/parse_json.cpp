#include <RcppSimdJson.hpp>

// #if __cplusplus >= 201703L
// #include "simdjson.h"
// #include "simdjson.cpp"
// #endif

// [[Rcpp::export(.parse_json)]]
SEXP parse_json(const Rcpp::String& x, const int int64_T = 0) {
  using rcppsimdjson::parse_json::parse_element;
  using rcppsimdjson::utils::Int64_R_Type;

  const auto int64_type = static_cast<Int64_R_Type>(int64_T);
  simdjson::dom::parser parser;

#if __cplusplus >= 201703L
  auto [value, error] = parser.parse(simdjson::padded_string(x));
#else
  auto init = parser.parse(simdjson::padded_string(x));
  auto value = init.value();
  auto error = init.error();
#endif

  if (error) {
    Rcpp::stop(simdjson::error_message(error));
  }

  switch (int64_type) {
    case Int64_R_Type::Double:
      return parse_element<Int64_R_Type::Double>(value);

    case Int64_R_Type::String:
      return parse_element<Int64_R_Type::String>(value);

    case Int64_R_Type::Integer64:
      return parse_element<Int64_R_Type::Integer64>(value);
  }

  return R_NilValue;
}