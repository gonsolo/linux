/* SPDX-License-Identifier:GPL-2.0 */

#ifndef __BORG_DEVICE_H__
#define __BORG_DEVICE_H__

#include <drm/drm_device.h>

extern const struct dev_pm_ops borg_pm_ops;

struct borg_device {
        struct drm_device base;
        void __iomem *regs;
};

static inline struct borg_device *to_borg_device(const struct drm_device *dev)
{
        return container_of(dev, struct borg_device, base);
}

#endif
