/* SPDX-License-Identifier:GPL-2.0 */

#ifndef __BORG_GEM_H__
#define __BORG_GEM_H__

#include <drm/drm_device.h>
#include <drm/drm_gem.h>
#include <drm/drm_gem_shmem_helper.h>

#include <linux/container_of.h>
#include <linux/types.h>

struct borg_gem_object {
        struct drm_gem_shmem_object base;
};

extern struct borg_gem_object* borg_gem_create(struct drm_device *, u64);
extern int borg_gem_ioctl_new(struct drm_device *, void *, struct drm_file *);

static inline
struct  borg_gem_object *to_borg_bo(struct drm_gem_object *obj)
{
        return container_of(to_drm_gem_shmem_obj(obj), struct borg_gem_object, base);
}

#endif
