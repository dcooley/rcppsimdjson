#define STRICT_R_HEADERS

#include <RcppSimdJson.hpp>

// TODO vectorized version, "many" (ND)JSON(L) version, readers/streams


/*** R
.parse_json <- RcppSimdJson:::.parse_json
.query_json <- RcppSimdJson:::.query_json 

int64_options <- list(Double = 0L, String = 1L, Integer64 = 2L)

.parse_json('1')
.parse_json('{"test":1}')
.parse_json('{"test":[1,2.0,3000000000]}')
.parse_json('{"test":[1,2.0,3000000000]}', int64_options$Double)
.parse_json('{"test":[1,2.0,3000000000]}', int64_T = int64_options$String)
.parse_json('{"test":[1,2.0,3000000000]}', int64_T = int64_options$Integer64)
.parse_json('{"test":[10000000000,20000000000,3,{"a":{"b":[true,false,null]}}]}')

.query_json('{"test":1}', 
            json_pointer = "test")
.query_json('{"test":[1,2.0,3000000000]}', 
            json_pointer =  "test/0", int64_T = int64_options$String)
.query_json('{"test":[1,2.0,3000000000]}', 
            json_pointer = "test/2")
.query_json('{"test":[10000000000,20000000000,3,{"a":{"b":[true,false,null]}}]}', 
            json_pointer = "test/3/a/b/0")
*/

// [[Rcpp::export(.parse_json)]]
SEXP parse_json(const Rcpp::String& x, const int int64_T = 0) {
  using rcppsimdjson::parse_json::parse_element;
  using rcppsimdjson::utils::Int64_R_Type;

  const auto int64_type = static_cast<Int64_R_Type>(int64_T);
  simdjson::dom::parser parser;

#if __cplusplus >= 201703L
  auto [value, error] = parser.parse(x);
#else
  auto init = parser.parse(simdjson::padded_string(x));
  auto value = init.value();
  auto error = init.error();
#endif

  if (error != simdjson::error_code::SUCCESS) {
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


// [[Rcpp::export(.query_json)]]
SEXP query_json(const Rcpp::String& x,
                const std::string& json_pointer,
                const int int64_T = 0) {
  using rcppsimdjson::parse_json::parse_element;
  using rcppsimdjson::utils::Int64_R_Type;

  const auto int64_type = static_cast<Int64_R_Type>(int64_T);
  simdjson::dom::parser parser;

#if __cplusplus >= 201703L
  auto [value, error] = parser.parse(x).at(json_pointer);
#else
  auto init = parser.parse(simdjson::padded_string(x)).at(json_pointer);
  auto value = init.value();
  auto error = init.error();
#endif

  if (error != simdjson::error_code::SUCCESS) {
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
