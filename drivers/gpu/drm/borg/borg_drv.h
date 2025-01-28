/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __BORG_DRV_H__
#define __BORG_DRV_H__

#include <drm/drm_file.h>

#include <linux/platform_device.h>

#define DRIVER_AUTHOR           "Andreas Wendleder"
#define DRIVER_EMAIL            "andreas.wendleder@gmail.com"

#define DRIVER_NAME             "borg"
#define DRIVER_DESC             "Borg"
#define DRIVER_DATE             "20250128"

#define DRIVER_MAJOR            0
#define DRIVER_MINOR            0
#define DRIVER_PATCHLEVEL       1

struct drm_device *
borg_platform_device_create(struct platform_device*);

#define BORG_PRINTK(l,c,f,a...) do {                                             \
        struct borg_cli *_cli = (c);                                           \
        dev_##l(_cli->drm->dev->dev, "%s: "f, _cli->name, ##a);                \
} while(0)

struct borg_drm {
        struct drm_device *dev;
};

struct borg_cli {
        struct borg_drm *drm;
        char name[32];
};

static inline struct borg_cli *
borg_cli(struct drm_file *fpriv)
{
        return fpriv ? fpriv->driver_priv : NULL;
}


#endif
