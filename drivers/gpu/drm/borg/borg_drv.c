#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/printk.h>

#include "borg_device.h"

static int borg_probe(struct platform_device *pdev)
{
	pr_info("Borg probe!");

	return 0;
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
