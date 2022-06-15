/* Disassembling 'MatVecMulUncoalesced0-gfx900.bin' */
.amdcl2
.gpu GFX901
.64bit
.arch_minor 0
.arch_stepping 1
.driver_version 226400
.kernel MatVecMulUncoalesced0
    .config
        .dims x
        .sgprsnum 15
        .vgprsnum 7
        .floatmode 0xc0
        .pgmrsrc1 0x00ac0081
        .pgmrsrc2 0x00000094
        .dx10clamp
        .ieeemode
        .useargs
        .usesetup
        .priority 0
        .arg _.global_offset_0, "size_t", long
        .arg _.global_offset_1, "size_t", long
        .arg _.global_offset_2, "size_t", long
        .arg _.printf_buffer, "size_t", void*, global, , rdonly
        .arg _.vqueue_pointer, "size_t", long
        .arg _.aqlwrap_pointer, "size_t", long
        .arg M, "float*", float*, global, const, rdonly
        .arg V, "float*", float*, global, const, rdonly
        .arg width, "uint", uint
        .arg height, "uint", uint
        .arg W, "float*", float*, global,
    .text
/*000000000000*/ s_load_dword    s0, s[4:5], 0x4
/*000000000008*/ s_load_dword    s1, s[6:7], 0x0
/*000000000010*/ s_load_dword    s2, s[6:7], 0x44
/*000000000018*/ s_waitcnt       lgkmcnt(0)
/*00000000001c*/ s_and_b32       s0, s0, 0xffff
/*000000000024*/ s_mul_i32       s0, s0, s10
/*000000000028*/ v_add_u32       v0, s0, v0
/*00000000002c*/ v_add_u32       v5, s1, v0
/*000000000030*/ s_mov_b64       s[0:1], exec
/*000000000034*/ v_cmpx_lt_u32   s[2:3], v5, s2
/*00000000003c*/ s_cbranch_execz .L272_0
/*000000000040*/ s_load_dword    s4, s[6:7], 0x40
/*000000000048*/ s_waitcnt       lgkmcnt(0)
/*00000000004c*/ s_cmp_eq_i32    s4, 0
/*000000000050*/ s_cbranch_scc1  .L224_0
/*000000000054*/ s_load_dwordx4  s[8:11], s[6:7], 0x30
/*00000000005c*/ v_mul_lo_u32    v0, v5, s4
/*000000000064*/ v_mov_b32       v1, 0
/*000000000068*/ v_lshlrev_b64   v[0:1], 2, v[0:1]
/*000000000070*/ s_waitcnt       lgkmcnt(0)
/*000000000074*/ v_add_co_u32    v0, vcc, s8, v0
/*000000000078*/ v_mov_b32       v3, s9
/*00000000007c*/ v_addc_co_u32   v1, vcc, v3, v1, vcc
/*000000000080*/ s_mov_b32       s2, s10
/*000000000084*/ s_mov_b32       s3, s11
/*000000000088*/ v_mov_b32       v3, 0
/*00000000008c*/ s_nop           0x0
/*000000000090*/ s_nop           0x0
/*000000000094*/ s_nop           0x0
/*000000000098*/ s_nop           0x0
/*00000000009c*/ s_nop           0x0
.L160_0:
/*0000000000a0*/ global_load_dword v4, v[0:1], off
/*0000000000a8*/ s_load_dword    s5, s[2:3], 0x0
/*0000000000b0*/ v_add_co_u32    v0, vcc, v0, 4
/*0000000000b8*/ v_addc_co_u32   v1, vcc, v1, 0, vcc
/*0000000000c0*/ s_add_u32       s2, s2, 4
/*0000000000c4*/ s_addc_u32      s3, s3, 0
/*0000000000c8*/ s_add_u32       s4, s4, -1
/*0000000000cc*/ s_waitcnt       vmcnt(0) & lgkmcnt(0)
/*0000000000d0*/ v_mac_f32       v3, s5, v4
/*0000000000d4*/ s_cmp_lg_i32    s4, 0
/*0000000000d8*/ s_cbranch_scc1  .L160_0
/*0000000000dc*/ s_branch        .L228_0
.L224_0:
/*0000000000e0*/ v_mov_b32       v3, 0
.L228_0:
/*0000000000e4*/ s_load_dwordx2  s[2:3], s[6:7], 0x48
/*0000000000ec*/ v_mov_b32       v6, 0
/*0000000000f0*/ v_lshlrev_b64   v[0:1], 2, v[5:6]
/*0000000000f8*/ s_waitcnt       lgkmcnt(0)
/*0000000000fc*/ v_add_co_u32    v0, vcc, s2, v0
/*000000000100*/ v_mov_b32       v2, s3
/*000000000104*/ v_addc_co_u32   v1, vcc, v2, v1, vcc
/*000000000108*/ global_store_dword v[0:1], v3, off
.L272_0:
/*000000000110*/ s_endpgm
