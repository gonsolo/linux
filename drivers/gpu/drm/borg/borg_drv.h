/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __BORG_DRV_H__
#define __BORG_DRV_H__

#include <linux/platform_device.h>

#define DRIVER_AUTHOR           "Andreas Wendleder"
#define DRIVER_EMAIL            "andreas.wendleder@gmail.com"

#define DRIVER_NAME             "borg"
#define DRIVER_DESC             "Borg"
#define DRIVER_DATE             "20240625"

#define DRIVER_MAJOR            0
#define DRIVER_MINOR            0
#define DRIVER_PATCHLEVEL       1

struct drm_device *
borg_platform_device_create(struct platform_device*);

#endif
