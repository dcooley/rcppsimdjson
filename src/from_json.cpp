
#include "simdjson.h"
#include "from_json/from_json.hpp"

// [[Rcpp::export]]
SEXP rcpp_from_json( std::string json ) {
  
  Rcpp::Rcout << json << std::endl;
  //simdjson::padded_string p = simdjson::get_corpus( json );
  simdjson::padded_string p = simdjson::padded_string( json );
  
  Rcpp::Rcout << "padded string" << std::endl;
  simdjson::ParsedJson pj;
  bool ok = pj.allocate_capacity( p.size() );
  const int res = simdjson::json_parse(p, pj);
  Rcpp::Rcout << "res: " << res << std::endl;
  
  if (res != 0) {
    // You can use the "simdjson/simdjson.h" header to access the error message
    Rcpp::stop( simdjson::error_message(res) );
  }
  //simdjson::ParsedJson pj = simdjson::build_parsed_json( json ); // do the parsing
  
  if ( !pj.is_valid() ) {
    Rcpp::Rcout << "something went wrong" << std::endl;
    Rcpp::stop(pj.get_error_message());
  }
  
  return rcppsimdjson::from_json::json_to_sexp( pj );
}