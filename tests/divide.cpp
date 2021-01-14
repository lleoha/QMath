#include "gtest/gtest.h"
#include "qmath/qfloat.hpp"

using qfloat8_t = QFloat<int16_t, int32_t, 8, true, true>;

TEST(Divide, OneBitPrecision) {
    for (auto l = 1; l <= 0xffff; ++l) {
        for (auto r = 1; r <= 0xffff; ++r) {
            auto li = static_cast<int16_t>(l);
            auto ri = static_cast<int16_t>(r);
            auto lq = qfloat8_t(li, nullptr);
            auto rq = qfloat8_t(ri, nullptr);
            auto lf = float(lq);
            auto rf = float(rq);

            auto resultFloat = std::max(std::min(lf / rf, 128.f - (1.f / 256)), -128.f);
            auto resultQ = lq / rq;
            auto diff = std::abs(float(resultFloat) - float(resultQ));
            ASSERT_LE(diff, 1.f / 256);
        }
    }
    \
}
