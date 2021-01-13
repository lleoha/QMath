#include <cstdint>
#include <cstdio>
#include <iostream>
#include "include/qmath.hpp"

using qfloat8_t = QFloat<int16_t, int32_t, 8, true, true>;

int main(int argc, char *argv[]) {
    auto maxP = 0.0L;
    auto totalP = 0.0L;
    auto count = 0.0L;
    for (auto l = 1; l <= 0xffff; ++l) {
        for (auto r = 1; r <= 0xffff; ++r) {
            auto li = static_cast<int16_t>(l);
            auto ri = static_cast<int16_t>(r);
            auto lq = qfloat8_t(li, nullptr);
            auto rq = qfloat8_t(ri, nullptr);
            auto lf = float(lq);
            auto rf = float(rq);

            auto resultFloat = lf * rf;
            auto resultQ = lq * rq;
            if (lf < 0 && rf < 0 && float(resultQ) < 0) {
                continue;
            }

            if (lf > 0 && rf > 0 && float(resultQ) < 0) {
                continue;
            }

            if (lf < 0 && rf > 0 && float(resultQ) > 0) {
                continue;
            }

            if (lf > 0 && rf < 0 && float(resultQ) > 0) {
                continue;
            }

            // if (resultFloat <= 127 && resultFloat >= -128) {
                auto p = 100 * std::abs(double(resultQ) / double(resultFloat) - 1);
                totalP += p;
                count++;
                if (p > maxP) {
                    maxP = p;
                    std::cout << p << std::endl;
                }
                if (p >= 100) {
                    maxP = p;
                    std::printf("%04x %04x %04x\n", lq.getRaw(), rq.getRaw(), resultQ.getRaw());
                    std::cout << lf << " * " << rf << " = " << resultFloat << " ~ " << float(resultQ) << " | " << p << std::endl;
                }
            // }
        }
    }

    std::cout << "totalP=" << totalP << ", count=" << count << std::endl;
    std::cout << totalP / count << std::endl;
}