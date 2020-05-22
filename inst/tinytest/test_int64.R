expect_identical(
  RcppSimdJson:::.check_int64(is_signed = TRUE, integer64 = 0),
  3e9
)
expect_identical(
  RcppSimdJson:::.check_int64(is_signed = FALSE, integer64 = 0),
  3e9
)

expect_identical(
  RcppSimdJson:::.check_int64(is_signed = TRUE, integer64 = 1),
  "3000000000"
)
expect_identical(
  RcppSimdJson:::.check_int64(is_signed = FALSE, integer64 = 1),
  "3000000000"
)

expect_equal(
  RcppSimdJson:::.check_int64(is_signed = TRUE, integer64 = 2),
  structure(1.48219693752374e-314, class = "integer64")
)
expect_equal(
  RcppSimdJson:::.check_int64(is_signed = FALSE, integer64 = 2),
  structure(1.48219693752374e-314, class = "integer64")
)

