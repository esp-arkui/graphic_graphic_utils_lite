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

#include "gfx_utils/graphics/common/graphic_common_math.h"
#include "gfx_utils/graphics/vertexprimitive/graphic_geometry_array.h"
#include "graphic_geometry_bezier_arc.h"

namespace OHOS {
    /**
     * @brief Vertex source data block
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

        ~VertexBlockStorage()
        {
            FreeAll();
        };

        VertexBlockStorage()
            : totalVertices_(0),
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
                float x, y;
                unsigned cmd = vertexBlockStorage.Vertex(iIndex, &x, &y);
                AddVertex(x, y, cmd);
            }
            this->totalBlocks_= vertexBlockStorage.totalBlocks_;
            maxBlocks_= vertexBlockStorage.maxBlocks_;
            croodBlocks_ = vertexBlockStorage.croodBlocks_;
            cmdBlocks_= vertexBlockStorage.cmdBlocks_;
            return *this;
        }

        VertexBlockStorage(const VertexBlockStorage& vertexBlockStorage)
        {
            *this = vertexBlockStorage;
        }

        /**
         * @brief Remove all vertices.
         *
         * @since 1.0
         * @version 1.0
         */
        void RemoveAll()
        {
            totalVertices_ = 0;
        }
        /**
         * @brief Free up space
         * @since 1.0
         * @version 1.0
         */
        void FreeAll()
        {
            const int32_t totalBlocksMax = 1024 ;
            if (totalBlocks_ > 0 && totalBlocks_<= totalBlocksMax) {
                float** coordBLK = croodBlocks_ + totalBlocks_ - 1;
                for (; totalBlocks_ > 0; totalBlocks_--) {
                    GeometryArrayAllocator<float>::Deallocate(
                        *coordBLK,
                        BLOCK_SIZE * OHOS::TWO_TIMES +
                            BLOCK_SIZE / (sizeof(float) / sizeof(unsigned char)));
                    --coordBLK;
                }
                GeometryArrayAllocator<float*>::Deallocate(croodBlocks_, maxBlocks_ * OHOS::TWO_TIMES);
                totalBlocks_ = 0;
                maxBlocks_ = 0;
                croodBlocks_ = 0;
                cmdBlocks_ = 0;
                totalVertices_ = 0;
            }
        }
        /**
         * @brief add vertex.
         *
         * @param x Vertex X-axis coordinates.
         * @param y Vertex Y-axis coordinates.
         * @param cmd Instruction type.
         * @since 1.0
         * @version 1.0
         */
        void AddVertex(float x, float y, unsigned cmd)
        {
            float* coord_ptr = 0;
            *StoragePtrs(&coord_ptr) = (uint8_t)cmd;
            coord_ptr[0] = float(x);
            coord_ptr[1] = float(y);
            totalVertices_++;
        }
        /**
         * @brief Returns the last instruction.
         * @return Returns the last instruction type.
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
         * @brief Returns the coordinates of the last vertex.
         * @param x Used to obtain the x-axis coordinates of the last vertex.
         * @param y Used to obtain the y-axis coordinates of the last vertex.
         * @return Returns the instruction type corresponding to the vertex.
         * @since 1.0
         * @version 1.0
         */
        unsigned LastVertex(float* x, float* y) const
        {
            if (totalVertices_) {
                return Vertex(totalVertices_ - 1, x, y);
            }
            return PATH_CMD_STOP;
        }
        /**
         * @brief Returns the number of vertices.
         * @return Returns the number of vertices.
         * @since 1.0
         * @version 1.0
         */
        unsigned TotalVertices() const
        {
            return totalVertices_;
        }
        /**
         * @brief Gets the coordinates of a particular vertex.
         * @param idx Vertex subscript.
         * @param x Used to obtain the x-axis coordinates of vertices.
         * @param y Used to obtain the y-axis coordinates of vertices.
         * @return Returns the instruction type corresponding to the vertex.
         * @since 1.0
         * @version 1.0
         */
        unsigned Vertex(unsigned idx, float* x, float* y) const
        {
            unsigned nb = idx >> BLOCK_SHIFT;
            const float* pv = croodBlocks_[nb] + ((idx & BLOCK_MASK) << 1);
            *x = pv[0];
            *y = pv[1];
            return cmdBlocks_[nb][idx & BLOCK_MASK];
        }
        /**
         * @brief ets the instruction type corresponding to a specific vertex.
         * @param index Vertex subscript.
         * @return Returns the instruction type corresponding to the vertex.
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
                float** new_coords = GeometryArrayAllocator<float*>::Allocate(
                    (maxBlocks_ + BLOCK_POOL) * OHOS::TWO_TIMES);

                unsigned char** new_cmds =
                    (unsigned char**)(new_coords + maxBlocks_ + BLOCK_POOL);

                if (croodBlocks_) {
                    if (memcpy_s(new_coords, maxBlocks_ * sizeof(float*),
                                 croodBlocks_, maxBlocks_ * sizeof(float*)) != EOK) {
                    }
                    if (memcpy_s(new_cmds, maxBlocks_ * sizeof(float*),
                                 cmdBlocks_, maxBlocks_ * sizeof(unsigned char*)) != EOK) {
                    }
                    GeometryArrayAllocator<float*>::Deallocate(croodBlocks_, maxBlocks_ * OHOS::TWO_TIMES);
                }
                croodBlocks_ = new_coords;
                cmdBlocks_ = new_cmds;
                maxBlocks_ += BLOCK_POOL;
            }
            croodBlocks_[nb] =
                GeometryArrayAllocator<float>::Allocate(BLOCK_SIZE * OHOS::TWO_TIMES +
                                                         BLOCK_SIZE / (sizeof(float) / sizeof(unsigned char)));

            cmdBlocks_[nb] =
                (unsigned char*)(croodBlocks_[nb] + BLOCK_SIZE * OHOS::TWO_TIMES);

            totalBlocks_++;
        }
        uint8_t* StoragePtrs(float** xy_ptr)
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
        float** croodBlocks_; // Input points
        uint8_t** cmdBlocks_; // Mark point status
    };

    class UICanvasVertices : public HeapBase {
    public:
        UICanvasVertices() : vertices_(), iterator_(0)
        {}

        /**
         * @brief Remove all vertices
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
         * @brief Free up space
         * @since 1.0
         * @version 1.0
         */
        void FreeAll()
        {
            vertices_.FreeAll();
            iterator_ = 0;
        }

        /**
         * @brief Move a point to the set coordinates
         * @param x Vertex X-axis coordinates
         * @param y Vertex Y-axis coordinates
         * @since 1.0
         * @version 1.0
         */
        void MoveTo(float x, float y)
        {
            vertices_.AddVertex(x, y, PATH_CMD_MOVE_TO);
        }
        /**
         * @brief Connect the line to the entered coordinates
         * @param x Vertex X-axis coordinates
         * @param y Vertex Y-axis coordinates
         * @since 1.0
         * @version 1.0
         */
        void LineTo(float x, float y)
        {
            vertices_.AddVertex(x, y, PATH_CMD_LINE_TO);
        }

        /**
         * @brief Draw an arc
         * @param rx Long and short axle
         * @param ry Long and short axle
         * @param angle angle
         * @param largeArcFlag Superior inferior arc
         * @param sweepFlag Clockwise and counterclockwise
         * @param x End X coordinate
         * @param y End Y-axis coordinates
         * @since 1.0
         * @version 1.0
         */
        void ArcTo(float rx, float ry,
                   float angle,
                   bool largeArcFlag,
                   bool sweepFlag,
                   float x, float y)
        {
            if (vertices_.TotalVertices() && IsVertex(vertices_.LastCommand())) {
                const float epsilon = 1e-30;
                float x0 = 0.0;
                float y0 = 0.0;
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
#if GRAPHIC_GEOMETYR_ENABLE_BEZIER_ARC_VERTEX_SOURCE
                BezierArcSvg bezierArcSvg(x0, y0, rx, ry, angle, largeArcFlag, sweepFlag, x, y);
                if (bezierArcSvg.RadiiOK()) {
                    JoinPath(bezierArcSvg);
                } else {
                    LineTo(x, y);
                }
#else
                LineTo(x, y);
#endif
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
         * @brief ClosePolygon Closed polygon path
         * @param flags
         */
        void ClosePolygon(unsigned flags = PATH_FLAGS_NONE)
        {
            EndPoly(PATH_FLAGS_CLOSE | flags);
        }

        /**
         * @brief Returns the number of vertices.
         * @return Returns the number of vertices.
         * @since 1.0
         * @version 1.0
         */
        unsigned TotalVertices() const
        {
            return vertices_.TotalVertices();
        }

        /**
         * @brief Returns the coordinates of the last vertex.
         * @param x Used to obtain the x-axis coordinates of the last vertex.
         * @param y Used to obtain the y-axis coordinates of the last vertex.
         * @return Returns the instruction type corresponding to the vertex.
         * @since 1.0
         * @version 1.0
         */
        unsigned LastVertex(float* x, float* y) const
        {
            return vertices_.LastVertex(x, y);
        }
        /**
         * @brief Gets the coordinates of a particular vertex.
         * @param idx Vertex subscript.
         * @param x Used to obtain the x-axis coordinates of vertices.
         * @param y Used to obtain the y-axis coordinates of vertices.
         * @return Returns the instruction type corresponding to the vertex.
         * @since 1.0
         * @version 1.0
         */
        unsigned Vertex(unsigned idx, float* x, float* y) const;

        /**
         * @brief Iterator fallback to a vertex。
         * @param pathId The vertex sequence number of the fallback.
         * @since 1.0
         * @version 1.0
         */
        void Rewind(unsigned pathId)
        {
            iterator_ = pathId;
        }

        /**
         * @brief Gets the coordinates of the next vertex.
         * @param x Used to obtain the x-axis coordinates of vertices.
         * @param y Used to obtain the y-axis coordinates of vertices.
         * @return Returns the instruction type corresponding to the next vertex.
         * @since 1.0
         * @version 1.0
         */
        unsigned Vertex(float* x, float* y)
        {
            if (iterator_ >= vertices_.TotalVertices()) {
                return PATH_CMD_STOP;
            }
            return vertices_.Vertex(iterator_++, x, y);
        }
#if GRAPHIC_GEOMETYR_ENABLE_BEZIER_ARC_VERTEX_SOURCE
        /**
         * @brief Connection path.
         * @param vs Vertex source to connect.
         * @param pathId Connection location.
         * @since 1.0
         * @version 1.0
         */
        void ConcatPath(BezierArc& path, unsigned pathId = 0)
        {
            float x, y;
            unsigned cmd;
            path.Rewind(pathId);
            while (!IsStop(cmd = path.Vertex(&x, &y))) {
                vertices_.AddVertex(x, y, cmd);
            }
        }

        /**
         * @brief Add vertex source to existing source.
         * @param vs Vertex source to join.
         * @param pathId Join location.
         * @since 1.0
         * @version 1.0
         */
        void JoinPath(BezierArcSvg& path, unsigned pathId = 0)
        {
            float x, y;
            unsigned pathCommand;
            path.Rewind(pathId);
            pathCommand = path.Vertex(&x, &y);
            if (!IsStop(pathCommand)) {
                if (IsVertex(pathCommand)) {
                    float x0, y0;
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
#endif
    private:
        VertexBlockStorage vertices_;
        unsigned iterator_;
    };
} // namespace OHOS
#endif
