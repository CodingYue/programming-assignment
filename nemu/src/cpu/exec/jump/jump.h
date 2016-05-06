#ifndef __JUMP_H__
#define __JUMP_H__


make_helper(call);
make_helper(jmp_b);
make_helper(je_b);
make_helper(jc_b);
make_helper(jbe_b);
make_helper(jne_b);
make_helper(jle_b);
make_helper(jg_b);
make_helper(jge_b);
make_helper(jl_b);
make_helper(ja_b);
make_helper(jmp_rm_b);

make_helper(jmp_v);
make_helper(je_v);
make_helper(jc_v);
make_helper(jbe_v);
make_helper(jne_v);
make_helper(jle_v);
make_helper(jg_v);
make_helper(jge_v);
make_helper(jl_v);
make_helper(ja_v);
make_helper(jmp_rm_v);

#endif