//
// Created by mingyi.li on 2020-03-22.
//

#include "Al2DCoordinate.h"
#include "AlOrthMatrix.h"
#include "AlRectF.h"
#include "AlVec4.h"
#include "AlMath.h"

Al2DCoordinate::Al2DCoordinate(int32_t xWide, int32_t yWide)
        : Object(), xWide(xWide), yWide(yWide) {

}

Al2DCoordinate::Al2DCoordinate(const Al2DCoordinate &o)
        : Object(), xWide(o.xWide), yWide(o.yWide) {

}

Al2DCoordinate::~Al2DCoordinate() {

}

void Al2DCoordinate::setScale(float scaleX, float scaleY) {
    scale.x = scaleX;
    scale.y = scaleY;
}

void Al2DCoordinate::setRotation(AlRational r) {
    rotation.num = r.num;
    rotation.den = r.den;
}

void Al2DCoordinate::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
}

void Al2DCoordinate::translate(AlVec2 *vec, Al2DCoordinate *dstCoord) {
    if (nullptr == vec || nullptr == dstCoord) {
        return;
    }
    AlRectF sRect(-1.f,
                  yWide / (float) xWide,
                  1.f,
                  -yWide / (float) xWide);
    AlRectF dRect(-1.f,
                  yWide / (float) xWide,
                  1.f,
                  -yWide / (float) xWide);
    double alpha = static_cast<float>(dstCoord->rotation.toFloat() * AlMath::PI);


    AlVec4 tVec(-dstCoord->position.x,
                -dstCoord->position.y);
    AlMatrix sMat0, sMat1, rMat;
    sMat0.setScale(dRect.getWidth() / 2.0f, dRect.getHeight() / 2.0f);
    sMat1.setScale(1 / dstCoord->scale.x, 1 / dstCoord->scale.y);
    rMat.setRotation(alpha);
    tVec = tVec * sMat0;
    /// 矩阵表示变换步骤时，刚好和实际顺序相反
    /// 缩放(sMat)->旋转(rMat)->位移(tMat) = vec * tMat * rMat * sMat
    tVec = tVec * rMat * sMat1;

    AlMatrix mMat;
    AlOrthMatrix oMat;

    oMat.update(dRect.left, dRect.right, dRect.bottom, dRect.top, -1.0f, 1.0f);
    mMat.setTranslate(tVec.x, -tVec.y);

    AlVec4 vec4(vec->x, vec->y);
    AlVec4 point = vec4 * mMat * oMat;;
    vec->x = point.x;
    vec->y = point.y;
}
