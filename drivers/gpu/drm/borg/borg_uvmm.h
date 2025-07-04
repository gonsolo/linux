/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright 2024-2025 Andreas Wendleder
 */

int borg_uvmm_ioctl_vm_bind(struct drm_device *dev, void *data,
			    struct drm_file *file_priv);
int borg_uvmm_ioctl_vm_init(struct drm_device *dev, void *data,
			    struct drm_file *file_priv);
