/*
 * Copyright 2024 Andreas Wendleder
 */

#include <uapi/drm/borg_drm.h>
#include <drm/drm_ioctl.h>
#include "borg_gem.h"

static const struct drm_ioctl_desc
borg_ioctls[] = {
        DRM_IOCTL_DEF_DRV(BORG_GEM_NEW, borg_gem_ioctl_new, DRM_RENDER_ALLOW),
};

