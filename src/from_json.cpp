
#include "simdjson.h"
#include "from_json/from_json.hpp"

// [[Rcpp::export]]
SEXP rcpp_from_json( std::string json, bool simplify, bool fill_na ) {
  
  simdjson::dom::parser parser;
  simdjson::dom::element doc = parser.parse( json );
  
  simdjson::dom::element_type t = doc.type();

  if( doc.is< bool >() ) {
    Rcpp::LogicalVector x(1);
    x[0] = doc.get< bool >();
    return x;
  }
  
  if( doc.is< int64_t >() ) {
    Rcpp::IntegerVector x(1);
    x[0] = doc.get< int64_t >();
    return x;
  }
  
  if( doc.is< double >() ) {
    Rcpp::NumericVector x(1);
    x[0] = doc.get< double >();
    return x;
  }
  
  if( doc.is< std::string_view >() ) {
    Rcpp::StringVector x(1);
    std::string_view s = doc.get< std::string_view >();
    std::string st = std::string( s ).c_str();
    x[0] = st;
    return x;
  }
  // TODO:
  // simdjson::dom::element_type::UINT64

  R_xlen_t sequential_array_counter = 0;
  
  return rcppsimdjson::from_json::json_to_sexp(
    doc, 
    simplify,
    fill_na,
    sequential_array_counter
    );

}