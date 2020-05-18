#ifndef R_RCPPSIMDJSON_FROM_JSON_UTILS_H
#define R_RCPPSIMDJSON_FROM_JSON_UTILS_H

#include "simdjson.h"

#define R_NA_VAL Rcpp::LogicalVector::create(NA_LOGICAL);

namespace rcppsimdjson {
namespace from_json { 

inline R_xlen_t where_is(
    Rcpp::String to_find,
    Rcpp::StringVector& sv
) {
  R_xlen_t n = sv.size();
  R_xlen_t i;
  
  for( i = 0; i < n; ++i ) {
    
    if ( to_find == sv[i] ) {
      return i;
    }
  }
  return -1;
}

  // get_dtyles - different implementation to jsonify
  // interate over json object and get the unique data types of each value
  inline std::unordered_set< simdjson::dom::element_type > get_dtypes( simdjson::dom::array& arr ) {
    std::unordered_set< simdjson::dom::element_type > dtypes;
    
    //Rcpp::Rcout << "getting d types " << std::endl;
    
    R_xlen_t doc_len = arr.size();
    R_xlen_t i;
    for( i = 0; i < doc_len; ++i ) {
      Rcpp::Rcout << "curr_dtype " <<  arr.at(i).type() << std::endl;
      dtypes.insert( arr.at(i).type() );
    }
    return dtypes;
  }
  
  inline std::unordered_set< simdjson::dom::element_type > get_dtypes( simdjson::dom::object& obj ) {
    std::unordered_set< simdjson::dom::element_type > dtypes;

    //Rcpp::Rcout << "getting d types " << std::endl;
    
    for ( auto key_value : obj ) {
      Rcpp::Rcout << "curr_dtype " << key_value.value.type() << std::endl;
      dtypes.insert( key_value.value.type() );
    }
    return dtypes;
  }
  
  // inline std::unordered_set< simdjson::dom::element_type > get_dtypes( simdjson::dom::element& doc ) {
  //   
  //   Rcpp::Rcout << "getting d types " << std::endl;
  //   
  //   // iff it's an array or object, just need that type, no need to recurse into it.
  //   
  //   
  //   switch( doc.type() ) {
  //   case simdjson::dom::element_type::OBJECT: {
  //     // simdjson::dom::object obj = doc.get< simdjson::dom::object >();
  //     // return get_dtypes( obj );
  //   }
  //   case simdjson::dom::element_type::ARRAY: {
  //     // simdjson::dom::array arr = doc.get< simdjson::dom::array >();
  //     // return get_dtypes( arr );
  //   }
  //   default: {
  //     Rcpp::stop("RcppSimdJson - invalid type" );
  //   }
  //   }
  //   std::unordered_set< simdjson::dom::element_type > t;
  //   return t;
  // }
  
  
  inline bool contains_array( std::unordered_set< simdjson::dom::element_type >& dtypes ) {
    return dtypes.find( simdjson::dom::element_type::ARRAY ) != dtypes.end();
  }
  
  inline bool contains_object( std::unordered_set< simdjson::dom::element_type >& dtypes ) {
    return dtypes.find( simdjson::dom::element_type::OBJECT ) != dtypes.end();
  }
  
  inline bool contains_object_or_array( std::unordered_set< simdjson::dom::element_type >& dtypes ) {
    return contains_array( dtypes ) || contains_object( dtypes );
  }


  
  template <int RTYPE>
  inline R_xlen_t sexp_length(Rcpp::Vector<RTYPE> v) {
    return v.length();
  }
  
  inline R_xlen_t get_sexp_length( SEXP s ) {
    
    switch( TYPEOF(s) ) {
    case NILSXP: 
      return 0;
    case LGLSXP:
      return sexp_length< LGLSXP >( s );
    case REALSXP:
      return sexp_length< REALSXP >( s );
    case VECSXP:
      //return 2; // number bigger than 1 (vector)
      return sexp_length< VECSXP >( s );
    case INTSXP:
      return sexp_length< INTSXP >( s );
    case STRSXP:
      return sexp_length< STRSXP >( s );
    default: Rcpp::stop("jsonify - unknown vector type");
    }
    return 0;
  }

  inline R_xlen_t column_value(
      std::unordered_map< std::string, R_xlen_t >& column_map,
      const char* to_find
  ) {
    std::string str( to_find );
    std::unordered_map< std::string, R_xlen_t >::iterator it;
    it = column_map.find( str );
    
    if( it != column_map.end() ) {
      R_xlen_t res = it->second;
      return res;
    }
    return -1;
  }
  
  inline void insert_column_value(
      Rcpp::List& columns,
      const char* this_column,
      SEXP& val,
      R_xlen_t& row_index
  ) {
    Rcpp::List lst = columns[ this_column ];
    lst[ row_index ] = val;
    Rcpp::StringVector n = columns.names();
    columns[ this_column ] = lst;
  }
  
  
  // here we don't actually care what the type is yet
  // as for now a 'column' is just a list
  inline void append_new_column(
      Rcpp::List& columns,
      const char* this_column,
      R_xlen_t& n_rows
  ) {
    Rcpp::List new_column( n_rows );
    columns[ this_column ] = new_column;
  }
  
  inline void append_new_column_fill_na(
      Rcpp::List& columns,
      const char* this_column,
      R_xlen_t& n_rows
  ) {
    Rcpp::List new_column( n_rows );
    // need NAs when fill_na = true;
    R_xlen_t i;
    for( i = 0; i < n_rows; ++i ) {
      new_column[i] = NA_LOGICAL;
    }
    columns[ this_column ] = new_column;
  }

} // from_json
} // rcppsimdjson

#endif