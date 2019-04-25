#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
extern "C" {
#include "math-funcs.h"
#include "pyin.h"
}

namespace py = pybind11;
using namespace pybind11::literals;

py::array_t<FP_TYPE> Analyze(const py::array_t<FP_TYPE> &signal, int fs,
                             int win_len, int hop_len)
{
    int nx = (int)signal.size();
    FP_TYPE *x = (FP_TYPE *)malloc(sizeof(FP_TYPE) * signal.size());
    memcpy(x, signal.data(), sizeof(FP_TYPE) * signal.size());
    // param
    pyin_config param = pyin_init(hop_len);
    param.trange = pyin_trange(param.nq, param.fmin, param.fmax);
    param.nf = win_len;
    param.w = param.nf / 4;

    int nfrm = 0;
    FP_TYPE* f0 = pyin_analyze(param, x, nx, fs, & nfrm);

    FP_TYPE min = param.fmax;
    FP_TYPE max = param.fmin;
    for(int i = 0; i < nfrm; i ++) {
        if(f0[i] > 0) {
            if(f0[i] < min) min = f0[i];
            if(f0[i] > max) max = f0[i];
        }
    }

    py::array_t<FP_TYPE> ret(nfrm);
    memcpy(ret.mutable_data(), f0, sizeof(FP_TYPE) * ret.size());

    free(x);
    free(f0);
    return ret;
}

PYBIND11_MODULE(pyin11, m) {
    m.doc() = "pyin pybind11 plugin.";
    m.def("analyze", &Analyze, "y"_a, "sr"_a, "win_len"_a, "hop_len"_a);
}
