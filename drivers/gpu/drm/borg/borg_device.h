/* SPDX-License-Identifier:GPL-2.0 */

#ifndef __BORG_DEVICE_H__
#define __BORG_DEVICE_H__

#include <drm/drm_device.h>

extern const struct dev_pm_ops borg_pm_ops;

struct borg_device {
        struct drm_device base;
};

#endif
