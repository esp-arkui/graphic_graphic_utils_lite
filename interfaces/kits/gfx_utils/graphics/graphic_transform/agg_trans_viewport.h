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

#ifndef GRAPHIC_TRANS_VIEWPORT_INCLUDED
#define GRAPHIC_TRANS_VIEWPORT_INCLUDED

#include <cstring>
#include <cmath>
#include "gfx_utils/graphics/graphic_transform/agg_trans_affine.h"


namespace OHOS
{

    enum AspectRatioType
    {
        ASPECT_RATIO_STRETCH,
        ASPECT_RATIO_MEET,
        ASPECT_RATIO_SLICE
    };


 
    class TransViewPort
    {
    public:
       
        TransViewPort() : 
            worldX1_(0.0),
            worldY1_(0.0),
            worldX2_(1.0),
            worldY2_(1.0),
            deviceX1_(0.0),
            deviceY1_(0.0),
            deviceX2_(1.0),
            deviceY2_(1.0),
            aspect_(ASPECT_RATIO_STRETCH),
            isValid_(true),
            alignX_(0.5),
            alignY_(0.5),
            realWorldX1_(0.0),
            realWorldY1_(0.0),
            realWorldX2_(1.0),
            realWorldY2_(1.0),
            realDeviceX1_(0.0),
            realDeviceY1_(0.0),
            scaleX_(1.0),
            scaleY_(1.0)
        {}

      
        void PreserveAspectRatio(double alignX, 
                                   double alignY, 
                                   AspectRatioType aspect)
        {
            alignX_ = alignX;
            alignY_ = alignY;
            aspect_  = aspect;
            Update();
        }

        
        void DeviceViewPort(double x1, double y1, double x2, double y2)
        {
            deviceX1_ = x1;
            deviceY1_ = y1;
            deviceX2_ = x2;
            deviceY2_ = y2;
            Update();
        }

        
        void WorldViewPort(double x1, double y1, double x2, double y2)
        {
            worldX1_ = x1;
            worldY1_ = y1;
            worldX2_ = x2;
            worldY2_ = y2;
            Update();
        }

        /**
         * @brief 设定设备视野范围
         * 
         * @param x1 左上角x轴坐标
         * @param y1 左上角y轴坐标
         * @param x2 右下角x轴坐标
         * @param y2 右下角y轴坐标
         * @since 1.0
         * @version 1.0
         */
        void DeviceViewPort(double* x1, double* y1, double* x2, double* y2) const
        {
            *x1 = deviceX1_;
            *y1 = deviceY1_;
            *x2 = deviceX2_;
            *y2 = deviceY2_;
        }

        /**
         * @brief 设定世界视野范围
         * 
         * @param x1 左上角x轴坐标
         * @param y1 左上角y轴坐标
         * @param x2 右下角x轴坐标
         * @param y2 右下角y轴坐标
         * @since 1.0
         * @version 1.0
         */
        void WorldViewPort(double* x1, double* y1, double* x2, double* y2) const
        {
            *x1 = worldX1_;
            *y1 = worldY1_;
            *x2 = worldX2_;
            *y2 = worldY2_;
        }



        
        bool   IsValid() const 
        { 
            return isValid_; 
        }
        double AlignX() const 
        { 
            return alignX_; 
        }
        double AlignY() const 
        { 
            return alignY_; 
        }
        AspectRatioType AspectRatio() const 
        { 
            return aspect_; 
        }

  

        TransAffine ToAffine() const
        {
            TransAffine mtx = TransAffineTranslation(-realWorldX1_, -realWorldY1_);
            mtx *= TransAffineScaling(scaleX_, scaleY_);
            mtx *= TransAffineTranslation(realDeviceX1_, realDeviceY1_);
            return mtx;
        }


    private:
        /**
         * @brief 更新画布大小及坐标原点
         * 判定各坐标偏移量是否可以忽略，不可忽略时更新坐标
         * @since 1.0
         * @version 1.0
         */
        void Update();

        double         worldX1_;
        double         worldY1_;
        double         worldX2_;
        double         worldY2_;
        double         deviceX1_;
        double         deviceY1_;
        double         deviceX2_;
        double         deviceY2_;
        AspectRatioType    aspect_;
        bool           isValid_;
        double         alignX_;
        double         alignY_;
        double         realWorldX1_;
        double         realWorldY1_;
        double         realWorldX2_;
        double         realWorldY2_;
        double         realDeviceX1_;
        double         realDeviceY1_;
        double         scaleX_;
        double         scaleY_;
    };


    
    inline void TransViewPort::Update()
    {
        const double epsilon = 1e-30;
        if(std::fabs(worldX1_  - worldX2_)  < epsilon ||
           std::fabs(worldY1_  - worldY2_)  < epsilon ||
           std::fabs(deviceX1_ - deviceX2_) < epsilon ||
           std::fabs(deviceY1_ - deviceY2_) < epsilon){
            realWorldX1_ = worldX1_;
            realWorldY1_ = worldY1_;
            realWorldX2_ = worldX1_ + 1.0;
            realWorldY2_ = worldY2_ + 1.0;
            realDeviceX1_ = deviceX1_;
            realDeviceY1_ = deviceY1_;
            scaleX_  = 1.0;
            scaleY_  = 1.0;
            isValid_ = false;
            return;
        }
        double worldX1  = worldX1_;
        double worldY1  = worldY1_;
        double worldX2  = worldX2_;
        double worldY2  = worldY2_;
        double deviceX1 = deviceX1_;
        double deviceY1 = deviceY1_;
        double deviceX2 = deviceX2_;
        double deviceY2 = deviceY2_;
        if(aspect_ != ASPECT_RATIO_STRETCH){
            double d;
            scaleX_ = (deviceX2 - deviceX1) / (worldX2 - worldX1);
            scaleY_ = (deviceY2 - deviceY1) / (worldY2 - worldY1);

            if((aspect_ == ASPECT_RATIO_MEET) == (scaleX_ < scaleY_)){
                d = (worldY2 - worldY1) * scaleY_ / scaleX_;
                worldY1 += (worldY2 - worldY1 - d) * alignY_;
                worldY2 = worldY1 + d;
            }else{
                d = (worldX2 - worldX1) * scaleX_ / scaleY_;
                worldX1 += (worldX2 - worldX1 - d) * alignX_;
                worldX2 = worldX1 + d;
            }
        }
        realWorldX1_ = worldX1;
        realWorldY1_ = worldY1;
        realWorldX2_ = worldX2;
        realWorldY2_ = worldY2;
        realDeviceX1_ = deviceX1;
        realDeviceY1_ = deviceY1;
        scaleX_  = (deviceX2 - deviceX1) / (worldX2 - worldX1);
        scaleY_  = (deviceY2 - deviceY1) / (worldY2 - worldY1);
        isValid_ = true;
    }
}


#endif
