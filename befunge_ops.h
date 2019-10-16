#ifndef BEFUNGE_OPS_H
#define BEFUNGE_OPS_H

#include "befunge.h"

#define BEF_OP(OP) int bef_op_ ## OP(bef_state_t *state)

BEF_OP(digit);
BEF_OP(add);
BEF_OP(sub);
BEF_OP(mul);
BEF_OP(div);
BEF_OP(mod);
BEF_OP(not);
BEF_OP(gt);
BEF_OP(look_right);
BEF_OP(look_left);
BEF_OP(look_up);
BEF_OP(look_down);
BEF_OP(look_rand_any);
BEF_OP(look_rand_hor);
BEF_OP(look_rand_ver);
BEF_OP(dupe);
BEF_OP(swap);
BEF_OP(discard);
BEF_OP(print_int);
BEF_OP(print_char);
BEF_OP(skip);
BEF_OP(put);
BEF_OP(get);
BEF_OP(scan_int);
BEF_OP(scan_char);
// BEF_OP(exit);
BEF_OP(error);

#endif /* BEFUNGE_OPS_H */
