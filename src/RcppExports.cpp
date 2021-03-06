// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// validateJSON
bool validateJSON(const std::string filename);
RcppExport SEXP _RcppSimdJson_validateJSON(SEXP filenameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const std::string >::type filename(filenameSEXP);
    rcpp_result_gen = Rcpp::wrap(validateJSON(filename));
    return rcpp_result_gen;
END_RCPP
}
// parseExample
void parseExample();
RcppExport SEXP _RcppSimdJson_parseExample() {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    parseExample();
    return R_NilValue;
END_RCPP
}
// cppVersion
int cppVersion();
RcppExport SEXP _RcppSimdJson_cppVersion() {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    rcpp_result_gen = Rcpp::wrap(cppVersion());
    return rcpp_result_gen;
END_RCPP
}
// unsupportedArchitecture
bool unsupportedArchitecture();
RcppExport SEXP _RcppSimdJson_unsupportedArchitecture() {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    rcpp_result_gen = Rcpp::wrap(unsupportedArchitecture());
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_RcppSimdJson_validateJSON", (DL_FUNC) &_RcppSimdJson_validateJSON, 1},
    {"_RcppSimdJson_parseExample", (DL_FUNC) &_RcppSimdJson_parseExample, 0},
    {"_RcppSimdJson_cppVersion", (DL_FUNC) &_RcppSimdJson_cppVersion, 0},
    {"_RcppSimdJson_unsupportedArchitecture", (DL_FUNC) &_RcppSimdJson_unsupportedArchitecture, 0},
    {NULL, NULL, 0}
};

RcppExport void R_init_RcppSimdJson(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
