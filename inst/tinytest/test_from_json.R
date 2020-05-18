
library(RcppSimdJson)

## scalars

target <- list(a = 8L, b = 99.5, c = TRUE, d = "cats", e = NA)

js <- "{\"a\":8, \"b\":99.5, \"c\":true, \"d\":\"cats\", \"e\":null}"
x <- from_json(js)
expect_equal(x, target)

js <- '{"a":[8],"b":[99.5],"c":[true],"d":["cats"],"e":[null]}'
x <- from_json(js)
expect_equal(x, target)

expect_equal(from_json("1"), 1)
expect_equal(from_json("1.5"), 1.5)
expect_equal(from_json("\"a\""), "a")
expect_equal(from_json("true"), TRUE)


## vector / arrays

target <- list(a = list(1L, 2L, 3L, NA), b = list(1L, "cats", 3L, NA))
js <- "{\"a\":[1, 2, 3, null], \"b\":[1, \"cats\", 3, null]}"
x <- from_json(js, simplify = FALSE)
expect_equal(x, target)

js <- jsonify::to_json(target, unbox = T)
x <- from_json(js, simplify = FALSE)
expect_equal(x, target)

target <- list(a = c(1L, 2L, 3L, NA_integer_), b = c("1", "cats","3", NA))
js <- "{\"a\":[1, 2, 3, null], \"b\":[1, \"cats\", 3, null]}"
x <- from_json(js, simplify = TRUE)
expect_equal(x, target)

js <- '{"a":[1,2,3,null],"b":["1","cats","3",null]}'
expect_equal(from_json(js), target)

## Nested JSON

target <- list(a = 8, b = list(c = 123, d = list(e = 456, f = NA)))

js <- "{\"a\":8, \"b\":{\"c\":123, \"d\":{\"e\":456, \"f\":null}}}"
x <- from_json( js )
expect_equal(x, target)

js <- '{"a":[8.0],"b":{"c":[123.0],"d":{"e":[456.0],"f":[null]}}}'
x <- from_json( js )
expect_equal(x, target)

## Nestd JSON within an array

js <- '[{"f":"cats"}]'
target <- data.frame(f = "cats", stringsAsFactors = F)
x <- from_json( js )
expect_equal( x, target )

js <- '[{"f":"cats"}]'
expect_equal(from_json(js), target)

js <- '{"e":[{"f":"cats"}]}'
target <- list( e = data.frame(f = "cats", stringsAsFactors = F) )
x <- from_json( js )
expect_equal(x, target)

js <- '{"e":[{"f":"cats"}]}'
expect_equal(from_json(js), target)

js <- '[{"c":123,"d":456},{"e":[{"f":"cats"}]}]'
target <- list(list(c = c(123), d = c(456)), list( e = data.frame(f = "cats", stringsAsFactors = F) ) )
x <- from_json( js )
expect_equal( x, target )

js <- '[{"c":[123.0],"d":[456.0]},{"e":[{"f":"cats"}]}]'
expect_equal(from_json(js), target)

target <- list(a = 8L, b = list(list(c = 123L, d = 456L), list(e = data.frame(f = "cats", stringsAsFactors = F))))
js <- '{"a":8, "b":[{"c":123,"d":456},{"e":[{"f":"cats"}]}]}'
x <- from_json(js)
expect_equal(x, target)

js <- '{"a":[8],"b":[{"c":[123],"d":[456]},{"e":[{"f":"cats"}]}]} '
expect_equal(from_json(js), target)

## Missing keys


target <- c(1L, 2L, 3L, NA_integer_)

js <- "[1, 2, 3, null]"
x <- from_json(js)
expect_equal(x, target)

# list
target <- list(1L, "cats", 3L, NA)

js <- "[1, \"cats\", 3, null]"
x <- from_json(js, simplify = FALSE )
expect_equal(x, target)
expect_equal(from_json(js, simplify = FALSE), target)

target <- c("1","cats","3",NA)
x <- from_json(js, simplify = TRUE )
expect_equal(x, target)


## data.frames

target <- data.frame("id" = c(1L, 2L), "val" = c("a", "b"), stringsAsFactors = FALSE)

js <- '[{"id":1,"val":"a"},{"id":2,"val":"b"}]'
x <- from_json( js )
expect_equal(x, target)

js <- jsonify::to_json(target)
expect_equal(from_json(js), target)

## missing val2 in 2nd objet
target <- list(list(id = c(1), val = c("a"), val2 = c(1)), list(id = c(2), val = c("b")))
js <- '[{"id":1,"val":"a","val2":1},{"id":2,"val":"b"}]'
x <- from_json( js )
expect_equal(x, target)

js <- jsonify::to_json(target)
expect_equal(from_json(js), target)

## fill_na
target <- data.frame(id = c(1,2), val = c("a","b"), val2 = c(1,NA), stringsAsFactors = FALSE)
x <- from_json( js, fill_na = TRUE )
expect_equal(x, target)

## two entries with same key
target <- list( list(id = 1, val = "a", val = 1), list(id = 2, val = "b"))
js <- '[{"id":1,"val":"a","val":1},{"id":2,"val":"b"}]'
x <- from_json( js )
expect_equal(x, target)

js <- jsonify::to_json(target)
expect_equal(from_json(js), target)

## fill_na (on duplicate keys)
target <- data.frame(id = c(1,2), val = c("a","b"), stringsAsFactors = FALSE)
x <- from_json( js, fill_na = TRUE )
expect_equal(x, target)

# Return data frame in which the values in each name are NOT of the same data type.
target <- data.frame("id" = c("cats", 2L), val = c("a", "b"), stringsAsFactors = FALSE)
js <- '[{"id":"cats","val":"a"},{"id":2,"val":"b"}]'
x <- from_json( js )
expect_equal(x, target)

js <- jsonify::to_json(target)
expect_equal(from_json(js), target)

# Return data frame in which the names do not align across JSON objects.
target <- list( list(id = c(1L), val = c("a")), list(id = c(2L), blah = c("b")) )
## - don't simplify to a data.frame if the names in objects after the first one
## are different.
js <- '[{"id":1,"val":"a"},{"id":2,"blah":"b"}]'
x <- from_json(js)
expect_equal(x, target)

js <- jsonify::to_json(target)
expect_equal(from_json(js), target)

## fill_na 
target <- data.frame(id = c(1,2), val = c("a",NA), blah = c(NA, "b"), stringsAsFactors = F)
x <- from_json( js, fill_na = TRUE )
expect_equal( x, target )

target <- list( list(id = c(1L), val = c("a")), list(id = c(2L), blah = c(1L,2L)) )
js <- '[{"id":1,"val":"a"},{"id":2,"blah":[1,2]}]'
x <- from_json(js)
expect_equal(x, target)

js <- jsonify::to_json(target)
expect_equal(from_json(js), target)

## filL_na
target <- data.frame(id = 1:2, val = c("a",NA), stringsAsFactors = FALSE)
target$blah <- list(NA_integer_, 1:2)
x <- from_json(js, fill_na = TRUE )
expect_equal( x, target )

## 'val' changes type and length
target <- data.frame(id = c("1","2"), stringsAsFactors = F)
l <- list("a", 1:2)
target$val <- l
js <- '[{"id":"1","val":"a"},{"id":"2","val":[1,2]}]'
x <- from_json(js)
expect_equal(x, target)

js <- jsonify::to_json(target)
expect_equal(from_json(js), target)

target <- data.frame(id = c("1","2"), stringsAsFactors = F)
l <- list("a", matrix(1:2, ncol = 2) )
target$val <- l
js <- '[{"id":"1","val":["a"]},{"id":"2","val":[[1,2]]}]'
x <- from_json(js)
expect_equal(x, target)

js <- jsonify::to_json(target)
expect_equal(from_json(js), target)

js <- '[{"val":["a"]},{"val":[1,2]}]'
x <- from_json( js )
expect_equal( as.character(jsonify::to_json(x) ), js )

js <- '[{"val":"a"},{"val":[[1,2]]}]'
df <- from_json( js )
expect_equal( as.character(jsonify::to_json(df, unbox = T) ), js )

## matrices
js <- '[{"val":[[1,2]]},{"val":[[1,2]]}]'
x <- from_json( js )
expect_equal( as.character(jsonify::to_json(x, unbox = T) ), js )

## complex df columns
js <- '[{"val":[[1,{"a":1,"b":2}]]},{"val":[[1,2]]}]'
x <- from_json( js )
expect_equal( as.character(jsonify::to_json(x, unbox = T) ), js )


## different type
js <- '[{"val":[[1,2]]},{"val":[["a","b"]]}]'
x <- from_json( js )
expect_equal( as.character(jsonify::to_json(x, unbox = T) ), js )

js <- '[{"val":[[1,2],[3,4]]},{"val":[["a","b"]]}]'
x <- from_json( js )
expect_equal( as.character(jsonify::to_json(x, unbox = T) ), js )


## something more complex
js <- '[{"val":{"inner_val":[[1,2],[3,4]]}},{"val":[["a","b"]]}]'
x <- from_json( js )
expect_equal( as.character(jsonify::to_json(x, unbox = T) ), js )

js <- '[{"val":{"inner_val":[[1,2],[3,4]]}},{"val":{"inner_val":[["a","b"]]}}]'
x <- from_json( js )
expect_equal( as.character(jsonify::to_json(x, unbox = T) ), js )

## other complex stuff
df1 <- data.frame(
  x = 1:2
  , y = 3:4
  , stringsAsFactors = T
)
df2 <- data.frame(
  z = c("a","b")
  , m = c("c","d")
  , stringsAsFactors = F
)

l <- list( df1, df2 )
js <-jsonify::to_json( l )
x <- from_json( js )
expect_equal(x, l)

df1$z <- df2

js <-jsonify::to_json( df1 )
x <- from_json( js )
expect_equal(x, df1)

## shouldn't be a data.frame
js <- '{"id":1,"val":2}'
x <- from_json( js )
expect_equal( as.character(jsonify::to_json(x, unbox = T) ), js )

## should be a data.frame
js <- '[{"id":1,"val":2}]'
x <- from_json( js )
expect_equal( as.character(jsonify::to_json(x, unbox = T) ), js )

## should be a list
l <- list(1,2,df)
js <- '[1,2,{"id":1,"val":2}]'
x <- from_json( js )
expect_equal( as.character(jsonify::to_json(x, unbox = T) ), js )

## should be a list with a data.frame element
js <- '[1,2,[{"id":1,"val":2}]]'
x <- from_json( js )
expect_equal( as.character(jsonify::to_json(x, unbox = T) ), js )

df <- data.frame( id = 1:2, mat = I(matrix(1:4, ncol = 2)), stringsAsFactors = TRUE )
js <- jsonify::to_json( df )
x <- from_json( js )
## I can't recreate 'AsIs' columns

expect_equal(x$id, df$id)
expect_equal(x$mat, unclass( df$mat) )
expect_equal( jsonify::to_json(x, unbox = T), js )

## Issue 42
df <- structure(list(fill_colour = structure(c(68, 49, 53, 253, 1,
                                               104, 183, 231, 84, 142, 121, 37, 255, 255, 255, 255), .Dim = c(4L,
                                                                                                              4L)), geometry = c(1, 2, -5, 0.3), lat = 1:4, lon = c(1, 2, -5,
                                                                                                                                                                    0.3)), class = "data.frame", row.names = c(NA, 4L))

js <- jsonify::to_json( df )
res <- from_json( js )
expect_equal( res, df )

