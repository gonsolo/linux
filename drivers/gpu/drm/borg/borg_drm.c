/*
 * Copyright 2024 Andreas Wendleder
 */

#include <drm/drm_drv.h>
#include <drm/drm_file.h>
#include <drm/drm_gem.h>
#include <drm/drm_ioctl.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <uapi/drm/borg_drm.h>
#include "borg_drv.h"
#include "borg_gem.h"
#include "borg_ioctl.h"

static struct drm_driver driver_stub;
static struct drm_driver driver_platform;

static const struct drm_ioctl_desc
borg_ioctls[] = {
        DRM_IOCTL_DEF_DRV(BORG_GEM_NEW, borg_gem_ioctl_new, DRM_RENDER_ALLOW),
};

static int __init
borg_drm_init(void)
{
        driver_platform = driver_stub;
        //platform_driver_register(&borg_platform_driver);
        return 0;
}

static void __exit
borg_drm_exit(void)
{
        // TODO
}

static int
borg_drm_open(struct drm_device *dev, struct drm_file *fpriv)
{
        // TODO
        return -1;
}

static void
borg_drm_postclose(struct drm_device *dev, struct drm_file *fpriv)
{
        // TODO
}

long
borg_drm_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
//        struct drm_file *filp = file->private_data;
//        struct drm_device *dev = filp->minor->dev;
        long ret;

//        ret = pm_runtime_get_sync(dev->dev);
//        if (ret < 0 && ret != -EACCES) {
//                pm_runtime_put_autosuspend(dev->dev);
//                return ret;
//        }

        ret = drm_ioctl(file, cmd, arg);

//        pm_runtime_mark_last_busy(dev->dev);
//        pm_runtime_put_autosuspend(dev->dev);
        return ret;
}

static const struct file_operations
borg_driver_fops = {
        .owner = THIS_MODULE,
        .open = drm_open,
        .release = drm_release,
        .unlocked_ioctl = borg_drm_ioctl,
        .mmap = drm_gem_mmap,
        .poll = drm_poll,
        .read = drm_read,
        .llseek = noop_llseek,
};

static struct drm_driver
driver_stub = {
        .driver_features = DRIVER_GEM | DRIVER_RENDER,
        .open = borg_drm_open,
        .postclose = borg_drm_postclose,
        //.lastclose = nouveau_vga_lastclose,

        .ioctls = borg_ioctls,
        .num_ioctls = ARRAY_SIZE(borg_ioctls),
        .fops = &borg_driver_fops,

        //.gem_prime_import_sg_table = nouveau_gem_prime_import_sg_table,

        //.dumb_create = nouveau_display_dumb_create,
        //.dumb_map_offset = drm_gem_ttm_dumb_map_offset,

        .name = DRIVER_NAME,
        .desc = DRIVER_DESC,
        .date = DRIVER_DATE,
        .major = DRIVER_MAJOR,
        .minor = DRIVER_MINOR,
        .patchlevel = DRIVER_PATCHLEVEL,
};

static int
borg_drm_device_init(struct drm_device *dev)
{
        return -1;
}

struct drm_device *
borg_platform_device_create(struct platform_device* pdev)
{
        struct drm_device *drm;
        int err;

        drm = drm_dev_alloc(&driver_platform, &pdev->dev);
        if (IS_ERR(drm)) {
                err = PTR_ERR(drm);
                goto err_free;
        }

        err = borg_drm_device_init(drm);
        if (err)
                goto err_put;

        platform_set_drvdata(pdev, drm);

        return drm;

err_put:
        drm_dev_put(drm);
err_free:
        //nvkm_device_del(pdevice);

        return ERR_PTR(err);
}
module_init(borg_drm_init);
module_exit(borg_drm_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL and additional rights");

