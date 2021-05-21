//
// Created by Diana Kudaiberdieva
//

#include "alu.h"

static inline void run_v_add_u32(WfStateVOP2& state, size_t wiInd) {
    state.VDST[wiInd] = state.SRC0[wiInd] + state.SRC1[wiInd];
}

static inline void run_v_sub_u32(WfStateVOP2& state, size_t wiInd) {
    state.VDST[wiInd] = state.SRC0[wiInd] - state.SRC1[wiInd];
}

void run_vop2(const Instruction& instr, Wavefront* wf) {
    auto state = wf->get_vop2_state(instr);
    for (size_t wiInd = 0; wiInd < wf->workItems.size(); ++wiInd) {
        if (!wf->work_item_masked(wiInd)) continue;

        switch (instr.get_key()) {
            case V_ADD_U32:
                run_v_add_u32(state, wiInd);
                break;
            case V_SUB_U32:
                run_v_sub_u32(state, wiInd);
                break;
        }
    }
    wf->update_with_vop2_state(instr, state);
}