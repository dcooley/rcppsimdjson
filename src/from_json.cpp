
#include "simdjson.h"
#include "from_json/from_json.hpp"

// [[Rcpp::export]]
SEXP rcpp_from_json( std::string json ) {
  
  simdjson::padded_string p = simdjson::padded_string( json );
  simdjson::ParsedJson pj;
  bool ok = pj.allocate_capacity( p.size() );
  const int res = simdjson::json_parse(p, pj);

  if (res != 0) {
    Rcpp::stop( simdjson::error_message(res) );
  }
  
  if ( !pj.is_valid() ) {
    Rcpp::stop(pj.get_error_message());
  }
  
  return rcppsimdjson::from_json::json_to_sexp( pj );
}