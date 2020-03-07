
#include "simdjson.h"
#include "from_json/from_json.hpp"

simdjson::ParsedJson::Iterator parse_json( std::string& json ) {
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
  
  simdjson::ParsedJson::Iterator pjh( pj );
  return pjh;
}

/*
 * rcpp_get_json_length
 * for testing 
 * returns the size of teh json object at the current depth
 */
// [[Rcpp::export]]
R_xlen_t rcpp_get_json_length( std::string json ) {
  simdjson::ParsedJson::Iterator pjh = parse_json( json );
  R_xlen_t count = 0;
  //rcppsimdjson::from_json::get_json_length( pjh, count );
  rcppsimdjson::from_json::compute_dump( pjh );
  return count;
}


// [[Rcpp::export]]
SEXP rcpp_from_json( std::string json ) {
  
  simdjson::ParsedJson::Iterator pjh = parse_json( json );
    
  return rcppsimdjson::from_json::json_to_sexp( pjh );
}