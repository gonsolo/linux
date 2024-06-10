#include <drm/drm_device.h>

extern const struct dev_pm_ops borg_pm_ops;

struct borg_device {
        struct drm_device base;
};
