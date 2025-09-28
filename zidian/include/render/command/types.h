#pragma once

namespace zidian{
    const int CMD_TYPE_UNSET = -1;
    const int CMD_TYPE_DRAW_POINT = 1;
    const int CMD_TYPE_CLEAR = 2;
    const int CMD_TYPE_SET_CLEAR_COLOR = 3;

    const int CMD_TYPE_BATCH_DRAW_POINTS_BEGIN = 4;
    const int CMD_TYPE_BATCH_DRAW_POINTS_END = 5;
}