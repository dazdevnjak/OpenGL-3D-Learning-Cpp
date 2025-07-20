#include <LinearMath/btVector3.h>
#include <glm.hpp>

static class PhysicsUtilities {
public:
    static btVector3 glm_to_bt_vec3(const glm::vec3 glm_vec3) {
        return btVector3(glm_vec3.x, glm_vec3.y, glm_vec3.z);
    }

    static glm::vec3 bt_to_glm_vec3(const btVector3* bt_vec3) {
        return glm::vec3(bt_vec3->x(), bt_vec3->y(), bt_vec3->z());
    }
};