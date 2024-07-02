/*
 * Copyright 2024 Andreas Wendleder
 */

#include <drm/drm_device.h>
#include <drm/drm_file.h>
#include <drm/drm_gem.h>

#include <linux/types.h>

#include <uapi/drm/borg_drm.h>

#include "borg_gem.h"

struct borg_gem_object* borg_gem_create(struct drm_device* dev, u64 size)
{
        struct drm_gem_shmem_object *shmem;
        struct borg_gem_object *bo;

        shmem = drm_gem_shmem_create(dev, size);
        if (IS_ERR(shmem))
                return ERR_CAST(shmem);

        bo = to_borg_bo(&shmem->base);

        return bo;
}

int
borg_gem_ioctl_new(struct drm_device *dev, void *data, struct drm_file *file_priv)
{
        pr_info("Borg: borg_gem_ioctl_new!");
        struct drm_borg_gem_new *req = data;
        struct borg_gem_object *bo = NULL;
        int ret = -1;

        bo = borg_gem_create(dev, req->info.size);
        if (ret)
                return ret;

        ret = drm_gem_handle_create(file_priv, &bo->base.base, &req->info.handle);

        drm_gem_object_put(&bo->base.base);
        return ret;
}

