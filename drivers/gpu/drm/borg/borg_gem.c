/*
 * Copyright 2024 Andreas Wendleder
 */

#include <drm/drm_device.h>
#include <drm/drm_file.h>
#include <linux/types.h>
#include <uapi/drm/borg_drm.h>

int
borg_gem_new(struct borg_cli *cli, u64 size, struct borg_bo **pborgbo)
{
        // TODO
        return -1;
}

int borg_gem_ioctl_new(struct drm_device *dev, void *data, struct drm_file *file_priv)
{
        //struct nouveau_cli *cli = nouveau_cli(file_priv);
        //struct drm_nouveau_gem_new *req = data;
        //struct nouveau_bo *nvbo = NULL;
        int ret = -1;


        //ret = borg_gem_new(cli, req->info.size, req->align,
        //                      req->info.domain, req->info.tile_mode,
        //                      req->info.tile_flags, &nvbo);
        //if (ret)
        //        return ret;

        //ret = drm_gem_handle_create(file_priv, &nvbo->bo.base,
        //                            &req->info.handle);
        //if (ret == 0) {
        //        ret = nouveau_gem_info(file_priv, &nvbo->bo.base, &req->info);
        //        if (ret)
        //                drm_gem_handle_delete(file_priv, req->info.handle);
        //}

        ///* drop reference from allocate - handle holds it now */
        //drm_gem_object_put(&nvbo->bo.base);
        return ret;
}

