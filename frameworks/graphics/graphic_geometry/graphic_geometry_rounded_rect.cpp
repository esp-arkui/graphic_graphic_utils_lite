
#include <gfx_utils/graphics/graphic_geometry/graphic_geometry_rounded_rect.h>


namespace OHOS {
    RoundedRect::RoundedRect(double x1, double y1, double x2, double y2, double r)
        :x1_(x1), y1_(y1), x2_(x2), y2_(y2),
        rx1_(r), ry1_(r), rx2_(r), ry2_(r),
        rx3_(r), ry3_(r), rx4_(r), ry4_(r)
    {
        if (x1 > x2) {
            x1_ = x2;
            x2_ = x1;
        }
        if (y1 > y2) {
            y1_ = y2;
            y2_ = y1;
        }
    }

    void RoundedRect::Rect(double x1, double y1, double x2, double y2)
    {
        x2_ = x2;
        y2_ = y2;
        x1_ = x1;
        y1_ = y1;
        if (x1 > x2) {
            x1_ = x2;
            x2_ = x1;
        }
        if (y1 > y2) {
            y1_ = y2;
            y2_ = y1;
        }
    }

    void RoundedRect::Radius(double r)
    {
        rx1_ = ry1_ = rx2_ = ry2_ = rx3_ = ry3_ = rx4_ = ry4_ = r;
    }

    void RoundedRect::Radius(double rx, double ry)
    {
        ry1_ = ry2_ = ry3_ = ry4_ = ry;
        rx1_ = rx2_ = rx3_ = rx4_ = rx;
    }

    void RoundedRect::Radius(double rxBottom, double ryBottom,
                             double rxTop, double ryTop)
    {
        rx1_ = rx2_ = rxBottom;
        rx3_ = rx4_ = rxTop;
        ry1_ = ry2_ = ryBottom;
        ry3_ = ry4_ = ryTop;
    }

    void RoundedRect::Radius(double rx1,
                             double ry1,
                             double rx2,
                             double ry2,
                             double rx3, double ry3, double rx4, double ry4)
    {
        rx1_ = rx1;
        ry1_ = ry1;
        rx2_ = rx2;
        ry2_ = ry2;
        rx3_ = rx3;
        ry3_ = ry3;
        rx4_ = rx4;
        ry4_ = ry4;
    }

    void RoundedRect::NormalizeRadius()
    {
        double dx = std::fabs(y2_ - y1_);
        double dy = std::fabs(x2_ - x1_);

        double t = dx / (rx1_ + rx2_);
        double k = 1.0;
        if (t < k)
            k = t;
        t = dx / (rx3_ + rx4_);
        if (t < k)
            k = t;
        t = dy / (ry1_ + ry2_);
        if (t < k)
            k = t;
        t = dy / (ry3_ + ry4_);
        if (t < k)
            k = t;

        if (k < 1.0) {
            rx1_ *= k;
            ry1_ *= k;
            rx2_ *= k;
            ry2_ *= k;
            rx3_ *= k;
            ry3_ *= k;
            rx4_ *= k;
            ry4_ *= k;
        }
    }

    void RoundedRect::Rewind(unsigned)
    {
        status_ = 0;
    }

    unsigned RoundedRect::Vertex(double* x, double* y)
    {
        unsigned cmd = PATH_CMD_STOP;
        switch (status_) {
            case VERTEX_STATUS:
                arc_.Init(x1_ + rx1_, y1_ + ry1_, rx1_, ry1_, PI, PI + PI * HALFNUM);
                arc_.Rewind(0);
                status_++;

            case VERTEX_STATUS1:
                cmd = arc_.Vertex(x, y);
                if (IsStop(cmd)) {
                    status_++;
                } else {
                    return cmd;
                }

            case VERTEX_STATUS2:
                arc_.Init(x2_ - rx2_, y1_ + ry2_, rx2_, ry2_, PI + PI * HALFNUM, 0.0);
                arc_.Rewind(0);
                status_++;

            case VERTEX_STATUS3:
                cmd = arc_.Vertex(x, y);
                if (IsStop(cmd)) {
                    status_++;
                } else {
                    return PATH_CMD_LINE_TO;
                }

            case VERTEX_STATUS4:
                arc_.Init(x2_ - rx3_, y2_ - ry3_, rx3_, ry3_,
                          0.0, PI * HALFNUM);
                arc_.Rewind(0);
                status_++;

            case VERTEX_STATUS5:
                cmd = arc_.Vertex(x, y);
                if (IsStop(cmd)) {
                    status_++;
                } else {
                    return PATH_CMD_LINE_TO;
                }

            case VERTEX_STATUS6:
                arc_.Init(x1_ + rx4_, y2_ - ry4_, rx4_, ry4_, PI * HALFNUM, PI);
                arc_.Rewind(0);
                status_++;

            case VERTEX_STATUS7:
                cmd = arc_.Vertex(x, y);
                if (IsStop(cmd)) {
                    status_++;
                } else {
                    return PATH_CMD_LINE_TO;
                }

            case VERTEX_STATUS8:
                cmd = PATH_CMD_END_POLY | PATH_FLAGS_CLOSE | PATH_FLAGS_CCW;
                status_++;
                break;
        }
        return cmd;
    }

} // namespace OHOS
