#ifndef R_RCPPSIMDJSON_FROM_JSON_H
#define R_RCPPSIMDJSON_FROM_JSON_H

#include <Rcpp.h>
#include "simdjson.h"

namespace rcppsimdjson {
namespace from_json {

  // interim function until v0.4
  // - https://github.com/lemire/simdjson/issues/308
  // finds the 'size' of the json at the current depth
  inline R_xlen_t get_json_length( simdjson::ParsedJson::Iterator& pjh ) {
    // bool next(): Within a given scope (series of nodes at the same depth within either an array or an object),
    // we move foreward. Thus, given `true, null, {"a":1},[1,2]]`, we would visit
    // `true`, `null`, `{` and `[`. 
    R_xlen_t count = 0;
    
    //if( pjh.is_object()  ) {
      if( pjh.down() ) {
        while( pjh.next() ) {
          //Rcpp::Rcout << "{" << std::endl;
          ++count;
          pjh.next();
        }
        pjh.up();
      }
    // } else if ( pjh.is_array() ) {
    //   Rcpp::Rcout << "is_array" << std::endl;
    //   if( pjh.down() ) {
    //     while( pjh.next() ) {
    //       ++count;
    //       pjh.next();
    //     }
    //   pjh.up();
    //   }
    // }
    
    return count;
    
  }

  inline SEXP json_to_sexp( simdjson::ParsedJson::Iterator& pjh ) {
    
    Rcpp::List res(1);
    
    int json_type = pjh.get_type();
    Rcpp::Rcout << "type: " << json_type << std::endl;
    
    R_xlen_t json_length = get_json_length( pjh );
    Rcpp::Rcout << "length: " << json_length << std::endl;
    
    
    if( json_type == '{' ) { // object {}
      
    }
    
    
    return res;
  }

} // from_json
} // rcppsimdjson

#endif