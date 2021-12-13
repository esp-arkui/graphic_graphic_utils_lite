/*
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd.
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

#ifndef GRAPHIC_PATH_STORAGE_INCLUDED
#define GRAPHIC_PATH_STORAGE_INCLUDED

#include <cstring>

#include "graphic_geometry_bezier_arc.h"
#include "gfx_utils/graphics/graphic_common/graphic_common_math.h"
#include "gfx_utils/graphics/graphic_geometry/graphic_geometry_array.h"

namespace OHOS {

    //----------------------------------------------------VertexBlockStorage
    template <class ValueType, unsigned BlockShift = 8, unsigned BlockPool = 256>
    class VertexBlockStorage {
    public:
        // Allocation parameters
        enum BlockScale {
            BLOCK_SHIFT = BlockShift,
            BLOCK_SIZE = 1 << BLOCK_SHIFT,
            BLOCK_MASK = BLOCK_SIZE - 1,
            BLOCK_POOL = BlockPool
        };
        using SelfType = VertexBlockStorage<ValueType, BlockShift, BlockPool>;

        ~VertexBlockStorage();
        VertexBlockStorage();
        VertexBlockStorage(const SelfType& v);
        const SelfType& operator=(const SelfType& ps);
        /**
         * @brief 去除所有顶点
         *
         * @since 1.0
         * @version 1.0
         */
        void RemoveAll();
        /**
         * @brief 释放空间
         * 
         * @since 1.0
         * @version 1.0
         */
        void FreeAll();
        /**
         * @brief 添加顶点
         *
         * @param x 顶点x轴坐标
         * @param y 顶点y轴坐标
         * @param cmd 指令类型
         * @since 1.0
         * @version 1.0
         */
        void AddVertex(double x, double y, unsigned cmd);
        /**
         * @brief 返回最后一条指令
         * 
         * @return 返回最后一条指令类型
         * @since 1.0
         * @version 1.0
         */
        unsigned LastCommand() const;
        /**
         * @brief 返回最后一个顶点的坐标
         * 
         * @param x 用于获取最后一个顶点x轴坐标
         * @param y 用于获取最后一个顶点y轴坐标
         * @return 返回该顶点对应的指令类型
         * @since 1.0
         * @version 1.0
         */
        unsigned LastVertex(double* x, double* y) const;
        /**
         * @brief 返回顶点数量
         * 
         * @return 返回顶点数量
         * @since 1.0
         * @version 1.0
         */
        unsigned TotalVertices() const;
        /**
         * @brief 获取特定的顶点的坐标
         * 
         * @param idx 顶点下标
         * @param x 用于获取顶点x轴坐标
         * @param y 用于获取顶点y轴坐标
         * @return 返回该顶点对应的指令类型
         * @since 1.0
         * @version 1.0
         */
        unsigned Vertex(unsigned idx, double* x, double* y) const;
        /**
         * @brief 获取特定的顶点对应的指令类型
         * 
         * @param idx 顶点下标
         * @return 返回该顶点对应的指令类型
         * @since 1.0
         * @version 1.0
         */
        unsigned Command(unsigned idx) const;

    private:
        void AllocateBlock(unsigned nb);
        int8u* StoragePtrs(ValueType** xy_ptr);

    private:
        unsigned totalVertices_;
        unsigned totalBlocks_;
        unsigned maxBlocks_;
        ValueType** croodBlocks_;
        int8u** cmdBlocks_;
    };

    //------------------------------------------------------------------------
    template <class ValueType, unsigned S, unsigned P>
    void VertexBlockStorage<ValueType, S, P>::FreeAll()
    {
        if (totalBlocks_) {
            ValueType** coord_blk = croodBlocks_ + totalBlocks_ - 1;
            for (; totalBlocks_ > 0; totalBlocks_--) {
                ArrAllocator<ValueType>::Deallocate(
                    *coord_blk,
                    BLOCK_SIZE * 2 +
                        BLOCK_SIZE / (sizeof(ValueType) / sizeof(unsigned char)));
                --coord_blk;
            }
            ArrAllocator<ValueType*>::Deallocate(croodBlocks_, maxBlocks_ * 2);
            totalBlocks_ = 0;
            maxBlocks_ = 0;
            croodBlocks_ = 0;
            cmdBlocks_ = 0;
            totalVertices_ = 0;
        }
    }

    //------------------------------------------------------------------------
    template <class ValueType, unsigned S, unsigned P>
    VertexBlockStorage<ValueType, S, P>::~VertexBlockStorage()
    {
        FreeAll();
    }

    //------------------------------------------------------------------------
    template <class ValueType, unsigned S, unsigned P>
    VertexBlockStorage<ValueType, S, P>::VertexBlockStorage() :
        totalVertices_(0),
        totalBlocks_(0),
        maxBlocks_(0),
        croodBlocks_(0),
        cmdBlocks_(0)
    {
    }

    //------------------------------------------------------------------------
    template <class ValueType, unsigned S, unsigned P>
    VertexBlockStorage<ValueType, S, P>::VertexBlockStorage(const VertexBlockStorage<ValueType, S, P>& v) :
        totalVertices_(0),
        totalBlocks_(0),
        maxBlocks_(0),
        croodBlocks_(0),
        cmdBlocks_(0)
    {
        *this = v;
    }

    //------------------------------------------------------------------------
    template <class ValueType, unsigned S, unsigned P>
    const VertexBlockStorage<ValueType, S, P>&
        VertexBlockStorage<ValueType, S, P>::operator=(const VertexBlockStorage<ValueType, S, P>& v)
    {
        RemoveAll();
        unsigned i;
        for (i = 0; i < v.TotalVertices(); i++) {
            double x, y;
            unsigned cmd = v.Vertex(i, &x, &y);
            AddVertex(x, y, cmd);
        }
        return *this;
    }

    //------------------------------------------------------------------------
    template <class ValueType, unsigned S, unsigned P>
    inline void VertexBlockStorage<ValueType, S, P>::RemoveAll()
    {
        totalVertices_ = 0;
    }

    //------------------------------------------------------------------------
    template <class ValueType, unsigned S, unsigned P>
    inline void VertexBlockStorage<ValueType, S, P>::AddVertex(double x, double y,
                                                               unsigned cmd)
    {
        ValueType* coord_ptr = 0;
        *StoragePtrs(&coord_ptr) = (int8u)cmd;
        coord_ptr[0] = ValueType(x);
        coord_ptr[1] = ValueType(y);
        totalVertices_++;
    }

    //------------------------------------------------------------------------
    template <class ValueType, unsigned S, unsigned P>
    inline unsigned VertexBlockStorage<ValueType, S, P>::LastCommand() const
    {
        if (totalVertices_) {
            return Command(totalVertices_ - 1);
        }
        return PATH_CMD_STOP;
    }

    //------------------------------------------------------------------------
    template <class ValueType, unsigned S, unsigned P>
    inline unsigned VertexBlockStorage<ValueType, S, P>::LastVertex(double* x, double* y) const
    {
        if (totalVertices_) {
            return Vertex(totalVertices_ - 1, x, y);
        }
        return PATH_CMD_STOP;
    }

    // //------------------------------------------------------------------------
    template <class ValueType, unsigned S, unsigned P>
    inline unsigned VertexBlockStorage<ValueType, S, P>::TotalVertices() const
    {
        return totalVertices_;
    }

    //------------------------------------------------------------------------
    template <class ValueType, unsigned S, unsigned P>
    inline unsigned VertexBlockStorage<ValueType, S, P>::Vertex(unsigned idx,
                                                                double* x, double* y) const
    {
        unsigned nb = idx >> BLOCK_SHIFT;
        const ValueType* pv = croodBlocks_[nb] + ((idx & BLOCK_MASK) << 1);
        *x = pv[0];
        *y = pv[1];
        return cmdBlocks_[nb][idx & BLOCK_MASK];
    }

    //------------------------------------------------------------------------
    template <class ValueType, unsigned S, unsigned P>
    inline unsigned VertexBlockStorage<ValueType, S, P>::Command(unsigned idx) const
    {
        return cmdBlocks_[idx >> BLOCK_SHIFT][idx & BLOCK_MASK];
    }

    //------------------------------------------------------------------------
    template <class ValueType, unsigned S, unsigned P>
    void VertexBlockStorage<ValueType, S, P>::AllocateBlock(unsigned nb)
    {
        if (nb >= maxBlocks_) {
            ValueType** new_coords =
                ArrAllocator<ValueType*>::Allocate((maxBlocks_ + BLOCK_POOL) * 2);

            unsigned char** new_cmds =
                (unsigned char**)(new_coords + maxBlocks_ + BLOCK_POOL);

            if (croodBlocks_) {
                std::memcpy(new_coords,
                            croodBlocks_,
                            maxBlocks_ * sizeof(ValueType*));

                std::memcpy(new_cmds,
                            cmdBlocks_,
                            maxBlocks_ * sizeof(unsigned char*));

                ArrAllocator<ValueType*>::Deallocate(croodBlocks_, maxBlocks_ * 2);
            }
            croodBlocks_ = new_coords;
            cmdBlocks_ = new_cmds;
            maxBlocks_ += BLOCK_POOL;
        }
        croodBlocks_[nb] =
            ArrAllocator<ValueType>::Allocate(BLOCK_SIZE * 2 +
                                              BLOCK_SIZE / (sizeof(ValueType) / sizeof(unsigned char)));

        cmdBlocks_[nb] =
            (unsigned char*)(croodBlocks_[nb] + BLOCK_SIZE * 2);

        totalBlocks_++;
    }

    //------------------------------------------------------------------------
    template <class ValueType, unsigned S, unsigned P>
    int8u* VertexBlockStorage<ValueType, S, P>::StoragePtrs(ValueType** xy_ptr)
    {
        unsigned nb = totalVertices_ >> BLOCK_SHIFT;
        if (nb >= totalBlocks_) {
            AllocateBlock(nb);
        }
        *xy_ptr = croodBlocks_[nb] + ((totalVertices_ & BLOCK_MASK) << 1);
        return cmdBlocks_[nb] + (totalVertices_ & BLOCK_MASK);
    }

    //-----------------------------------------------------PolyPlainAdaptor
    template <class ValueType>
    class PolyPlainAdaptor {
    public:
        PolyPlainAdaptor() :
            data_(0),
            ptr_(0),
            end_(0),
            closed_(false),
            stop_(false)
        {}

        PolyPlainAdaptor(const ValueType* data, unsigned numPoints, bool closed) :
            data_(data),
            ptr_(data),
            end_(data + numPoints * 2),
            closed_(closed),
            stop_(false)
        {}

        void Init(const ValueType* data, unsigned numPoints, bool closed)
        {
            data_ = data;
            ptr_ = data;
            end_ = data + numPoints * 2;
            closed_ = closed;
            stop_ = false;
        }

        void Rewind(unsigned)
        {
            ptr_ = data_;
            stop_ = false;
        }

        unsigned Vertex(double* x, double* y)
        {
            if (ptr_ < end_) {
                bool first = ptr_ == data_;
                *x = *ptr_++;
                *y = *ptr_++;
                if (first) {
                    return PATH_CMD_MOVE_TO;
                } else {
                    return PATH_CMD_LINE_TO;
                }
            }
            *x = *y = 0.0;
            if (closed_ && !stop_) {
                stop_ = true;
                return PATH_CMD_END_POLY | PATH_FLAGS_CLOSE;
            }
            return PATH_CMD_STOP;
        }

    private:
        const ValueType* data_;
        const ValueType* ptr_;
        const ValueType* end_;
        bool closed_;
        bool stop_;
    };

    //---------------------------------------------------------------PathBase
    // A container to store vertices with their flags.
    // A path consists of a number of contours separated with "MoveTo"
    // commands. The path storage can keep and maintain more than one
    // path.
    // To navigate to the beginning of a particular path, use Rewind(pathId);
    // Where pathId is what StartNewPath() returns. So, when you call
    // StartNewPath() you need to store its return value somewhere else
    // to navigate to the path afterwards.
    //
    // See also: vertex_source concept
    //------------------------------------------------------------------------
    template <class VertexContainer>
    class PathBase {
    public:
        using SelfType = PathBase<VertexContainer>;

        //--------------------------------------------------------------------
        PathBase() :
            vertices_(), iterator_(0)
        {}
        /**
         * @brief 去除所有顶点
         *
         * @since 1.0
         * @version 1.0
         */
        void RemoveAll()
        {
            vertices_.RemoveAll();
            iterator_ = 0;
        }
        /**
         * @brief 释放空间
         * 
         * @since 1.0
         * @version 1.0
         */
        void FreeAll()
        {
            vertices_.FreeAll();
            iterator_ = 0;
        }

        /**
         * @brief 开始新的路径
         * 
         * @return 返回顶点数量
         * @since 1.0
         * @version 1.0
         */
        unsigned StartNewPath();
        /**
         * @brief 移动一个点到设定的坐标
         * 
         * @param x 顶点x轴坐标
         * @param y 顶点y轴坐标
         * @since 1.0
         * @version 1.0
         */
        void MoveTo(double x, double y);

        /**
         * @brief 连线连到输入的坐标
         * 
         * @param x 顶点x轴坐标
         * @param y 顶点y轴坐标
         * @since 1.0
         * @version 1.0
         */
        void LineTo(double x, double y);

        /**
         * @brief 画弧线
         * 
         * @param rx 长短半轴
         * @param ry 长短半轴
         * @param angle 角度
         * @param largeArcFlag 优劣弧
         * @param sweepFlag 顺逆时针
         * @param x 终点x轴坐标
         * @param y 终点y轴坐标
         * @since 1.0
         * @version 1.0
         */
        void ArcTo(double rx, double ry,
                   double angle,
                   bool largeArcFlag,
                   bool sweepFlag,
                   double x, double y);

        void EndPoly(unsigned flags = PATH_FLAGS_CLOSE);
        void ClosePolygon(unsigned flags = PATH_FLAGS_NONE);

        /**
         * @brief 返回顶点数量
         * 
         * @return 返回顶点数量
         * @since 1.0
         * @version 1.0
         */
        unsigned TotalVertices() const;

        /**
         * @brief 返回最后一个顶点的坐标
         * 
         * @param x 用于获取最后一个顶点x轴坐标
         * @param y 用于获取最后一个顶点y轴坐标
         * @return 返回该顶点对应的指令类型
         * @since 1.0
         * @version 1.0
         */
        unsigned LastVertex(double* x, double* y) const;
        /**
         * @brief 获取特定的顶点的坐标
         * 
         * @param idx 顶点下标
         * @param x 用于获取顶点x轴坐标
         * @param y 用于获取顶点y轴坐标
         * @return 返回该顶点对应的指令类型
         * @since 1.0
         * @version 1.0
         */
        unsigned Vertex(unsigned idx, double* x, double* y) const;

        /**
         * @brief 迭代器回退到某一个顶点
         * 
         * @param pathId 回退的顶点序号
         * @since 1.0
         * @version 1.0
         */
        void Rewind(unsigned pathId);

        /**
         * @brief 获取下一个顶点的坐标
         * 
         * @param x 用于获取顶点x轴坐标
         * @param y 用于获取顶点y轴坐标
         * @return 返回下一个顶点对应的指令类型
         * @since 1.0
         * @version 1.0
         */
        unsigned Vertex(double* x, double* y);

        /**
         * @brief 连接路径
         * 
         * @param vs 要连接的顶点源
         * @param pathId 连接位置
         * @since 1.0
         * @version 1.0
         */
        template <class VertexSource>
        void ConcatPath(VertexSource& vs, unsigned pathId = 0)
        {
            double x, y;
            unsigned cmd;
            vs.Rewind(pathId);
            while (!IsStop(cmd = vs.Vertex(&x, &y))) {
                vertices_.AddVertex(x, y, cmd);
            }
        }

        /**
         * @brief 将顶点源加入到已有的源
         * 
         * @param vs 要加入的顶点源
         * @param pathId 加入的位置
         * @since 1.0
         * @version 1.0
         */
        template <class VertexSource>
        void JoinPath(VertexSource& vs, unsigned pathId = 0)
        {
            double x, y;
            unsigned cmd;
            vs.Rewind(pathId);
            cmd = vs.Vertex(&x, &y);
            if (!IsStop(cmd)) {
                if (IsVertex(cmd)) {
                    double x0, y0;
                    unsigned cmd0 = LastVertex(&x0, &y0);
                    if (IsVertex(cmd0)) {
                        if (CalcDistance(x, y, x0, y0) > VERTEX_DIST_EPSILON) {
                            if (IsMoveTo(cmd)) {
                                cmd = PATH_CMD_LINE_TO;
                            }
                            vertices_.AddVertex(x, y, cmd);
                        }
                    } else {
                        if (IsStop(cmd0)) {
                            cmd = PATH_CMD_MOVE_TO;
                        } else {
                            if (IsMoveTo(cmd)) {
                                cmd = PATH_CMD_LINE_TO;
                            }
                        }
                        vertices_.AddVertex(x, y, cmd);
                    }
                }
                for (; !IsStop(cmd = vs.Vertex(&x, &y));) {
                    unsigned int c;
                    if (IsMoveTo(cmd)) {
                        c = unsigned(PATH_CMD_LINE_TO);
                    } else {
                        c = cmd;
                    }
                    vertices_.AddVertex(x, y, c);
                }
            }
        }

        /**
         * @brief 连接路径
         * 
         * @param data 要连接的顶点源
         * @param numPoints 顶点数
         * @param closed 是否闭合
         * @since 1.0
         * @version 1.0
         */
        template <class ValueType>
        void ConcatPoly(const ValueType* data,
                        unsigned numPoints,
                        bool closed)
        {
            PolyPlainAdaptor<ValueType> poly(data, numPoints, closed);
            ConcatPath(poly);
        }

        /**
         * @brief 将顶点源加入到已有的源
         * 
         * @param data 要加入的顶点源
         * @param pathId 加入的位置
         * @param numPoints 顶点数
         * @param closed 是否闭合
         * @since 1.0
         * @version 1.0
         */
        template <class ValueType>
        void JoinPoly(const ValueType* data,
                      unsigned numPoints,
                      bool closed)
        {
            PolyPlainAdaptor<ValueType> poly(data, numPoints, closed);
            JoinPath(poly);
        }

    private:
        VertexContainer vertices_;
        unsigned iterator_;
    };

    //------------------------------------------------------------------------
    template <class VC>
    unsigned PathBase<VC>::StartNewPath()
    {
        if (!is_stop(vertices_.LastCommand())) {
            vertices_.AddVertex(0.0, 0.0, PATH_CMD_STOP);
        }
        return vertices_.TotalVertices();
    }

    //------------------------------------------------------------------------
    template <class VC>
    inline void PathBase<VC>::MoveTo(double x, double y)
    {
        vertices_.AddVertex(x, y, PATH_CMD_MOVE_TO);
    }

    //------------------------------------------------------------------------
    template <class VC>
    inline void PathBase<VC>::LineTo(double x, double y)
    {
        vertices_.AddVertex(x, y, PATH_CMD_LINE_TO);
    }

    //------------------------------------------------------------------------
    template <class VC>
    void PathBase<VC>::ArcTo(double rx, double ry,
                             double angle,
                             bool largeArcFlag,
                             bool sweepFlag,
                             double x, double y)
    {
        if (vertices_.TotalVertices() && IsVertex(vertices_.LastCommand())) {
            const double epsilon = 1e-30;
            double x0 = 0.0;
            double y0 = 0.0;
            vertices_.LastVertex(&x0, &y0);

            rx = std::fabs(rx);
            ry = std::fabs(ry);

            // Ensure radii are valid
            //-------------------------
            if (rx < epsilon || ry < epsilon) {
                LineTo(x, y);
                return;
            }

            if (CalcDistance(x0, y0, x, y) < epsilon) {
                // If the endpoints (x, y) and (x0, y0) are identical, then this
                // is equivalent to omitting the elliptical arc segment entirely.
                return;
            }
            BezierArcSvg a(x0, y0, rx, ry, angle, largeArcFlag, sweepFlag, x, y);
            if (a.RadiiOK()) {
                JoinPath(a);
            } else {
                LineTo(x, y);
            }
        } else {
            MoveTo(x, y);
        }
    }

    //------------------------------------------------------------------------
    template <class VC>
    inline void PathBase<VC>::EndPoly(unsigned flags)
    {
        if (IsVertex(vertices_.LastCommand())) {
            vertices_.AddVertex(0.0, 0.0, PATH_CMD_END_POLY | flags);
        }
    }

    //------------------------------------------------------------------------
    template <class VC>
    inline void PathBase<VC>::ClosePolygon(unsigned flags)
    {
        EndPoly(PATH_FLAGS_CLOSE | flags);
    }

    //------------------------------------------------------------------------
    template <class VC>
    inline unsigned PathBase<VC>::TotalVertices() const
    {
        return vertices_.TotalVertices();
    }

    template <class VC>
    inline unsigned PathBase<VC>::LastVertex(double* x, double* y) const
    {
        return vertices_.LastVertex(x, y);
    }

    //------------------------------------------------------------------------
    template <class VC>
    inline void PathBase<VC>::Rewind(unsigned pathId)
    {
        iterator_ = pathId;
    }

    //------------------------------------------------------------------------
    template <class VC>
    inline unsigned PathBase<VC>::Vertex(double* x, double* y)
    {
        if (iterator_ >= vertices_.TotalVertices()) {
            return PATH_CMD_STOP;
        }
        return vertices_.Vertex(iterator_++, x, y);
    }

    //-----------------------------------------------------------PathStorage
    using PathStorage = PathBase<VertexBlockStorage<double> >;

} // namespace OHOS

#endif
