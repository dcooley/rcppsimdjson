#' From JSON
#' 
#' Converts JSON to an R object. 
#' 
#' @param json JSON to convert to R object. Can be a string, url or link to a file.
#' @param simplify logical, if \code{TRUE}, coerces JSON to the simplest R object possible. See Details
#' @param fill_na logical, if \code{TRUE} and \code{simplify} is \code{TRUE}, 
#' data.frames will be na-filled if there are missing JSON keys.
#' Ignored if \code{simplify} is \code{FALSE}. See details and examples.
#' @details 
#' 
#' When \code{simplify = TRUE}
#' \itemize{
#'   \item{single arrays are coerced to vectors}
#'   \item{array of arrays (all the same length) are coerced to matrices}
#'   \item{objects with the same keys are coerced to data.frames}
#' }
#' 
#' When \code{simplify = TRUE} and \code{fill_na = TRUE}
#' \itemize{
#'   \item{objects are coerced to data.frames, and any missing values are filled with NAs}
#' }
#' 
#' @examples 
#' 
#' from_json('{"a":[1, 2, 3]}')
#' from_json('{"a":8, "b":99.5, "c":true, "d":"cats", "e":[1, "cats", 3]}')
#' from_json('{"a":8, "b":{"c":123, "d":{"e":456}}}')
#'  
#' ## Return a data frame
#' from_json('[{"id":1,"val":"a"},{"id":2,"val":"b"}]')
#' 
#' ## Return a data frame with a list column
#' from_json('[{"id":1,"val":"a"},{"id":2,"val":["b","c"]}]')
#' 
#' ## Without simplifying to a data.frame
#' from_json('[{"id":1,"val":"a"},{"id":2,"val":["b","c"]}]', simplify = FALSE )
#' 
#' ## Missing JSON keys 
#' from_json('[{"x":1},{"x":2,"y":"hello"}]')
#' 
#' ## Missing JSON keys - filling with NAs
#' from_json('[{"x":1},{"x":2,"y":"hello"}]', fill_na = TRUE )
#' 
#' ## Duplicate object keys
#' from_json('[{"x":1,"x":"a"},{"x":2,"x":"b"}]')
#' 
#' from_json('[{"id":1,"val":"a","val":1},{"id":2,"val":"b"}]', fill_na = TRUE )
#' 
#' 
#' @export
from_json <- function(json, simplify = TRUE, fill_na = FALSE ) {
  rcpp_from_json( json, simplify, fill_na )
}