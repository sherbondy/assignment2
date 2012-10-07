#include "Joint.h"


Matrix4f Joint::rotatedTransform() {
    return this->rotation * this->transform;
}