// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright 2024-2025 Andreas Wendleder
 */

#include <drm/drm_drv.h>
#include <drm/drm_file.h>
#include <drm/drm_gem.h>
#include <drm/drm_ioctl.h>

#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/printk.h>
#include <linux/regmap.h>

#include <uapi/drm/borg_drm.h>

#include "borg_device.h"
#include "borg_gem.h"
#include "borg_uvmm.h"

#define BORG_TEST1 0x000
#define BORG_KICK 0x020
#define BORG_COMPLETED 0x040
#define BORG_SHADER_POINTER 0x060
#define BORG_SHADER_SIZE 0X100

static inline u32 borg_gpu_read(struct borg_device *borg_dev, u32 reg)
{
	return readl(borg_dev->regs + reg);
}

static inline void borg_gpu_write(struct borg_device *borg_dev, u32 reg,
				  u32 value)
{
	writel(value, borg_dev->regs + reg);
}

static int borg_ioctl_getparam(struct drm_device *dev, void *data,
			       struct drm_file *file_priv)
{
	struct drm_borg_getparam *getparam = data;
	struct borg_device *borg_dev = to_borg_device(dev);
	u32 test1;

	switch (getparam->param) {
	case BORG_GETPARAM_STATUS:
		test1 = borg_gpu_read(borg_dev, BORG_TEST1);
		pr_info("Borg: status (test1): %d\n", test1);
		getparam->value = test1;
		break;
	default:
		pr_info("Borg: unknown parameter %lld\n", getparam->param);
		return -EINVAL;
	}
	return 0;
}

static int borg_ioctl_submit(struct drm_device *dev, void *data,
			     struct drm_file *file_priv)
{
	pr_info("Borg: submit\n");
	struct borg_device *borg_dev = to_borg_device(dev);
	struct drm_borg_submit *args = data;

	borg_gpu_write(borg_dev, BORG_SHADER_POINTER, args->shader_pointer);
	borg_gpu_write(borg_dev, BORG_SHADER_SIZE, args->shader_size);

	borg_gpu_write(borg_dev, BORG_KICK, 1);
	pr_info("Borg: wrote 1 to kick\n");

	while (borg_gpu_read(borg_dev, BORG_COMPLETED) != 1)
		pr_info("Borg: Waiting for completion.\n");
	pr_info("Borg: Completed submit\n");

	return 0;
}

static const struct drm_ioctl_desc borg_ioctls[] = {
	DRM_IOCTL_DEF_DRV(BORG_GEM_NEW, borg_gem_ioctl_new, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(BORG_GETPARAM, borg_ioctl_getparam, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(BORG_SUBMIT, borg_ioctl_submit, DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(BORG_VM_BIND, borg_uvmm_ioctl_vm_bind,
			  DRM_RENDER_ALLOW),
	DRM_IOCTL_DEF_DRV(BORG_VM_INIT, borg_uvmm_ioctl_vm_init,
			  DRM_RENDER_ALLOW),
};

static const struct file_operations borg_fops = {
	.owner = THIS_MODULE,
	DRM_GEM_FOPS,
	.show_fdinfo = drm_show_fdinfo,
};

static const struct drm_driver borg_drm_driver = {
	.driver_features = DRIVER_GEM | DRIVER_RENDER,
	.name = "borg",
	.desc = "borg DRM",
	.date = "20250128",
	.major = 0,
	.minor = 0,
	.patchlevel = 1,
	.ioctls = borg_ioctls,
	.num_ioctls = ARRAY_SIZE(borg_ioctls),
	.fops = &borg_fops,
};

static int borg_probe(struct platform_device *pdev)
{
	pr_info("Borg probe 1!");

	struct borg_device *borg_dev;
	struct drm_device *drm;
	struct device *dev = &pdev->dev;

	int ret = 0;

	borg_dev = devm_drm_dev_alloc(dev, &borg_drm_driver, struct borg_device,
				      base);
	if (IS_ERR(borg_dev)) {
		pr_info("Borg dev alloc failed!");
		return -ENOMEM;
	}
	pr_info("Borg dev alloc succeeded!");
	drm = &borg_dev->base;

	struct resource *mem_resource =
		platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!mem_resource) {
		pr_info("Borg: platform_get_resource failed.");
		return -EINVAL;
	}
	pr_info("Borg: mem resource: start %lli end %lli name %s.\n",
		mem_resource->start, mem_resource->end, mem_resource->name);

	struct resource *mem = devm_request_mem_region(
		&pdev->dev, mem_resource->start, resource_size(mem_resource),
		borg_drm_driver.name);
	if (!mem) {
		pr_info("Borg: Failed request mem region, trying raw");
		// Try raw resource
		mem = mem_resource;
	}

	borg_dev->regs =
		devm_ioremap(&pdev->dev, mem->start, resource_size(mem));
	if (!borg_dev->regs) {
		pr_info("Borg: devm_ioremap failed.");
		return -ENOMEM;
	}
	pr_info("Borg: regs: %p.\n", borg_dev->regs);

	u32 test1 = borg_gpu_read(borg_dev, BORG_TEST1);

	pr_info("Borg: test1: %i.\n", test1);

	platform_set_drvdata(pdev, drm);
	pr_info("Borg set drvdata ok!");

	ret = drm_dev_register(drm, 0);
	if (ret != 0) {
		pr_info("Borg drm_dev_register failed!");
		platform_set_drvdata(pdev, NULL);
		return ret;
	}

	pr_info("Borg probe ok!");
	return 0;
}

static void borg_remove(struct platform_device *pdev)
{
}

struct borg_compatible {};

static const struct borg_compatible default_data = {};

static const struct of_device_id dt_match[] = { {
							.compatible =
								"borg,borg-1",
							.data = &default_data,
						},
						{} };

MODULE_DEVICE_TABLE(of, dt_match);

static struct platform_driver borg_driver = {
	.probe		= borg_probe,
	.remove	        = borg_remove,
	.driver		= {
		.name	= "borg",
		.pm	= pm_ptr(&borg_pm_ops),
		.of_match_table = dt_match,
	},
};
module_platform_driver(borg_driver);

MODULE_AUTHOR("Andreas Wendleder");
MODULE_DESCRIPTION("Borg DRM Driver");
MODULE_LICENSE("GPL");
