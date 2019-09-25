/*
 * Copyright 2019 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "xf_pyr_down_config.h"

void pyr_down_accel(xf::cv::Mat<TYPE, HEIGHT, WIDTH, XF_NPPC1>& _src,
                    xf::cv::Mat<TYPE, HEIGHT, WIDTH, XF_NPPC1>& _dst) {
    xf::cv::pyrDown<TYPE, HEIGHT, WIDTH, XF_NPPC1, XF_USE_URAM>(_src, _dst);
}
