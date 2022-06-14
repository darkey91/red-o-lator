#include <common/test/doctest.h>

#include "alu/alu.h"

//static inline void fill_empty_wi(Wavefront& wf, size_t wiAmount) {
//    assert(wiAmount <= 64);
//
//    for (size_t i = 0; i < wiAmount; ++i) {
//        wf.workItems.push_back(nullptr);
//        wf.execReg |= uint64_t(1) << i;
//    }
//}
//
////VOP1
//TEST_CASE("run_v_mov_b32") {
//    Instruction instruction = Instruction(0,std::string( "v_mov_b32"), {"v3", "s1"});
//    auto* wavefront = new Wavefront(nullptr, 2, 4);
//    fill_empty_wi(wavefront, 64);
//
//    SUBCASE("value from s1 should be in all v0") {
//        wavefront.scalarRegFile[1] = 0x12345678;
//        run_vop1(instruction&, wavefront);
//        for (size_t wi = 0; wi < wavefront.workItems.size(); ++wi) {
//            CHECK(wavefront.read_vgpr(wi, 3) == 0x12345678);
//        }
//    }
//
//
//}
