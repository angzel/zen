// This is a helper file. no source includes this.

// These preprocessor macros maybe useful

// use debug mode
ZEN_DEBUG=1 

// all 'ZEN_P_xx_zz' are preprocessor macro
// in code the 'ZEN_xx_zz' are defined.
namespace std {}

// visual app or command line tool
// usually on pc, mac or linux.
// values:
//   not defined or 0: this is a command line tool
//   1: this is a visual window app
ZEN_P_RUNTIME_VISUAL=1
