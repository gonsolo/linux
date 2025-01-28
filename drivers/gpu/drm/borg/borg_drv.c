/*
 * Copyright 2024-2025 Andreas Wendleder
 */

#include <drm/drm_drv.h>
#include <drm/drm_file.h>
#include <drm/drm_gem.h>
#include <drm/drm_ioctl.h>

#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/printk.h>

#include <uapi/drm/borg_drm.h>

#include "borg_device.h"
#include "borg_gem.h"
#include "borg_uvmm.h"

static int
borg_ioctl_getparam(struct drm_device *dev, void *data, struct drm_file *file_priv)
{
        struct drm_borg_getparam *getparam = data;

        switch (getparam->param) {
        case BORG_GETPARAM_STATUS: {
                __iomem uint32_t *addr = ioremap(0x4000, 0x0);
                pr_info("Borg: attempting to read status\n");
                uint64_t status = readl(addr) & 0x1;
                pr_info("Borg: status: %lld\n", status);
                getparam->value = status;
                break;
        }
        default:
                pr_info("Borg: unknown parameter %lld\n", getparam->param);
                return -EINVAL;
        }
        return 0;
}

static const struct drm_ioctl_desc borg_ioctls[] = {
        DRM_IOCTL_DEF_DRV(BORG_GETPARAM, borg_ioctl_getparam, DRM_RENDER_ALLOW),
        DRM_IOCTL_DEF_DRV(BORG_VM_INIT, borg_uvmm_ioctl_vm_init, DRM_RENDER_ALLOW),
        DRM_IOCTL_DEF_DRV(BORG_VM_BIND, borg_uvmm_ioctl_vm_bind, DRM_RENDER_ALLOW),
        DRM_IOCTL_DEF_DRV(BORG_GEM_NEW, borg_gem_ioctl_new, DRM_RENDER_ALLOW),
};

static const struct file_operations borg_fops = {
        .owner = THIS_MODULE,
        DRM_GEM_FOPS,
        .show_fdinfo = drm_show_fdinfo,
};

static const struct drm_driver borg_drm_driver = {
        .driver_features        = DRIVER_GEM | DRIVER_RENDER,
        .name                   = "borg",
        .desc                   = "borg DRM",
        .date                   = "20250128",
        .major                  = 0,
        .minor                  = 0,
        .patchlevel             = 1,
        .ioctls                 = borg_ioctls,
        .num_ioctls             = ARRAY_SIZE(borg_ioctls),
        .fops                   = &borg_fops,
};

static int borg_probe(struct platform_device *pdev)
{
	pr_info("Borg probe 1!");

        struct borg_device *borg_dev;
        struct drm_device *drm;
        int ret = 0;

        borg_dev = devm_drm_dev_alloc(&pdev->dev, &borg_drm_driver,
                                     struct borg_device, base);
        if (IS_ERR(borg_dev)) {
	        pr_info("Borg dev alloc failed!");
                return -ENOMEM;
        }
	pr_info("Borg dev alloc succeeded!");

        drm = &borg_dev->base;
        platform_set_drvdata(pdev, drm);
	pr_info("Borg set drvdata ok!");

        ret = drm_dev_register(drm, 0);
        if (ret < 0) {
	        pr_info("Borg drm_dev_register failed!");
                goto err;
        }

	pr_info("Borg probe ok!");
        return 0;
err:
        platform_set_drvdata(pdev, NULL);
	return ret;
}

static void borg_remove(struct platform_device *pdev)
{

}

struct borg_compatible {};

static const struct borg_compatible default_data = {
};

static const struct of_device_id dt_match[] = {
	{ .compatible = "borg,borg-1", .data = &default_data, },
	{}
};

MODULE_DEVICE_TABLE(of, dt_match);

static struct platform_driver borg_driver = {
	.probe		= borg_probe,
	.remove_new	= borg_remove,
	.driver		= {
		.name	= "borg",
		.pm	= pm_ptr(&borg_pm_ops),
		.of_match_table = dt_match,
	},
};
module_platform_driver(borg_driver);

MODULE_AUTHOR("Andreas Wendleder");
MODULE_DESCRIPTION("Borg DRM Driver");
MODULE_LICENSE("GPL v2");
