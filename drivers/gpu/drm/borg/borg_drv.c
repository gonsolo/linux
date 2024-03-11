// SPDX-License-Identifier: GPL-2.0
/* Copyright 2024 Andreas Wendleder <andreas.wendleder@gmail.com> */

#include <linux/platform_device.h>

static int borg_probe(struct platform_device *pdev)
{
	return 0;
}

static void borg_remove(struct platform_device *pdev)
{
}

static struct platform_driver borg_driver = {
	.probe		= borg_probe,
	.remove_new	= borg_remove,
	.driver		= {
		.name	= "borg",
	},
};
module_platform_driver(borg_driver);

MODULE_AUTHOR("Andreas Wendleder");
MODULE_DESCRIPTION("Borg DRM Driver");
MODULE_LICENSE("GPL v2");
