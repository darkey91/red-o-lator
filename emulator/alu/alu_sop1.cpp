//
// Created by Diana Kudaiberdieva
//

#include <stdexcept>
#include "alu.h"

static inline void run_s_abs_i32(WfStateSOP1& state) {
    auto SRC0 = static_cast<int32_t>(state.SSRC0);
    state.SDST = static_cast<uint64_t>(SRC0 < 0 ? -SRC0 : SRC0) & 0xffffffff;
    state.SCC = state.SDST != 0;
}

static inline void run_s_and_saveexec_b64(WfStateSOP1& state) {
    state.SDST = state.EXEC;
    state.EXEC = state.SSRC0 & state.EXEC;
    state.SCC = state.EXEC != 0;
}

static inline void run_s_andn1_saveexec_b64(WfStateSOP1& state) {
    state.SDST = state.EXEC;
    state.EXEC = ~state.SSRC0 & state.EXEC;
    state.SCC = state.EXEC != 0;
}

static inline void run_s_andn1_wrexec_b64(WfStateSOP1& state) {
    state.EXEC = ~state.SSRC0 & state.EXEC;
    state.SDST = state.EXEC;
    state.SCC = state.EXEC != 0;
}

static inline void run_s_andn2_saveexec_b64(WfStateSOP1& state) {
    state.SDST = state.EXEC;
    state.EXEC = state.SSRC0 & ~state.EXEC;
    state.SCC = state.EXEC != 0;
}

static inline void run_s_andn2_wrexec_b64(WfStateSOP1& state) {
    state.EXEC = state.SSRC0 & ~state.EXEC;
    state.SDST = state.EXEC;
    state.SCC = state.EXEC != 0;
}

static inline void run_s_bcnt0_i32_b32(WfStateSOP1& state) {
    state.SDST = bit_count(~(static_cast<uint32_t>(state.SSRC0)));
    state.SCC = state.SDST != 0;
}

static inline void run_s_bcnt0_i32_b64(WfStateSOP1& state) {
    state.SDST = bit_count(~state.SSRC0);
    state.SCC = state.SDST != 0;
}

static inline void run_s_bcnt1_i32_b32(WfStateSOP1& state) {
    state.SDST = bit_count(static_cast<uint32_t>(state.SSRC0));
    state.SCC = state.SDST != 0;
}

static inline void run_s_bcnt1_i32_b64(WfStateSOP1& state) {
    state.SDST = bit_count(state.SSRC0);
    state.SCC = state.SDST != 0;
}

static inline void run_s_bitreplicate_b64_b32(WfStateSOP1& state) {
    state.SDST = 0;
    auto SSRC0 = static_cast<uint32_t>(state.SSRC0);
    for (uint8_t i = 0; i < 32; i++) {
        state.SDST |= (((SSRC0 >> i) & 1) * 3) << (i << 1);
    }
}

static inline void run_s_bitset0_b32(WfStateSOP1& state) {
    state.SDST &= ~(uint32_t(1) << (state.SSRC0 & 31));
}

static inline void run_s_bitset0_b64(WfStateSOP1& state) {
    state.SDST &= ~(uint64_t(1) << (state.SSRC0 & 63));
}

static inline void run_s_bitset1_b32(WfStateSOP1& state) {
    state.SDST |= uint32_t(1) << (state.SSRC0 & 31);
}

static inline void run_s_bitset1_b64(WfStateSOP1& state) {
    state.SDST |= uint64_t(1) << (state.SSRC0 & 63);
}

static inline void run_s_brev_b32(WfStateSOP1& state) {
    state.SDST = rev_bit((uint32_t) state.SSRC0);
}

static inline void run_s_brev_b64(WfStateSOP1& state) {
    state.SDST = rev_bit(state.SSRC0);
}

static inline void run_s_cbranch_join(WfStateSOP1& state, Wavefront* wf) {
    uint8_t csp = state.MODE->csp();
    if (uint64_t(csp) == state.SSRC0) {
        state.PC->add(4);
    } else {
        csp--;
        state.EXEC = wf->read_sgpr_pair(csp * 4);
        uint64_t value =wf->read_sgpr_pair(csp * 4 + 2);
        state.PC->set_value(value);
    }
}

static inline void run_s_cmov_b32(WfStateSOP1& state) {
    if (state.SCC) state.SDST = static_cast<uint32_t>(state.SSRC0);
}

static inline void run_s_cmov_b64(WfStateSOP1& state) {
    if (state.SCC) state.SDST = state.SSRC0;
}

static inline void run_s_ff0_i32_b32(WfStateSOP1& state) {
    int32_t SDST = -1;
    auto SRC0 = (uint32_t) state.SSRC0;
    for (uint8_t i = 0; i < 32; i++) {
        if (((uint32_t(1) << i) & SRC0) == 0) {
            SDST = i;
            break;
        }
    }
    state.SDST = SDST & 0xffffffff;
}

static inline void run_s_ff0_i32_b64(WfStateSOP1& state) {
    state.SDST = int32_t(-1);
    for (uint8_t i = 0; i < 64; i++) {
        if (((uint64_t(1) << i) & state.SSRC0) == 0) {
            state.SDST = i;
            break;
        }
    }
}

static inline void run_s_ff1_i32_b32(WfStateSOP1& state) {
    int32_t SDST = -1;
    auto SRC0 = (uint32_t) state.SSRC0;
    for (uint8_t i = 0; i < 32; i++) {
        if (((uint32_t(1) << i) & SRC0) != 0) {
            SDST = i;
            break;
        }
    }
    state.SDST = SDST & 0xffffffff;
}

static inline void run_s_ff1_i32_b64(WfStateSOP1& state) {
    state.SDST = int32_t(-1);
    auto SRC0 = (uint64_t) state.SSRC0;
    for (uint8_t i = 0; i < 64; i++) {
        if (((uint64_t(1) << i) & SRC0) != 0) {
            state.SDST = i;
            break;
        }
    }
}

static inline void run_s_flbit_i32_b32(WfStateSOP1& state) {
    int32_t SDST = -1;
    auto SRC0 = (uint32_t) state.SSRC0;
    for (int8_t i = 31; i >= 0; i--) {
        if (((uint32_t(1) << i) & SRC0) != 0) {
            SDST = 31 - i;
            break;
        }
    }
    state.SDST = SDST & 0xffffffff;
}

static inline void run_s_flbit_i32_b64(WfStateSOP1& state) {
    state.SDST = int32_t(-1);
    auto SRC0 = (uint64_t) state.SSRC0;
    for (int8_t i = 63; i >= 0; i--) {
        if (((uint64_t(1) << i) & SRC0) != 0) {
            state.SDST = 63 - i;
            break;
        }
    }
}

static inline void run_s_flbit_i32(WfStateSOP1& state) {
    int32_t SDST = -1;
    auto SRC0 = (int32_t) state.SSRC0;
    uint32_t bitval = SRC0 >= 0 ? 1 : 0;
    for (int8_t i = 31; i >= 0; i--) {
        if (((uint32_t(1) << i) & SRC0) == (bitval << i)) {
            SDST = 31 - i;
            break;
        }
    }
    state.SDST = SDST & 0xffffffff;
}

static inline void run_s_flbit_i32_i64(WfStateSOP1& state) {
    state.SDST = int32_t(-1);
    auto SRC0 = (int64_t) state.SSRC0;
    uint64_t bitval = SRC0 >= 0 ? 1 : 0;
    for (int8_t i = 63; i >= 0; i--) {
        if (((uint64_t(1) << i) & SRC0) == (bitval << i)) {
            state.SDST = 63 - i;
            break;
        }
    }
}

static inline void run_s_getpc_b64(WfStateSOP1& state) {
    state.SDST = state.PC->get_value() + 4;
}

static inline void run_s_mov_b32(WfStateSOP1& state) {
    state.SDST = (uint32_t) state.SSRC0;
}

static inline void run_s_mov_b64(WfStateSOP1& state) {
    state.SDST = state.SSRC0;
}

static inline void run_s_movreld_b32(WfStateSOP1& state, Wavefront* wf) {
    assert(is_s_reg(state.DEST_TYPE));
    wf->scalarRegFile[state.DEST_TYPE - S0 + state.M0] =
        static_cast<uint32_t>(state.SSRC0);
}

static inline void run_s_movreld_b64(WfStateSOP1& state, Wavefront* wf) {
    assert(is_s_reg(state.DEST_TYPE));
    wf->set_sgpr_pair(state.DEST_TYPE - S0 + state.M0, state.SSRC0);
}

static inline void run_s_movrels_b32(WfStateSOP1& state, Wavefront* wf) {
    assert(is_s_reg(state.SRC0_TYPE));
    state.SDST = wf->scalarRegFile[state.SRC0_TYPE - S0 + state.M0];
}

static inline void run_s_movrels_b64(WfStateSOP1& state, Wavefront* wf) {
    assert(is_s_reg(state.SRC0_TYPE));
    state.SDST = wf->read_sgpr_pair(state.SRC0_TYPE - S0 + state.M0);
}

static inline void run_s_nand_saveexec_b64(WfStateSOP1& state) {
    state.SDST = state.EXEC;
    state.EXEC = ~(state.SSRC0 & state.EXEC);
    state.SCC = state.EXEC != 0;
}

static inline void run_s_nor_saveexec_b64(WfStateSOP1& state) {
    state.SDST = state.EXEC;
    state.EXEC = ~(state.SSRC0 | state.EXEC);
    state.SCC = state.EXEC != 0;
}

static inline void run_s_not_b32(WfStateSOP1& state) {
    state.SDST = ~((uint32_t) state.SSRC0);
    state.SCC = state.SDST != 0;
}

static inline void run_s_not_b64(WfStateSOP1& state) {
    state.SDST = ~state.SSRC0;
    state.SCC = state.SDST != 0;
}

static inline void run_s_or_saveexec_b64(WfStateSOP1& state) {
    state.SDST = state.EXEC;
    state.EXEC = state.SSRC0 | state.EXEC;
    state.SCC = state.EXEC != 0;
}

static inline void run_s_orn2_saveexec_b64(WfStateSOP1& state) {
    state.SDST = state.EXEC;
    state.EXEC = ~state.SSRC0 & state.EXEC;
    state.SCC = state.EXEC != 0;
}

static inline void run_s_quadmask_b32(WfStateSOP1& state) {
    uint32_t temp = 0;
    auto SRC0 = (uint32_t) state.SSRC0;
    for (uint8_t i = 0; i < 8; i++) {
        temp |= ((SRC0 >> (i << 2)) & 15) != 0 ? (uint32_t(1) << i) : i;
    }
    state.SDST = temp;
    state.SCC = state.SDST != 0;
}

static inline void run_s_quadmask_b64(WfStateSOP1& state) {
    uint64_t temp = 0;
    for (uint8_t i = 0; i < 16; i++) {
        temp |= ((state.SSRC0 >> (i << 2)) & 15) != 0 ? (uint32_t(1) << i) : i;
    }
    state.SDST = temp;
    state.SCC = state.SDST != 0;
}

// Return from exception handler and continue. This instruction may
// only be used within a trap handler.
static inline void run_s_rfe_b64(WfStateSOP1& state) {
    state.STATUS->priv(0);
    state.PC->set_value(state.SSRC0);
}

static inline void run_s_set_gpr_idx_idx(WfStateSOP1& state) {
    state.M0 = (state.M0 & 0xffffff00) | (((uint32_t) state.SSRC0) & 0xff);
}

static inline void run_s_setpc_b64(WfStateSOP1& state) {
    state.PC->set_value(state.SSRC0);
}

static inline void run_s_sext_i32_i8(WfStateSOP1& state) {
    state.SDST = sign_ext((int8_t) state.SSRC0);
}

static inline void run_s_sext_i32_i16(WfStateSOP1& state) {
    state.SDST = sign_ext((int16_t) state.SSRC0);
}

static inline void run_s_swappc_b64(WfStateSOP1& state) {
    state.SDST = state.PC->get_value() + 4;
    state.PC->set_value(state.SSRC0);
}

static inline void run_s_wqm_b32(WfStateSOP1& state) {
    uint32_t temp = 0;
    auto SRC0 = (uint32_t) state.SSRC0;
    for (uint8_t i = 0; i < 32; i += 4) {
        temp |= ((SRC0 >> i) & 15) != 0 ? (uint32_t(15) << i) : 0;
    }
    state.SDST = temp;
    state.SCC = state.SDST != 0;
}

static inline void run_s_wqm_b64(WfStateSOP1& state) {
    uint64_t temp = 0;
    for (uint8_t i = 0; i < 64; i += 4) {
        temp |= ((state.SSRC0 >> i) & 15) != 0 ? (uint64_t(15) << i) : 0;
    }
    state.SDST = temp;
    state.SCC = state.SDST != 0;
}

static inline void run_s_xnor_saveexec_b64(WfStateSOP1& state) {
    state.SDST = state.EXEC;
    state.EXEC = ~(state.SSRC0 ^ state.EXEC);
    state.SCC = state.EXEC != 0;
}

static inline void run_s_xor_saveexec_b64(WfStateSOP1& state) {
    state.SDST = state.EXEC;
    state.EXEC = state.SSRC0 ^ state.EXEC;
    state.SCC = state.EXEC != 0;
}

void run_sop1(const Instruction& instr, Wavefront* wf) {
    assert(instr.get_operands_count() <= 2 && "Unexpected amount of operands for SOP1");

    auto state = wf->get_sop1_state(instr);

    switch (instr.get_key()) {
        case S_CBRANCH_JOIN:
            assert(instr.get_operands_count() == 1);
            state.SSRC0 = to_uin64_t(wf->read_operand(*instr[0]));
            run_s_cbranch_join(state, wf);
            break;
        case S_SET_GPR_IDX_IDX:
            assert(instr.get_operands_count() == 1);
            state.SSRC0 = to_uin64_t(wf->read_operand(*instr[0]));
            run_s_set_gpr_idx_idx(state);
            break;
        case S_RFE_B64:
            assert(instr.get_operands_count() == 1);
            state.SSRC0 = to_uin64_t(wf->read_operand(*instr[0]));
            run_s_rfe_b64(state);
            break;
        case S_SETPC_B64:
            assert(instr.get_operands_count() == 1);
            state.SSRC0 = to_uin64_t(wf->read_operand(*instr[0]));
            run_s_setpc_b64(state);
            break;
        case S_GETPC_B64:
            assert(instr.get_operands_count() == 1);
            state.SDST = to_uin64_t(wf->read_operand(*instr[0]));
            run_s_getpc_b64(state);
            wf->write_operand_to_gpr(*instr[0], state.SDST);
            break;
        case S_ABS_I32:
            run_s_abs_i32(state);
            break;
        case S_AND_SAVEEXEC_B64:
            run_s_and_saveexec_b64(state);
            break;
        case S_ANDN1_SAVEEXEC_B64:
            run_s_andn1_saveexec_b64(state);
            break;
        case S_ANDN1_WREXEC_B64:
            run_s_andn1_wrexec_b64(state);
            break;
        case S_ANDN2_SAVEEXEC_B64:
            run_s_andn2_saveexec_b64(state);
            break;
        case S_ANDN2_WREXEC_B64:
            run_s_andn2_wrexec_b64(state);
            break;
        case S_BCNT0_I32_B32:
            run_s_bcnt0_i32_b32(state);
            break;
        case S_BCNT0_I32_B64:
            run_s_bcnt0_i32_b64(state);
            break;
        case S_BCNT1_I32_B32:
            run_s_bcnt1_i32_b32(state);
            break;
        case S_BCNT1_I32_B64:
            run_s_bcnt1_i32_b64(state);
            break;
        case S_BITREPLICATE_B64_B32:
            run_s_bitreplicate_b64_b32(state);
            break;
        case S_BITSET0_B32:
            run_s_bitset0_b32(state);
            break;
        case S_BITSET0_B64:
            run_s_bitset0_b64(state);
            break;
        case S_BITSET1_B32:
            run_s_bitset1_b32(state);
            break;
        case S_BITSET1_B64:
            run_s_bitset1_b64(state);
            break;
        case S_BREV_B32:
            run_s_brev_b32(state);
            break;
        case S_BREV_B64:
            run_s_brev_b64(state);
            break;
        case S_CMOV_B32:
            run_s_cmov_b32(state);
            break;
        case S_CMOV_B64:
            run_s_cmov_b64(state);
            break;
        case S_FF0_I32_B32:
            run_s_ff0_i32_b32(state);
            break;
        case S_FF0_I32_B64:
            run_s_ff0_i32_b64(state);
            break;
        case S_FF1_I32_B32:
            run_s_ff1_i32_b32(state);
            break;
        case S_FF1_I32_B64:
            run_s_ff1_i32_b64(state);
            break;
        case S_FLBIT_I32_B32:
            run_s_flbit_i32_b32(state);
            break;
        case S_FLBIT_I32_B64:
            run_s_flbit_i32_b64(state);
            break;
        case S_FLBIT_I32:
            run_s_flbit_i32(state);
            break;
        case S_FLBIT_I32_I64:
            run_s_flbit_i32_i64(state);
            break;
        case S_MOV_B32:
            run_s_mov_b32(state);
            break;
        case S_MOV_B64:
            run_s_mov_b64(state);
            break;
        case S_MOVRELD_B32:
            run_s_movreld_b32(state, wf);
            break;
        case S_MOVRELD_B64:
            run_s_movreld_b64(state, wf);
            break;
        case S_MOVRELS_B32:
            run_s_movrels_b32(state, wf);
            break;
        case S_MOVRELS_B64:
            run_s_movrels_b64(state, wf);
            break;
        case S_NAND_SAVEEXEC_B64:
            run_s_nand_saveexec_b64(state);
            break;
        case S_NOR_SAVEEXEC_B64:
            run_s_nor_saveexec_b64(state);
            break;
        case S_NOT_B32:
            run_s_not_b32(state);
            break;
        case S_NOT_B64:
            run_s_not_b64(state);
            break;
        case S_OR_SAVEEXEC_B64:
            run_s_or_saveexec_b64(state);
            break;
        case S_ORN2_SAVEEXEC_B64:
            run_s_orn2_saveexec_b64(state);
            break;
        case S_QUADMASK_B32:
            run_s_quadmask_b32(state);
            break;
        case S_QUADMASK_B64:
            run_s_quadmask_b64(state);
            break;
        case S_SEXT_I32_I8:
            run_s_sext_i32_i8(state);
            break;
        case S_SEXT_I32_I16:
            run_s_sext_i32_i16(state);
            break;
        case S_SWAPPC_B64:
            run_s_swappc_b64(state);
            break;
        case S_WQM_B32:
            run_s_wqm_b32(state);
            break;
        case S_WQM_B64:
            run_s_wqm_b64(state);
            break;
        case S_XNOR_SAVEEXEC_B64:
            run_s_xnor_saveexec_b64(state);
            break;
        case S_XOR_SAVEEXEC_B64:
            run_s_xor_saveexec_b64(state);
            break;
        default:
            assert(false && "Unknown instruction met!");
    }

    wf->update_with_sop1_state(instr, state);
}