
#include "simdjson.h"
#include "from_json/from_json.hpp"

// [[Rcpp::export]]
SEXP rcpp_from_json( std::string json, bool simplify, bool fill_na ) {
  
  simdjson::dom::parser parser;
  simdjson::dom::element doc = parser.parse( json );
  
  if( doc.is< bool >() ) {
    return Rcpp::LogicalVector::create(doc);
  }
  
  if( doc.is< int64_t >() ) {
    return rcppsimdjson::from_json::resolve_int<int64_t>(doc);
  }

  if( doc.is< uint64_t >() ) {
    return rcppsimdjson::from_json::resolve_int<uint64_t>(doc);
  }
  
  if( doc.is< double >() ) {
    return Rcpp::NumericVector::create(doc);
  }
  
  if( doc.is< std::string_view >() ) {
    return Rcpp::StringVector::create(std::string(doc).c_str());
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

