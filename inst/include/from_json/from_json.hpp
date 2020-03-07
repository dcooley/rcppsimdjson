#ifndef R_RCPPSIMDJSON_FROM_JSON_H
#define R_RCPPSIMDJSON_FROM_JSON_H

#include <Rcpp.h>
#include "simdjson.h"

namespace rcppsimdjson {
namespace from_json {

  inline void compute_dump(simdjson::ParsedJson::Iterator &pjh) {
    if (pjh.is_object()) {
      std::cout << "{";
      if (pjh.down()) {
        pjh.print(std::cout); // must be a string
        std::cout << ":";
        pjh.next();
        compute_dump(pjh); // let us recurse
        while (pjh.next()) {
          std::cout << ",";
          pjh.print(std::cout);
          std::cout << ":";
          pjh.next();
          compute_dump(pjh); // let us recurse
        }
        pjh.up();
      }
      std::cout << "}";
    } else if (pjh.is_array()) {
      std::cout << "[";
      if (pjh.down()) {
        compute_dump(pjh); // let us recurse
        while (pjh.next()) {
          std::cout << ",";
          compute_dump(pjh); // let us recurse
        }
        pjh.up();
      }
      std::cout << "]";
    } else {
      pjh.print(std::cout); // just print the lone value
    }
  }

  // interim function until v0.4
  // - https://github.com/lemire/simdjson/issues/308
  // finds the 'size' of the json at the current depth
  inline void get_json_length( simdjson::ParsedJson::Iterator& pjh, R_xlen_t& count ) {
    // bool next(): Within a given scope (series of nodes at the same depth within either an array or an object),
    // we move foreward. Thus, given `true, null, {"a":1},[1,2]]`, we would visit
    // `true`, `null`, `{` and `[`. 
    
    if( pjh.is_object()  ) {
      Rcpp::Rcout << "object" << std::endl;
      if( pjh.down() ) {
        while( pjh.next() ) {
          //Rcpp::Rcout << "{" << std::endl;
          ++count;
          pjh.next();
        }
        pjh.up();
      }
    } else if ( pjh.is_array() ) {
      Rcpp::Rcout << "array" << std::endl;
       if( pjh.down() ) {
         get_json_length( pjh, count );
         //Rcpp::Rcout << "down" << std::endl;
         // while( pjh.next() ) {
         //   Rcpp::Rcout << "next" << std::endl;
         //   ++count;
         //   pjh.next();
         // }
       pjh.up();
       }
    }
    //return count;
  }

  inline SEXP json_to_sexp( simdjson::ParsedJson::Iterator& pjh ) {
    
    Rcpp::List res(1);
    
    int json_type = pjh.get_type();
    Rcpp::Rcout << "type: " << json_type << std::endl;
    
    
    R_xlen_t json_length = 0;
    
    get_json_length( pjh, json_length );
    Rcpp::Rcout << "length: " << json_length << std::endl;
    
    
    if( json_type == '{' ) { // object {}
      
    }
    
    
    return res;
  }

} // from_json
} // rcppsimdjson

#endif