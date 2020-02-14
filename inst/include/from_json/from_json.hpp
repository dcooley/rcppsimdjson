#ifndef R_RCPPSIMDJSON_FROM_JSON_H
#define R_RCPPSIMDJSON_FROM_JSON_H

#include <Rcpp.h>
#include "simdjson.h"

namespace rcppsimdjson {
namespace from_json {

  inline SEXP json_to_sexp( simdjson::ParsedJson& pj ) {
    
    Rcpp::List res(1);
    
    simdjson::ParsedJson::Iterator pjh( pj ); 
    
    const size_t d = pjh.get_depth();
    
    
    
    
    return res;
  }

} // from_json
} // rcppsimdjson

#endif