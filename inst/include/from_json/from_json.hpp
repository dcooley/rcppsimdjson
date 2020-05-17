#ifndef R_RCPPSIMDJSON_FROM_JSON_H
#define R_RCPPSIMDJSON_FROM_JSON_H

#include <Rcpp.h>
#include "simdjson.h"

#include "from_json/from_json_utils.hpp"
#include "from_json/simplify/simplify.hpp"

namespace rcppsimdjson {
namespace from_json {

   inline SEXP json_to_sexp(
      simdjson::dom::element& doc, 
      bool& simplify,
      bool& fill_na,
      R_xlen_t& sequential_array_counter
  ) {
    
    Rcpp::List res(1);
    
    simdjson::dom::element_type json_type = doc.type();
    
    R_xlen_t i;
    R_xlen_t json_length;
    std::unordered_set< simdjson::dom::element_type > dtypes;
    
    if( json_type == simdjson::dom::element_type::OBJECT ) {
      // json object {}
      simdjson::dom::object obj = doc.get< simdjson::dom::object >();
      json_length = obj.size();
      
      Rcpp::List out( json_length );
      Rcpp::CharacterVector names( json_length );
      
      R_xlen_t i = 0;
      for( const auto& key_value : obj ) {
        std::string_view n = key_value.key;
        names[i] = std::string( n ).c_str();
        // Rcpp::Rcout << "names: " << names << std::endl;
        
        switch( key_value.value.type() ) {
        case simdjson::dom::element_type::BOOL: {
          bool x = key_value.value.get< bool >();
          out[i] = x;
          break;
        }
          
        case simdjson::dom::element_type::INT64: {
          int64_t x = key_value.value.get< int64_t >();
          out[i] = x;
          break;
        }
         
        case simdjson::dom::element_type::DOUBLE: {
          double x = key_value.value.get< double >();
          out[i] = x;
          break;
        }
          
        case simdjson::dom::element_type::NULL_VALUE: {
          out[i] = R_NA_VAL;
          break;
        }
          
        case simdjson::dom::element_type::ARRAY: {
          simdjson::dom::element inner_arr = key_value.value;
          out[i] = json_to_sexp( inner_arr, simplify, fill_na, sequential_array_counter );
          break;
        }
          
        case simdjson::dom::element_type::OBJECT: {
          simdjson::dom::element inner_obj = key_value.value;
          out[i] = json_to_sexp( inner_obj, simplify, fill_na, sequential_array_counter );
          break;
        }
          
        default: {
          Rcpp::stop("RcppSimdJson - unknown type");
        }
          
        }
        
        i++;
      } // for (auto key value)
      
      out.attr("names") = names;
      res[0] = out;
      
    } else if ( json_type == simdjson::dom::element_type::ARRAY ) {
      
      simdjson::dom::array arr = doc.get< simdjson::dom::array >();
      json_length = arr.size();
      dtypes = ::rcppsimdjson::from_json::get_dtypes( arr );
      
      if( !::rcppsimdjson::from_json::contains_object_or_array( dtypes ) ) {
        // array of scalars (no internal arrays or objects) 
        // can go straight to a vector
        res[0] = ::rcppsimdjson::from_json::array_to_vector( arr, simplify );
        
      } else {
        /// array with internal array
        // possibly simplified to matrix
        Rcpp::List array_of_array( json_length );
        for( i = 0; i < json_length; ++i ) {
          
          switch( arr.at(i).type() ) {
            
          case simdjson::dom::element_type::NULL_VALUE: {
            sequential_array_counter = 0;
            array_of_array[i] = R_NA_VAL;
            break;
          }
            
          case simdjson::dom::element_type::BOOL: {
            sequential_array_counter = 0;
            bool x = arr.at(i).get< bool >();
            array_of_array[i] = x;
            break;
          }
            
          case simdjson::dom::element_type::INT64: {
            sequential_array_counter = 0;
            int64_t x = arr.at(i).get< int64_t >();
            array_of_array[i] = x;
            break;
          }
            
          case simdjson::dom::element_type::DOUBLE: {
            sequential_array_counter = 0;
            double x = arr.at(i).get< double >();
            array_of_array[i] = x;
            break;
          }
            
          case simdjson::dom::element_type::ARRAY: {
            simdjson::dom::element inner_arr = arr.at(i);
            array_of_array[i] = json_to_sexp( inner_arr, simplify, fill_na, sequential_array_counter );
            sequential_array_counter++;
            break;
          }
            
          case simdjson::dom::element_type::OBJECT: {
            sequential_array_counter = 0;
            simdjson::dom::element inner_obj = arr.at(i);
            array_of_array[i] = json_to_sexp( inner_obj, simplify, fill_na, sequential_array_counter );
            break;
          }
            
          default: {
            Rcpp::stop("RcppSimdJson - case not handled");
          }
            
          } // switch
        } // for
        
        if( sequential_array_counter > 0 && simplify ) {
          res[0] = ::rcppsimdjson::from_json::list_to_matrix( array_of_array );
           
        } else if ( ::rcppsimdjson::from_json::contains_object( dtypes ) && dtypes.size() == 1 && !::rcppsimdjson::from_json::contains_array( dtypes ) && simplify ) {
          
          if( fill_na ) {
            Rcpp::Rcout << "simplify_data_frame_fill_na" << std::endl;
            res[0] = ::rcppsimdjson::from_json::simplify_dataframe_fill_na( array_of_array, json_length );
          } else {
            Rcpp::Rcout << "simplify_dataframe" << std::endl;
            res[0] = ::rcppsimdjson::from_json::simplify_dataframe( array_of_array, json_length );
          }
          
        } else {
          res[0] = array_of_array;
        }
         
      }
    } else {
      Rcpp::stop("RcppSimdJson - case not handled");
    }

    return res[0];
  }

} // from_json
} // rcppsimdjson

#endif