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
#include "gfx_utils/graphics/graphic_common/graphic_common_math.h"
#include "gfx_utils/graphics/graphic_geometry/graphic_geometry_array.h"
#include "graphic_geometry_bezier_arc.h"

namespace OHOS {
    /**
     * @brief 顶点源数据块
     * @since 1.0
     * @version 1.0
     */
    class VertexBlockStorage {
    public:
        enum BlockScale {
            BLOCK_SHIFT = OHOS::THIRTY_TWO_COLOR_NUM,
            BLOCK_SIZE = 1 << BLOCK_SHIFT,
            BLOCK_MASK = BLOCK_SIZE - 1,
            BLOCK_POOL = OHOS::MAX_COLOR_SIZE
        };

        ~VertexBlockStorage(){
            FreeAll();
        };

        VertexBlockStorage()
            :totalVertices_(0),
            totalBlocks_(0),
            maxBlocks_(0),
            croodBlocks_(0),
            cmdBlocks_(0)
        {};

        const VertexBlockStorage& operator=(const VertexBlockStorage& vertexBlockStorage)
        {
            RemoveAll();
            unsigned iIndex;
            for (iIndex = 0; iIndex < vertexBlockStorage.TotalVertices(); iIndex++) {
                double x, y;
                unsigned cmd = vertexBlockStorage.Vertex(iIndex, &x, &y);
                AddVertex(x, y, cmd);
            }
            return *this;
        }

        VertexBlockStorage(const VertexBlockStorage& vertexBlockStorage)
        {
            *this = vertexBlockStorage;
        }

        /**
         * @brief 去除所有顶点
         *
         * @since 1.0
         * @version 1.0
         */
        void RemoveAll()
        {
            totalVertices_ = 0;
        }
        /**
         * @brief 释放空间
         * @since 1.0
         * @version 1.0
         */
        void FreeAll()
        {
            if (totalBlocks_) {
                double** coordBLK = croodBlocks_ + totalBlocks_ - 1;
                for (; totalBlocks_ > 0; totalBlocks_--) {
                    GeometryArrayAllocator<double>::Deallocate(
                        *coordBLK,
                        BLOCK_SIZE * OHOS::TWO_TIMES +
                            BLOCK_SIZE / (sizeof(double) / sizeof(unsigned char)));
                    --coordBLK;
                }
                GeometryArrayAllocator<double*>::Deallocate(croodBlocks_, maxBlocks_ * OHOS::TWO_TIMES);
                totalBlocks_ = 0;
                maxBlocks_ = 0;
                croodBlocks_ = 0;
                cmdBlocks_ = 0;
                totalVertices_ = 0;
            }
        }
        /**
         * @brief 添加顶点
         *
         * @param x 顶点x轴坐标
         * @param y 顶点y轴坐标
         * @param cmd 指令类型
         * @since 1.0
         * @version 1.0
         */
        void AddVertex(double x, double y, unsigned cmd)
        {
            double* coord_ptr = 0;
            *StoragePtrs(&coord_ptr) = (int8u)cmd;
            coord_ptr[0] = double(x);
            coord_ptr[1] = double(y);
            totalVertices_++;
        }
        /**
         * @brief 返回最后一条指令
         * @return 返回最后一条指令类型
         * @since 1.0
         * @version 1.0
         */
        unsigned LastCommand() const
        {
            if (totalVertices_) {
                return Command(totalVertices_ - 1);
            }
            return PATH_CMD_STOP;
        }
        /**
         * @brief 返回最后一个顶点的坐标
         * @param x 用于获取最后一个顶点x轴坐标
         * @param y 用于获取最后一个顶点y轴坐标
         * @return 返回该顶点对应的指令类型
         * @since 1.0
         * @version 1.0
         */
        unsigned LastVertex(double* x, double* y) const
        {
            if (totalVertices_) {
                return Vertex(totalVertices_ - 1, x, y);
            }
            return PATH_CMD_STOP;
        }
        /**
         * @brief 返回顶点数量
         * @return 返回顶点数量
         * @since 1.0
         * @version 1.0
         */
        unsigned TotalVertices() const
        {
            return totalVertices_;
        }
        /**
         * @brief 获取特定的顶点的坐标
         * @param idx 顶点下标
         * @param x 用于获取顶点x轴坐标
         * @param y 用于获取顶点y轴坐标
         * @return 返回该顶点对应的指令类型
         * @since 1.0
         * @version 1.0
         */
        unsigned Vertex(unsigned index, double* x, double* y) const
        {
            unsigned nb = index >> BLOCK_SHIFT;
            const double* pv = croodBlocks_[nb] + ((index & BLOCK_MASK) << 1);
            *x = pv[0];
            *y = pv[1];
            return cmdBlocks_[nb][index & BLOCK_MASK];
        }
        /**
         * @brief 获取特定的顶点对应的指令类型
         * @param index 顶点下标
         * @return 返回该顶点对应的指令类型
         * @since 1.0
         * @version 1.0
         */
        unsigned Command(unsigned index) const
        {
            return cmdBlocks_[index >> BLOCK_SHIFT][index & BLOCK_MASK];
        }
    private:
        void AllocateBlock(unsigned nb)
        {
            if (nb >= maxBlocks_) {
                double** new_coords = GeometryArrayAllocator<double*>::Allocate(
                    (maxBlocks_ + BLOCK_POOL) * OHOS::TWO_TIMES);

                unsigned char** new_cmds =
                    (unsigned char**)(new_coords + maxBlocks_ + BLOCK_POOL);

                if (croodBlocks_) {
                    memcpy_s(new_coords, maxBlocks_ * sizeof(double*),
                             croodBlocks_,
                             maxBlocks_ * sizeof(double*));

                    memcpy_s(new_cmds, maxBlocks_ * sizeof(double*),
                             cmdBlocks_,
                             maxBlocks_ * sizeof(unsigned char*));

                    GeometryArrayAllocator<double*>::Deallocate(croodBlocks_,
                                                                   maxBlocks_ * OHOS::TWO_TIMES);
                }
                croodBlocks_ = new_coords;
                cmdBlocks_ = new_cmds;
                maxBlocks_ += BLOCK_POOL;
            }
            croodBlocks_[nb] =
                GeometryArrayAllocator<double>::Allocate(BLOCK_SIZE * OHOS::TWO_TIMES +
                                                            BLOCK_SIZE / (sizeof(double) / sizeof(unsigned char)));

            cmdBlocks_[nb] =
                (unsigned char*)(croodBlocks_[nb] + BLOCK_SIZE * OHOS::TWO_TIMES);

            totalBlocks_++;
        }
        int8u* StoragePtrs(double** xy_ptr)
        {
            unsigned nb = totalVertices_ >> BLOCK_SHIFT;
            if (nb >= totalBlocks_) {
                AllocateBlock(nb);
            }
            *xy_ptr = croodBlocks_[nb] + ((totalVertices_ & BLOCK_MASK) << 1);
            return cmdBlocks_[nb] + (totalVertices_ & BLOCK_MASK);
        }
    private:
        unsigned totalVertices_;
        unsigned totalBlocks_;
        unsigned maxBlocks_;
        double** croodBlocks_;//输入的点
        int8u** cmdBlocks_;//标记点状态
    };

    class UICanvasVertices : public HeapBase{
    public:
        UICanvasVertices() :
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
         * @since 1.0
         * @version 1.0
         */
        void FreeAll()
        {
            vertices_.FreeAll();
            iterator_ = 0;
        }

        /**
         * @brief 移动一个点到设定的坐标
         * @param x 顶点x轴坐标
         * @param y 顶点y轴坐标
         * @since 1.0
         * @version 1.0
         */
        void MoveTo(double x, double y)
        {
            vertices_.AddVertex(x, y, PATH_CMD_MOVE_TO);
        }
        /**
         * @brief 连线连到输入的坐标
         * @param x 顶点x轴坐标
         * @param y 顶点y轴坐标
         * @since 1.0
         * @version 1.0
         */
        void LineTo(double x, double y)
        {
            vertices_.AddVertex(x, y, PATH_CMD_LINE_TO);
        }

        /**
         * @brief 画弧线
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
                   double x, double y)
        {
            if (vertices_.TotalVertices() && IsVertex(vertices_.LastCommand())) {
                const double epsilon = 1e-30;
                double x0 = 0.0;
                double y0 = 0.0;
                vertices_.LastVertex(&x0, &y0);

                rx = std::fabs(rx);
                ry = std::fabs(ry);
                if (rx < epsilon || ry < epsilon) {
                    LineTo(x, y);
                    return;
                }
                if (CalcDistance(x0, y0, x, y) < epsilon) {
                    return;
                }
                BezierArcSvg bezierArcSvg(x0, y0, rx, ry, angle, largeArcFlag, sweepFlag, x, y);
                if (bezierArcSvg.RadiiOK()) {
                    JoinPath(bezierArcSvg);
                } else {
                    LineTo(x, y);
                }
            } else {
                MoveTo(x, y);
            }
        }

        void EndPoly(unsigned flags = PATH_FLAGS_CLOSE)
        {
            if (IsVertex(vertices_.LastCommand())) {
                vertices_.AddVertex(0.0, 0.0, PATH_CMD_END_POLY | flags);
            }
        }
        /**
         * @brief ClosePolygon 闭合多边形路径
         * @param flags
         */
        void ClosePolygon(unsigned flags = PATH_FLAGS_NONE)
        {
            EndPoly(PATH_FLAGS_CLOSE | flags);
        }

        /**
         * @brief 返回顶点数量
         * @return 返回顶点数量
         * @since 1.0
         * @version 1.0
         */
        unsigned TotalVertices() const
        {
            return vertices_.TotalVertices();
        }

        /**
         * @brief 返回最后一个顶点的坐标
         * @param x 用于获取最后一个顶点x轴坐标
         * @param y 用于获取最后一个顶点y轴坐标
         * @return 返回该顶点对应的指令类型
         * @since 1.0
         * @version 1.0
         */
        unsigned LastVertex(double* x, double* y) const
        {
            return vertices_.LastVertex(x, y);
        }
        /**
         * @brief 获取特定的顶点的坐标
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
         * @param pathId 回退的顶点序号
         * @since 1.0
         * @version 1.0
         */
        void Rewind(unsigned pathId)
        {
            iterator_ = pathId;
        }

        /**
         * @brief 获取下一个顶点的坐标
         * @param x 用于获取顶点x轴坐标
         * @param y 用于获取顶点y轴坐标
         * @return 返回下一个顶点对应的指令类型
         * @since 1.0
         * @version 1.0
         */
        unsigned Vertex(double* x, double* y)
        {
            if (iterator_ >= vertices_.TotalVertices()) {
                return PATH_CMD_STOP;
            }
            return vertices_.Vertex(iterator_++, x, y);
        }

        /**
         * @brief 连接路径
         * @param vs 要连接的顶点源
         * @param pathId 连接位置
         * @since 1.0
         * @version 1.0
         */
        void ConcatPath(BezierArc& path, unsigned pathId = 0)
        {
            double x, y;
            unsigned cmd;
            path.Rewind(pathId);
            while (!IsStop(cmd = path.Vertex(&x, &y))) {
                vertices_.AddVertex(x, y, cmd);
            }
        }

        /**
         * @brief 将顶点源加入到已有的源
         * @param vs 要加入的顶点源
         * @param pathId 加入的位置
         * @since 1.0
         * @version 1.0
         */
        void JoinPath(BezierArcSvg& path, unsigned pathId = 0)
        {
            double x, y;
            unsigned pathCommand;
            path.Rewind(pathId);
            pathCommand = path.Vertex(&x, &y);
            if (!IsStop(pathCommand)) {
                if (IsVertex(pathCommand)) {
                    double x0, y0;
                    unsigned cmd0 = LastVertex(&x0, &y0);
                    if (IsVertex(cmd0) && (CalcDistance(x, y, x0, y0) > VERTEX_DIST_EPSILON)) {
                        if (IsMoveTo(pathCommand)) {
                            pathCommand = PATH_CMD_LINE_TO;
                        }
                        vertices_.AddVertex(x, y, pathCommand);
                    } else if (IsStop(cmd0)) {
                        pathCommand = PATH_CMD_MOVE_TO;
                        vertices_.AddVertex(x, y, pathCommand);
                    } else {
                        if (IsMoveTo(pathCommand)) {
                            pathCommand = PATH_CMD_LINE_TO;
                        }
                        vertices_.AddVertex(x, y, pathCommand);
                    }
                }
                for (; !IsStop(pathCommand = path.Vertex(&x, &y));) {
                    unsigned int pathCmd;
                    if (IsMoveTo(pathCommand)) {
                        pathCmd = unsigned(PATH_CMD_LINE_TO);
                    } else {
                        pathCmd = pathCommand;
                    }
                    vertices_.AddVertex(x, y, pathCmd);
                }
            }
        }

    private:
        VertexBlockStorage vertices_;
        unsigned iterator_;
    };
} // namespace OHOS
#endif
