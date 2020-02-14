

library(RcppSimdJson)

## objects
js <- '{}'
expect_true( RcppSimdJson:::rcpp_get_json_length(js) == 0 )
js <- '{"x":1,"y":2}'
expect_true( RcppSimdJson:::rcpp_get_json_length(js) == 2 )
js <- '{"x":1,"y":2,"z":[1,2,3]}'
expect_true( RcppSimdJson:::rcpp_get_json_length(js) == 3 )
js <- '{"x":1,"y":2,"z":[1,2,3,{"a":[1,2,3]}]}'
expect_true( RcppSimdJson:::rcpp_get_json_length(js) == 3 )


## arrays
js <- '[]'
expect_true( RcppSimdJson:::rcpp_get_json_length(js) == 0 )
js <- '[{"x":1}]'
expect_true( RcppSimdJson:::rcpp_get_json_length(js) == 1 )
