/*
 * Copyright 2024 Andreas Wendleder
 */

#include <linux/device.h>
#include <linux/pm.h>

static int borg_device_runtime_resume(struct device *dev)
{
	return 0;
}

static int borg_device_runtime_suspend(struct device *dev)
{
	return 0;
}

static int borg_device_resume(struct device *dev)
{
	return 0;
}

static int borg_device_suspend(struct device *dev)
{
	return 0;
}

EXPORT_GPL_DEV_PM_OPS(borg_pm_ops) = {
	RUNTIME_PM_OPS(borg_device_runtime_suspend, borg_device_runtime_resume, NULL)
	SYSTEM_SLEEP_PM_OPS(borg_device_suspend, borg_device_resume)
};
